////////////////////////////////////////////////////////////////////////////////
// IME - Infinite Motion Engine
//
// Copyright (c) 2020-2022 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
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
#include "IME/core/physics/rigid_body/PhysicsEngine.h"
#include "IME/core/object/GridObject.h"
#include "IME/graphics/RenderTarget.h"
#include <algorithm>

namespace ime {
    bool isInTile(GridObject* child, const Tile& tile) {
        if (child) {
            return tile.contains(child->getTransform().getPosition());
        }

        return false;
    }

    TileMap::TileMap(unsigned int tileWidth, unsigned int tileHeight, Scene& scene) :
        scene_{scene},
        tileSpacing_{1u},
        invalidTile_({0, 0}, {-1, -1}),
        physicsSim_{nullptr}
    {
        invalidTile_.setIndex({-1, -1});
        mapPos_ = {0, 0};
        numOfRows_ = numOfColms_ = 0u;
        if (tileWidth <= 0)
            tileWidth = 8;

        if (tileHeight <= 0)
            tileHeight = 8;

        tileSize_ = Vector2u{tileWidth, tileHeight};

        backgroundTile_.setFillColour(renderer_.getGridLineColour());
        renderer_.onPropertyChange([this](const Property& property){
            onRenderChange(property);
        });
    }

    void TileMap::setPhysicsEngine(PhysicsEngine* engine) {
        physicsSim_ = engine;
    }

    Scene &TileMap::getScene() {
        return scene_;
    }

    const Scene &TileMap::getScene() const {
        return scene_;
    }

    unsigned int TileMap::getRowCount() const {
        return numOfRows_;
    }

    unsigned int TileMap::getColumnCount() const {
        return numOfColms_;
    }

    TileMapRenderer &TileMap::getRenderer() {
        return renderer_;
    }

    const Tile& TileMap::getTile(const Vector2f &position) const {
        for (auto& tileRows : tiledMap_) {
            for (auto& tile : tileRows) {
                if (tile.contains(position))
                    return tile;
            }
        }

        return invalidTile_;
    }

    const Tile &TileMap::getTileAbove(const Tile &tile) const {
        return getTileAbove(tile.getIndex());
    }

    const Tile &TileMap::getTileBelow(const Tile &tile) const {
        return getTileBelow(tile.getIndex());
    }

    const Tile &TileMap::getTileLeftOf(const Tile &tile) const {
        return getTileLeftOf(tile.getIndex());
    }

    const Tile& TileMap::getTileRightOf(const Tile &tile) const {
        return getTileRightOf(tile.getIndex());
    }

    bool TileMap::isIndexValid(const Index &index) const {
        auto [row, colm] = index;
        return !(row >= static_cast<int>(numOfRows_) || row < 0 || colm >= static_cast<int>(numOfColms_) || colm < 0);
    }

    void TileMap::construct(const Vector2u& size, char id) {
        for (auto i = 0u; i < size.x; ++i) {
            auto innerVector = std::vector<char>(size.y, id);
            mapData_.push_back(std::move(innerVector));
        }

        computeDimensions();
        createTiledMap();
    }

    void TileMap::loadFromFile(const std::string &filename, const char& separator) {
        mapData_ = TileMapParser::parse(filename, separator);
        computeDimensions();
        createTiledMap();
    }

    void TileMap::loadFromVector(Map map) {
        mapData_ = std::move(map);
        computeDimensions();
        createTiledMap();
    }

    void TileMap::computeDimensions() {
        numOfRows_ = static_cast<unsigned int>(mapData_.size());
        numOfColms_ = static_cast<unsigned int>(mapData_[0].size());
        mapSizeInPixels_.x = numOfColms_ * tileSize_.y + (numOfColms_ + 1) * tileSpacing_;
        mapSizeInPixels_.y = numOfRows_ * tileSize_.x + (numOfRows_ + 1) * tileSpacing_;
        backgroundTile_.setSize({static_cast<float>(mapSizeInPixels_.x), static_cast<float>(mapSizeInPixels_.y)});
    }

