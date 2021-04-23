////////////////////////////////////////////////////////////////////////////////
// IME - Infinite Motion Engine
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "IME/core/tilemap/TileMap.h"
#include "IME/core/tilemap/TileMapParser.h"
#include "IME/core/resources/ResourceManager.h"
#include "IME/core/physics/rigid_body/colliders/BoxCollider.h"
#include "IME/core/physics/World.h"
#include "IME/graphics/Window.h"

namespace ime {
    TileMap::TileMap(unsigned int tileWidth, unsigned int tileHeight,
            RenderLayerContainer& renderLayers,
            GameObjectContainer& childContainer) :
        tileSpacing_{1u},
        isVisible_(true),
        invalidTile_({0, 0}, {-1, -1}),
        renderLayers_{renderLayers},
        childContainer_{childContainer},
        sprites_{renderLayers_}
    {
        invalidTile_.setIndex({-1, -1});
        mapPos_ = {0, 0};
        numOfRows_ = numOfColms_ = 0u;
        if (tileWidth <= 0)
            tileWidth = 8;

        if (tileHeight <= 0)
            tileHeight = 8;

        tileSize_ = Vector2u{tileWidth, tileHeight};
        backgroundTile_.setFillColour(Colour::Grey);
    }

    void TileMap::setPhysicsSimulation(std::shared_ptr<World> physicsSimulation) {
        physicsSim_ = std::move(physicsSimulation);
    }

    void TileMap::setVisible(bool visible) {
        if (isVisible_ == visible)
            return;

        isVisible_ = visible;

        forEachTile_([visible](Tile& tile) {
            tile.setVisible(visible);
        });

        if (isVisible_)
            backgroundTile_.setFillColour(Colour::Grey);
        else
            backgroundTile_.setFillColour(ime::Colour::Transparent);
    }

    bool TileMap::isVisible() const {
        return isVisible_;
    }

    void TileMap::toggleVisibility() {
        setVisible(!isVisible_);
    }

    const Tile& TileMap::getTile(const Vector2f &position) {
        for (auto& tileRows : tiledMap_) {
            for (auto& tile : tileRows)
                if (tile.contains(position))
                    return tile;
        }
        return invalidTile_;
    }

    const Tile &TileMap::getTileAbove(const Tile &tile) {
        return getTileAbove(tile.getIndex());
    }

    const Tile &TileMap::getTileBelow(const Tile &tile) {
        return getTileBelow(tile.getIndex());
    }

    const Tile &TileMap::getTileLeftOf(const Tile &tile) {
        return getTileLeftOf(tile.getIndex());
    }

    const Tile& TileMap::getTileRightOf(const Tile &tile) {
        return getTileRightOf(tile.getIndex());
    }

    bool TileMap::isIndexValid(const Index &index) const {
        auto [row, colm] = index;
        return !(row >= static_cast<int>(numOfRows_) || row < 0 || colm >= static_cast<int>(numOfColms_) || colm < 0);
    }

    void TileMap::construct(Vector2u size, char id) {
        for (auto i = 0u; i < size.x; ++i) {
            auto innerVector = std::vector<char>(size.y, id);
            mapData_.push_back(std::move(innerVector));
        }

        computeDimensions();
        createTiledMap();
        createObjectList();
    }

    void TileMap::loadFromFile(const std::string &filename, const char& separator) {
        mapData_ = TileMapParser::parse(filename, separator);
        computeDimensions();
        createTiledMap();
        createObjectList();
    }

    void TileMap::loadFromVector(Map map) {
        mapData_ = std::move(map);
        computeDimensions();
        createTiledMap();
        createObjectList();
    }

    void TileMap::computeDimensions() {
        numOfRows_ = mapData_.size();
        numOfColms_ = mapData_[0].size();
        mapSizeInPixels_.x = numOfColms_ * tileSize_.y + (numOfColms_ + 1) * tileSpacing_;
        mapSizeInPixels_.y = numOfRows_ * tileSize_.x + (numOfRows_ + 1) * tileSpacing_;
        backgroundTile_.setSize({static_cast<float>(mapSizeInPixels_.x), static_cast<float>(mapSizeInPixels_.y)});
    }

    void TileMap::setCollidable(Tile &tile, bool collidable) {
        if (collidable && !tile.hasCollider()) {
            auto collider = BoxCollider::create(Vector2f{static_cast<float>(tile.getSize().x), static_cast<float>(tile.getSize().y)});
            auto body = physicsSim_->createBody();
            body->attachCollider(collider);
            tile.attachCollider(std::move(collider));
            tile.setCollidable(true);
        } else
            tile.setCollidable(collidable);
    }

