#include "IME/core/tilemap/TileMap.h"
#include "IME/core/tilemap/TileMapParser.h"
#include "IME/graphics/Window.h"
#include "IME/core/managers/ResourceManager.h"

namespace IME {
    TileMap::TileMap(unsigned int tileWidth, unsigned int tileHeight)
        : isGridVisible_(true), isBackgroundDrawable_(true), isTilesDrawable_(true),
          isObjectsDrawable_(true), invalidTile_({0, 0}, {-1, -1})
    {
        invalidTile_.setId('!'); //Any tile returned from a function with this token is invalid
        invalidTile_.setPosition({-99, -99});
        mapPos_ = {0, 0};
        numOfRows_ = numOfColms_ = 0;
        if (tileWidth < 8)
            tileWidth = 8;

        if (tileHeight < 8)
            tileHeight = 8;

        tileSize_ = Dimensions{static_cast<float>(tileWidth), static_cast<float>(tileHeight)};
    }

    void TileMap::setGridVisible(bool isVisible) {
        if (isGridVisible_ == isVisible)
            return;

        forEachTile([=](Graphics::Tile& tile) {
            tile.setBorderVisible(isVisible);
        });

        isGridVisible_ = isVisible;
    }

    void TileMap::toggleGridVisibility() {
        setGridVisible(!isGridVisible_);
    }

    Graphics::Tile& TileMap::getTile(const Position &position) {
        if (auto index = getIndexFromPosition(position); isValidIndex(index))
            return getTile(index);
        return invalidTile_;
    }

    void TileMap::setBackground(const std::string &filename) {
        background_.setTexture(filename);
        if (background_.getSize().width > mapSizeInPixels_.width
            && background_.getSize().height > mapSizeInPixels_.height)
            background_.setTextureRect(0, 0, mapSizeInPixels_.width, mapSizeInPixels_.height);
        else if (background_.getSize().width > mapSizeInPixels_.width)
            background_.setTextureRect(0, 0, mapSizeInPixels_.width, background_.getSize().height);
        else if (background_.getSize().height > mapSizeInPixels_.height)
            background_.setTextureRect(0, 0, background_.getSize().width, mapSizeInPixels_.height);
    }

    void TileMap::scaleBackground(float xOffset, float yOffset) {
        background_.scale(xOffset, yOffset);
        setBackground(background_.getTexture());
    }

    bool TileMap::isValidIndex(const Index &index) const {
        auto [row, colm] = index;
        return !(row > numOfRows_ || row < 0 || colm > numOfColms_ || colm < 0);
    }

    Index TileMap::getIndexFromPosition(const Position &position) const {
        auto row = position.x / (tileSize_.width * position.y) / tileSize_.height;
        auto colm = position.y / (tileSize_.height - position.x) / tileSize_.width;
        return {static_cast<int>(row), static_cast<int>(colm)};
    }

    Position TileMap::getCoordinatesFromIndex(Index index) const {
        auto x = (mapSizeInPixels_.width - mapSizeInPixels_.height) * tileSize_.width / 2.0f;
        auto y = (mapSizeInPixels_.width + mapSizeInPixels_.height) * tileSize_.height / 2.0f;
        return {x, y};
    }

    void TileMap::loadFromFile(const std::string &filename, const char& separator) {
        mapData_ = TileMapParser().parse(filename, separator);
        computeDimensions();
        createTiledMap();
    }

    void TileMap::loadFromVector(const Map &map) {
        mapData_ = map;
        computeDimensions();
        createTiledMap();
    }

    void TileMap::computeDimensions() {
        numOfRows_ = mapData_.size();
        numOfColms_ = mapData_[0].size();
        mapSizeInPixels_ = {numOfColms_ * tileSize_.height, numOfRows_ * tileSize_.width};
    }