    void TileMap::setCollidable(Tile &tile, bool collidable, bool attachCollider) {
        if (tile.isCollidable() == collidable)
            return;
        else if (collidable && !tile.hasCollider() && attachCollider && physicsSim_) {
            tile.setBody(physicsSim_->createBody(RigidBody::Type::Static));
            tile.attachCollider(BoxCollider::create(Vector2f{static_cast<float>(tile.getSize().x), static_cast<float>(tile.getSize().y)}));
        }

        tile.setCollidable(collidable);

        if (collidable)
            tile.setFillColour(renderer_.getCollidableTileColour());
        else
            tile.setFillColour(renderer_.getTileColour());
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
                row.back().setVisible(renderer_.isVisible());
                row.back().setFillColour(renderer_.getTileColour());
            }
            tiledMap_.push_back(std::move(row));
        }
    }

    void TileMap::draw(priv::RenderTarget &renderTarget) const {
        if (renderer_.isVisible()) {
            renderTarget.draw(backgroundTile_);
            forEachTile([&renderTarget](const Tile &tile) {
                renderTarget.draw(tile);
            });
        }
    }

    void TileMap::setCollidableByIndex(const Index &index, bool isCollidable, bool attachCollider) {
        if (isIndexValid(index))
            setCollidable(tiledMap_[index.row][index.colm], isCollidable, attachCollider);
    }

    void TileMap::setCollidableByIndex(const std::initializer_list<Index> &locations, bool isCollidable, bool attachCollider) {
        std::for_each(locations.begin(), locations.end(), [=](const Index& index) {
            setCollidableByIndex(index, isCollidable, attachCollider);
        });
    }

    void TileMap::setCollidableByIndex(Index startPos, Index endPos, bool isCollidable, bool attachCollider) {
        if (isIndexValid(startPos) && isIndexValid(endPos)){
            for (auto i = startPos.colm; i < endPos.colm; i++)
                setCollidableByIndex({startPos.row, i}, isCollidable, attachCollider);
        }
    }

    void TileMap::setCollidableById(char id, bool isCollidable, bool attachCollider) {
        forEachTile_([=](Tile& tile) {
            if (tile.getId() == id)
                setCollidable(tile, isCollidable, attachCollider);
        });
    }

    void TileMap::setCollidableByExclusion(char id, bool isCollidable, bool attachCollider) {
        forEachTile_([=](Tile& tile) {
            if (tile.getId() != id)
                setCollidable(tile, isCollidable, attachCollider);
        });
    }

    const Tile& TileMap::getTile(const Index &index) const {
        if (isIndexValid(index))
            return tiledMap_[index.row][index.colm];

        return invalidTile_;
    }

    bool TileMap::isCollidable(const Index &index) const {
        if (isIndexValid(index))
            return tiledMap_[index.row][index.colm].isCollidable();

        return false;
    }

    bool TileMap::addChild(GridObject* child, const Index& index) {
        IME_ASSERT(child, "Child cannot be a nullptr")
        if (isIndexValid(index) && children_.insert(child).second) {
            destructionIds_[child->getObjectId()] = child->onDestruction([this, id = child->getObjectId()]{
                removeChildWithId(id);
            });

            child->getTransform().setPosition(getTile(index).getWorldCentre());
            child->setGrid(this);

            return true;
        }

        return false;
    }

    bool TileMap::hasChild(const GridObject* child) const {
        return std::find(children_.begin(), children_.end(), child) != children_.end();
    }

    GridObject* TileMap::getChildWithId(std::size_t id) const {
        for (const auto& child : children_) {
            if (child->getObjectId() == id)
                return child;
        }

        return nullptr;
    }

    void TileMap::forEachChild(const Callback<GridObject*>& callback) const {
        std::for_each(children_.begin(), children_.end(), [&callback](auto& child) {
            callback(child);
        });
    }

    void TileMap::forEachChildInTile(const Tile& tile, const Callback<GridObject*>& callback) const {
        forEachChild([&](GridObject* child) {
            if (isInTile(child, tile))
                callback(child);
        });
    }

    void TileMap::update(Time) {

    }

    bool TileMap::removeChildWithId(std::size_t id) {
        for (auto& child : children_) {
            if (child->getObjectId() == id) {
                unsubscribeDestructionListener(child);
                children_.erase(child);
                child->setGrid(nullptr);

                return true;
            }
        }

        return false;
    }

    bool TileMap::removeChild(GridObject* child) {
        if (!child)
            return false;

        return removeChildWithId(child->getObjectId());
    }

    void TileMap::removeChildIf(const std::function<bool(GridObject*)>& callback) {
        for (auto iter = children_.begin(); iter != children_.end(); ) {
            if (callback(*iter)) {
                GridObject* gameObject = *iter;
                unsubscribeDestructionListener(gameObject);
                iter = children_.erase(iter);
                gameObject->setGrid(nullptr);
            } else
                ++iter;
        }
    }

    void TileMap::removeAllChildren() {
        removeChildIf([](GridObject*) {
            return true;
        });
    }

    void TileMap::moveChild(GridObject* child, const Index& index) {
        if (hasChild(child) && isIndexValid(index))
            child->getTransform().setPosition(getTile(index).getWorldCentre());
    }

    void TileMap::moveChild(GridObject* child, const Tile &tile) {
        moveChild(child, tile.getIndex());
    }

    Vector2u TileMap::getTileSize() const {
        return tileSize_;
    }

    void TileMap::forEachTile(const Callback<const Tile&>& callback) const {
        std::for_each(tiledMap_.begin(), tiledMap_.end(), [&callback](auto& row) {
            std::for_each(row.begin(), row.end(), callback);
        });
    }

    void TileMap::forEachTile_(const Callback<Tile&> &callback) {
        std::for_each(tiledMap_.begin(), tiledMap_.end(), [&callback](auto& row) {
            std::for_each(row.begin(), row.end(), callback);
        });
    }

    void TileMap::forEachTileWithId(char id, const Callback<const Tile&>& callback) const {
        forEachTile([&callback, id](const Tile& tile) {
            if (tile.getId() == id)
                callback(tile);
        });
    }

    void TileMap::forEachTileExcept(char id, const Callback<const Tile&>& callback) const {
        forEachTile([&callback, id](const Tile& tile) {
            if (tile.getId() != id)
                callback(tile);
        });
    }

    void TileMap::forEachTileInRange(Index startPos, Index endPos, const Callback<const Tile&>& callback) const {
        if (isIndexValid(startPos) && isIndexValid(endPos)) {
            std::for_each(tiledMap_[startPos.row].begin() + startPos.colm,
                tiledMap_[startPos.row].begin() + endPos.colm,
                [&callback](const Tile& tile) {
                    callback(tile);
            });
        }
    }

    const Tile& TileMap::getTileAbove(const Index &index) const {
        return getTile(Index{index.row - 1, index.colm});
    }

    const Tile& TileMap::getTileBelow(const Index &index) const {
        return getTile(Index{index.row + 1, index.colm});
    }

    const Tile& TileMap::getTileLeftOf(const Index &index) const {
        return getTile(Index{index.row, index.colm - 1});
    }

    const Tile& TileMap::getTileRightOf(const Index &index) const {
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

    const Tile& TileMap::getTileOccupiedByChild(const GridObject* child) const {
        if (child && hasChild(child))
            return getTile(child->getTransform().getPosition());
        else
            return invalidTile_;
    }

    bool TileMap::isTileOccupied(const Tile &tile) const {
        return isTileOccupied(tile.getIndex());
    }

    bool TileMap::isTileOccupied(const Index &index) const {
        return std::any_of(children_.begin(), children_.end(), [this, &index] (GridObject* child) {
            return isInTile(child, tiledMap_[index.row][index.colm]);
        });
    }

    void TileMap::onRenderChange(const Property &property) {
        if (property.getName() == "visible") {
            auto visible = property.getValue<bool>();
            forEachTile_([visible](Tile& tile) {
                tile.setVisible(visible);
            });

            if (visible)
                backgroundTile_.setFillColour(renderer_.getGridLineColour());
            else
                backgroundTile_.setFillColour(ime::Colour::Transparent);
        } else if (property.getName() == "tileColour") {
            forEachTile_([&property](Tile& tile) {
                if (!tile.isCollidable())
                    tile.setFillColour(property.getValue<Colour>());
            });
        } else if (property.getName() == "collidableTileColour") {
            forEachTile_([&property](Tile& tile) {
                if (tile.isCollidable())
                    tile.setFillColour(property.getValue<Colour>());
            });
        } else if (property.getName() == "gridLineColour")
            backgroundTile_.setFillColour(property.getValue<Colour>());
    }

    void TileMap::unsubscribeDestructionListener(GridObject *child) {
        child->removeEventListener(destructionIds_[child->getObjectId()]);
        destructionIds_.erase(child->getObjectId());
    }

    TileMap::~TileMap() {
        removeAllChildren();
    }
}