    void TileMap::setPosition(int x, int y) {
        mapPos_.x = static_cast<float>(x);
        mapPos_.y = static_cast<float>(y);
        backgroundTile_.setPosition(mapPos_);

        for (auto i = 0u; i < tiledMap_.size(); i++) {
            for (auto j = 0u; j < mapData_[i].size(); j++) {
                if (i == 0 && j == 0)
                    tiledMap_[i][j].setPosition(mapPos_.x + tileSpacing_, mapPos_.y + tileSpacing_);
                else if (j == 0)
                    tiledMap_[i][j].setPosition( {mapPos_.x + tileSpacing_, tiledMap_[i - 1][j].getPosition().y + tileSize_.y + tileSpacing_});
                else
                    tiledMap_[i][j].setPosition( {tiledMap_[i][j - 1].getPosition().x + tileSize_.x + tileSpacing_, tiledMap_[i][j - 1].getPosition().y});
            }
        }
    }

    void TileMap::setTileset(const std::string& name, const std::string &filename) {
        ResourceManager::getInstance()->loadFromFile(ResourceType::Texture, filename);
        tilesets_.insert({name, filename});
    }

    void TileMap::setCurrentTileset(const std::string &name) {
        if (tilesets_.find(name) != tilesets_.end())
            tileSet_ = tilesets_.at(name);
    }

    Vector2f TileMap::getPosition() const {
        return mapPos_;
    }

    void TileMap::createTiledMap() {
        for (auto i = 0u; i < mapData_.size(); i++) {
            auto row = std::vector<Tile>{};
            for (auto j = 0u; j < mapData_[i].size(); j++) {
                row.emplace_back(Tile(tileSize_, {-99, -99}));
                if (i == 0 && j == 0)
                    row.back().setPosition(mapPos_.x + tileSpacing_, mapPos_.y + tileSpacing_);
                else if (j == 0)
                    row.back().setPosition( {mapPos_.x + tileSpacing_, tiledMap_[i - 1][j].getPosition().y + tileSize_.y + tileSpacing_});
                else
                    row.back().setPosition( {row[j - 1].getPosition().x + tileSize_.x + tileSpacing_, row[j - 1].getPosition().y});

                row.back().setId(mapData_[i][j]);
                row.back().setIndex({static_cast<int>(i), static_cast<int>(j)});
            }
            tiledMap_.push_back(std::move(row));
        }
    }

    void TileMap::createObjectList() {
        forEachTile([this](const Tile& tile) {
            children_.emplace(tile.getIndex(), std::vector<std::shared_ptr<GameObject>>{});
        });
    }

    void TileMap::draw(Window &renderTarget) {
        renderTarget.draw(backgroundTile_);
        forEachTile([&renderTarget](const Tile& tile) {
            renderTarget.draw(tile);
        });
    }

    void TileMap::addSprite(Sprite::Ptr sprite, const Index& index, int renderOrder, const std::string &renderLayer) {
        IME_ASSERT(sprite, "Sprite cannot be a nullptr")
        renderLayers_.add(*sprite, renderOrder, renderLayer);
        sprite->setPosition(getTile(index).getWorldCentre());
        sprites_.add(std::move(sprite));
    }

    void TileMap::setCollidableByIndex(const Index &index, bool isCollidable) {
        if (isIndexValid(index))
                setCollidable(tiledMap_[index.row][index.colm], isCollidable);
    }

    void TileMap::setCollidableByIndex(const std::initializer_list<Index> &locations, bool isCollidable) {
        std::for_each(locations.begin(), locations.end(), [=](const Index& index) {
            setCollidableByIndex(index, isCollidable);
        });
    }

    void TileMap::setCollidableByIndex(Index startPos, Index endPos, bool isCollidable) {
        if (isIndexValid(startPos) && isIndexValid(endPos)){
            for (auto i = startPos.colm; i < endPos.colm; i++)
                setCollidableByIndex({startPos.row, i}, isCollidable);
        }
    }

    void TileMap::setCollidableById(char id, bool isCollidable) {
        forEachTile_([id, isCollidable, this](Tile& tile) {
            if (tile.getId() == id)
                setCollidable(tile, isCollidable);
        });
    }

    void TileMap::setCollidableByExclusion(char id, bool isCollidable) {
        forEachTile_([id, isCollidable, this](Tile& tile) {
            if (tile.getId() != id)
                setCollidable(tile, isCollidable);
        });
    }