    void TileMap::setPosition(int x, int y) {
        mapPos_.x = x;
        mapPos_.y = y;

        for (auto i = 0u; i < tiledMap_.size(); i++) {
            for (auto j = 0u; j < mapData_[i].size(); j++) {
                if (i == 0 && j == 0)
                    tiledMap_[i][j].setPosition(mapPos_);
                else if (j == 0)
                    tiledMap_[i][j].setPosition( {mapPos_.x, tiledMap_[i - 1][j].getPosition().y + tileSize_.height});
                else
                    tiledMap_[i][j].setPosition( {tiledMap_[i][j - 1].getPosition().x + tileSize_.width, tiledMap_[i][j - 1].getPosition().y});
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

    Position TileMap::getPosition() const {
        return mapPos_;
    }

    void TileMap::createTiledMap() {
        for (auto i = 0u; i < mapData_.size(); i++) {
            auto row = std::vector<Graphics::Tile>{};
            for (auto j = 0u; j < mapData_[i].size(); j++) {
                auto tile = Graphics::Tile(tileSize_, {-99, -99});
                if (i == 0 && j == 0)
                    tile.setPosition(mapPos_);
                else if (j == 0)
                    tile.setPosition( {mapPos_.x, tiledMap_[i - 1][j].getPosition().y + tileSize_.height});
                else
                    tile.setPosition( {row[j - 1].getPosition().x + tileSize_.width, row[j - 1].getPosition().y});

                tile.setId(mapData_[i][j]);
                row.emplace_back(tile);
            }
            tiledMap_.push_back(row);
        }
    }

    void TileMap::draw(Graphics::Window &renderTarget) {
        //Draw background (first layer)
        if (isBackgroundDrawable_) {
            background_.setPosition(mapPos_.x, mapPos_.y);
            renderTarget.draw(background_);
        }

        //Draw tiles (second layer)
        if (isTilesDrawable_) {
            forEachTile([&](Graphics::Tile &tile) {
                renderTarget.draw(tile);
            });
        }

        //Draw objects (third layer)
        if (isObjectsDrawable_) {
            std::for_each(objects_.begin(), objects_.end(), [&](auto &sprite) {
                renderTarget.draw(sprite);
            });
        }
    }

    void TileMap::setTile(Index index, Graphics::Tile &&tile) {
        if (isValidIndex(index))
            tiledMap_[index.row][index.colm] = std::move(tile);
    }

    void TileMap::setCollideableByIndex(const Index &index, bool isCollideable) {
        if (isValidIndex(index))
            tiledMap_[index.row][index.colm].setCollideable(isCollideable);
    }

    void TileMap::setCollideableByIndex(const std::initializer_list<Index> &locations, bool isCollideable) {
        std::for_each(locations.begin(), locations.end(), [=](const Index& index) {
            setCollideableByIndex(index, isCollideable);
        });
    }

    void TileMap::setCollideableByIndex(Index startPos, Index endPos, bool isCollideable) {
        if (isValidIndex(startPos) && isValidIndex(endPos)){
            for (auto i = startPos.colm; i < endPos.colm; i++)
                setCollideableByIndex({startPos.row, i}, isCollideable);
        }
    }

    void TileMap::setCollideableById(const char &id, bool isCollideable) {
        forEachTile([=](Graphics::Tile& tile) {
            if (tile.getId() == id)
                tile.setCollideable(isCollideable);
        });
    }

    void TileMap::setCollideableByExclusion(const char &id, bool isCollideable) {
        forEachTile([=](Graphics::Tile& tile) {
            if (tile.getId() != id)
                tile.setCollideable(isCollideable);
        });
    }

    Graphics::Tile& TileMap::getTile(const Index &index) {
        if (isValidIndex(index))
            return tiledMap_[index.row][index.colm];
        return invalidTile_;
    }

    void TileMap::hide(const std::string &layer) {
        if (layer == "background")
            isBackgroundDrawable_ = false;
        else if (layer == "tiles")
            isTilesDrawable_ = false;
        else if (layer == "objects")
            isObjectsDrawable_ = false;
    }

    void TileMap::show(const std::string &layer) {
        if (layer == "background")
            isBackgroundDrawable_ = true;
        else if (layer == "tiles")
            isTilesDrawable_ = true;
        else if (layer == "objects")
            isObjectsDrawable_ = true;
    }

    bool TileMap::isHidden(const std::string &layer) const {
        if (layer == "background")
            return isBackgroundDrawable_;
        else if (layer == "tiles")
            return isTilesDrawable_;
        else if (layer == "objects")
            return isObjectsDrawable_;
        return false;
    }

    bool TileMap::isCollideable(const Index &index) const {
        if (!isValidIndex(index))
            return tiledMap_[index.row][index.colm].isCollideable();
        return false;
    }

    bool TileMap::addObject(Index index, Graphics::Sprite &object) {
        if (isValidIndex(index)) {
            auto& targetTile = getTile(index);
            object.setPosition(targetTile.getPosition().x, targetTile.getPosition().y);
            objects_.push_back(object);
            return true;
        }
        return false;
    }

    void TileMap::addTilesetImageData(const char &id, Position startPos, Dimensions size) {
        imagesData_.insert({id, {tileSet_, startPos, size}});
    }

    bool TileMap::isIdLinkedToImage(const char &id) const {
        return imagesData_.find(id) != imagesData_.end();
    }

    void TileMap::applyImages() {
        forEachTile([this](Graphics::Tile& tile) {
            if (auto tileId = tile.getId(); isIdLinkedToImage(tileId)) {
                auto [tileset, startPos, size] = imagesData_.at(tileId);
                tile.setTexture(tileset);
                tile.setTextureRect(startPos, size);
            }
        });
    }

    void TileMap::forEachTile(Callback<Graphics::Tile&> callback) {
        std::for_each(tiledMap_.begin(), tiledMap_.end(), [&](auto& row) {
            std::for_each(row.begin(), row.end(), callback);
        });
    }


    void TileMap::forEachTile(const char &id, Callback<Graphics::Tile&> callback) {
        forEachTile([&](Graphics::Tile& tile) {
            if (tile.getId() == id)
                callback(tile);
        });
    }

    void TileMap::forEachTile(Index startPos, Index endPos, Callback<Graphics::Tile&> callback) {
        if (isValidIndex(startPos) && isValidIndex(endPos)) {
            std::for_each(tiledMap_[startPos.row].begin() + startPos.colm,
                tiledMap_[startPos.row].begin() + endPos.colm,
                [&](Graphics::Tile& tile) { callback(tile);}
            );
        }
    }

    void TileMap::forEachObject(Callback<Graphics::Sprite &> callback) {
        std::for_each(objects_.begin(), objects_.end(), callback);
    }

    Graphics::Tile &TileMap::getTileAbove(const Index &index) {
        return getTile(Index{index.row, index.colm - 1});
    }

    Graphics::Tile& TileMap::getTileBelow(const Index &index) {
        return getTile(Index{index.row, index.colm + 1});
    }

    Graphics::Tile& TileMap::getTileLeftOf(const Index &index) {
        return getTile(Index{index.row - 1, index.colm});
    }

    Graphics::Tile& TileMap::getTileRightOf(const Index &index) {
        return getTile(Index{index.row + 1, index.colm});
    }

    int TileMap::onTileMapCollision(Callback<Graphics::Sprite &, Graphics::Tile &> callback) {
        return eventEmitter_.addEventListener("worldCollision", std::move(callback));
    }

    bool TileMap::isGridVisible() const {
        return isGridVisible_;
    }

    Dimensions TileMap::getSize() const {
        return mapSizeInPixels_;
    }

    Dimensions TileMap::getSizeInTiles() const {
        return {static_cast<float>(numOfColms_), static_cast<float>(numOfRows_)};
    }
}