    const Tile& TileMap::getTile(const Index &index) {
        if (isIndexValid(index))
            return tiledMap_[index.row][index.colm];
        return invalidTile_;
    }

    bool TileMap::isCollidable(const Index &index) const {
        if (isIndexValid(index))
            return tiledMap_[index.row][index.colm].isCollidable();
        return false;
    }

    bool TileMap::addChild(std::shared_ptr<GameObject> child, const Index& index, bool assignLayer) {
        IME_ASSERT(child, "Child cannot be a nullptr")
        if (isIndexValid(index) && !hasChild(child)) {
            child->getTransform().setPosition(getTile(index).getWorldCentre());
            if (child->hasRigidBody())
                child->getRigidBody()->setPosition(getTile(index).getWorldCentre());

            if (assignLayer)
                childContainer_.add(child); // Adds child to container and its sprite to the "default" render layer
            else
                childContainer_.addObject(child); // Just adds child to container

            children_[index].push_back(std::move(child));
            return true;
        }

        return false;
    }

    bool TileMap::hasChild(const GameObject::Ptr& child) {
        if (!child)
            return false;

        for (auto& childList : children_) {
            for (auto i = 0u; i < childList.second.size(); ++i)
                if (childList.second[i] == child)
                    return true;
        }
        return false;
    }

    std::shared_ptr<GameObject> TileMap::getChildWithId(std::size_t id) const {
        for (const auto& childList : children_) {
            for (auto i = 0u; i < childList.second.size(); ++i)
                if (childList.second[i]->getObjectId() == id)
                    return childList.second[i];
        }
        return nullptr;
    }

    bool TileMap::isTileOccupied(const Tile& tile) const {
        return !children_.at(tile.getIndex()).empty();
    }

    bool TileMap::tileHasVisitors(const Tile &tile) const {
        return children_.at(tile.getIndex()).size() > 1;
    }

    std::shared_ptr<GameObject> TileMap::getOccupant(const Tile& tile) {
        if (isTileOccupied(tile))
            return children_[tile.getIndex()].front();
        return nullptr;
    }

    void TileMap::forEachChild(const Callback<const GameObject::Ptr&>& callback) {
        std::for_each(children_.begin(), children_.end(), [&callback](auto& childList) {
            std::for_each(childList.second.begin(), childList.second.end(),
                [&callback] (const GameObject::Ptr& child) {
                    callback(child);
            });
        });
    }

    void TileMap::forEachChildInTile(const Tile& tile, const Callback<const GameObject::Ptr&>& callback) {
        if (isTileOccupied(tile)) {
            std::for_each(children_[tile.getIndex()].begin(), children_[tile.getIndex()].end(),
                [&callback](const GameObject::Ptr& child) {
                    callback(child)
            ;});
        }
    }

    std::size_t TileMap::getNumOfOccupants(const Tile &tile) const {
        if (isTileOccupied(tile))
            return children_.at(tile.getIndex()).size();
        return 0;
    }

    void TileMap::update(Time deltaTime) {
        sprites_.forEach([&deltaTime](const Sprite::Ptr& sprite) {
            sprite->updateAnimation(deltaTime);
        });
    }

    bool TileMap::removeChildFromTile(const Tile& tile, const GameObject::Ptr& child) {
        if (isTileOccupied(tile)) {
            if (!tileHasVisitors(tile) && getOccupant(tile) == child)
                return removeOccupant(tile);

            for (auto i = 0u; i < children_[tile.getIndex()].size(); ++i) {
                if (children_[tile.getIndex()][i] == child) {
                    children_[tile.getIndex()].erase(children_[tile.getIndex()].begin() + i);
                    return true;
                }
            }
        }
        return false;
    }

    bool TileMap::removeOccupant(const Tile &tile) {
        if (isTileOccupied(tile)) {
            children_[tile.getIndex()].erase(children_[tile.getIndex()].begin());
            return true;
        }
        return false;
    }

    bool TileMap::removeChildWithId(std::size_t id) {
        for (auto& childList : children_) {
            for (auto i = 0u; i < childList.second.size(); ++i)
                if (childList.second[i]->getObjectId() == id) {
                    childList.second.erase(childList.second.begin() + i);
                    return true;
                }
        }
        return false;
    }

    bool TileMap::removeChild(const GameObject::Ptr& child) {
        if (!child)
            return false;
        return removeChildWithId(child->getObjectId());
    }

    void TileMap::removeChildrenIf(const std::function<bool(const GameObject::Ptr&)>& callback) {
        for (auto& childList : children_)
            childList.second.erase(std::remove_if(childList.second.begin(), childList.second.end(), callback), childList.second.end());
    }

    bool TileMap::removeAllVisitors(const Tile &tile) {
        if (!tileHasVisitors(tile))
            return false;
        else {
            auto occupant = std::move(children_[tile.getIndex()].front());
            children_[tile.getIndex()].clear();
            children_[tile.getIndex()].push_back(std::move(occupant));
            return true;
        }
    }

    bool TileMap::removeAllChildren(const Tile &tile) {
        if (isTileOccupied(tile)) {
            children_[tile.getIndex()].clear();
            return true;
        }
        return false;
    }

    void TileMap::moveChild(const GameObject::Ptr& child, const Index& index) {
        if (hasChild(child) && isIndexValid(index) && index != getTileOccupiedByChild(child).getIndex()) {
            removeChildFromTile(getTileOccupiedByChild(child), child);
            addChild(child, index, false);
        }
    }

    void TileMap::moveChild(const GameObject::Ptr& child, const Tile &tile) {
        moveChild(child, tile.getIndex());
    }

    Vector2u TileMap::getTileSize() const {
        return tileSize_;
    }

    RenderLayerContainer &TileMap::renderLayers() {
        return renderLayers_;
    }

    void TileMap::textureTile(const Index& index, const UIntRect& rect) {
        IME_ASSERT(!tilesets_.empty(), "Cannot texture tile with an empty tileset, set tileset to be used first");
        if (isIndexValid(index)) {
            auto sprite = Sprite(tileSet_);
            sprite.setTextureRect(rect.left, rect.top, rect.width, rect.height);
            // @TODO add sprite to textures - Dont know the render layer to add it to
        }
    }

    void TileMap::textureTilesById(char id, const UIntRect& rect) {
        forEachTile([id, &rect, this](const Tile& tile) {
            if (tile.getId() == id)
                textureTile(tile.getIndex(), rect);
        });
    }

    void TileMap::textureTilesById(char id, const Sprite &sprite) {
        forEachTile([&sprite, id](const Tile& tile) {
            if (tile.getId() == id) {
                //addSprite(sprite, tile.getIndex()); @TODO fix - dont know the layer to put it in
            }
        });
    }

    void TileMap::forEachTile(const Callback<const Tile&>& callback) {
        std::for_each(tiledMap_.begin(), tiledMap_.end(), [&callback](auto& row) {
            std::for_each(row.begin(), row.end(), callback);
        });
    }

    void TileMap::forEachTile_(const Callback<Tile&> &callback) {
        std::for_each(tiledMap_.begin(), tiledMap_.end(), [&callback](auto& row) {
            std::for_each(row.begin(), row.end(), callback);
        });
    }

    void TileMap::forEachTileWithId(char id, const Callback<const Tile&>& callback) {
        forEachTile([&callback, id](const Tile& tile) {
            if (tile.getId() == id)
                callback(tile);
        });
    }

    void TileMap::forEachTileExcept(char id, const Callback<const Tile&>& callback) {
        forEachTile([&callback, id](const Tile& tile) {
            if (tile.getId() != id)
                callback(tile);
        });
    }

    void TileMap::forEachTileInRange(Index startPos, Index endPos, const Callback<const Tile&>& callback) {
        if (isIndexValid(startPos) && isIndexValid(endPos)) {
            std::for_each(tiledMap_[startPos.row].begin() + startPos.colm,
                tiledMap_[startPos.row].begin() + endPos.colm,
                [&callback](const Tile& tile) {
                    callback(tile);
            });
        }
    }

    const Tile& TileMap::getTileAbove(const Index &index) {
        return getTile(Index{index.row - 1, index.colm});
    }

    const Tile& TileMap::getTileBelow(const Index &index) {
        return getTile(Index{index.row + 1, index.colm});
    }

    const Tile& TileMap::getTileLeftOf(const Index &index) {
        return getTile(Index{index.row, index.colm - 1});
    }

    const Tile& TileMap::getTileRightOf(const Index &index) {
        return getTile(Index{index.row, index.colm + 1});
    }

    Vector2u TileMap::getSize() const {
        return mapSizeInPixels_;
    }

    unsigned int TileMap::getSpaceBetweenTiles() const {
        return tileSpacing_;
    }

    Vector2u TileMap::getSizeInTiles() const {
        return {numOfColms_, numOfRows_};
    }

    const Tile& TileMap::getTileOccupiedByChild(const GameObject::Ptr& child) {
        return getTile(child->getTransform().getPosition());
    }
}
