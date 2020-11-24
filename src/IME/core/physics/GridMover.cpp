////////////////////////////////////////////////////////////////////////////////
// IME - Infinite Motion Engine
//
// Copyright (c) 2020 Kwena Mashamaite (kmash.ime@gmail.com)
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

#include "IME/core/physics/GridMover.h"
#include "IME/core/entity/IMovable.h"
#include <cassert>

namespace IME {
    GridMover::GridMover(TileMap &tileMap, std::shared_ptr<Entity> target) :
        tileMap_(tileMap),
        target_(target),
        targetDirection_(Direction::None),
        targetTile_{tileMap.getTileSize(), {0, 0}},
        prevTile_({tileMap.getTileSize(), {}}),
        reachedTarget_(false)
    {
        if (target) {
            assert(std::dynamic_pointer_cast<IMovable>(target) && "Provided entity is not movable (derived from IMovable)");
            assert(tileMap_.hasChild(target) && "Target must already be in the grid before instantiating a grid mover");
            targetTile_ = tileMap.getTile(target->getPosition());
        }
    }

    void GridMover::setTarget(GridMover::EntityPtr target) {
        if (target_ == target)
            return;
        else if (target) {
            assert(std::dynamic_pointer_cast<IMovable>(target) && "Provided entity is not movable (derived from IMovable)");
            assert(tileMap_.hasChild(target) && "Target must already be in the grid before calling setTarget()");
            if (target_)
                teleportTargetToDestination();
            targetTile_ = tileMap_.getTile(target->getPosition());
            target_ = std::move(target);
        }
    }

    GridMover::EntityPtr GridMover::getTarget() const {
        return target_;
    }

    TileMap &GridMover::getGrid() {
        return tileMap_;
    }

    bool GridMover::isTargetMoving() const {
        auto target = std::dynamic_pointer_cast<IMovable>(target_);
        if (target)
            return target->isMoving();
        return false;
    }

    bool GridMover::requestDirectionChange(Direction newDir) {
        auto movableObj = std::dynamic_pointer_cast<IMovable>(target_);
        if (movableObj && !movableObj->isMoving()) {
            switch (newDir) {
                case Direction::None:
                    target_->setDirection(Direction::None);
                    break;
                case Direction::Left:
                    target_->setDirection(Direction::Left);
                    break;
                case Direction::Right:
                    target_->setDirection(Direction::Right);
                    break;
                case Direction::Up:
                    target_->setDirection(Direction::Up);
                    break;
                case Direction::Down:
                    target_->setDirection(Direction::Down);
                    break;
            }
            targetDirection_ = target_->getDirection();
            return true;
        }
        return false;
    }

    void GridMover::update(float deltaTime) {
        auto movable = std::dynamic_pointer_cast<IMovable>(target_);
        if (!movable)
            return;
        if (!movable->isMoving() && targetDirection_ != Direction::None) {
            prevTile_ = tileMap_.getTile(target_->getPosition());
            switch (targetDirection_) {
                case Direction::Left:
                    targetTile_ = tileMap_.getTileLeftOf(prevTile_);
                    break;
                case Direction::Right:
                    targetTile_ = tileMap_.getTileRightOf(prevTile_);
                    break;
                case Direction::Up:
                    targetTile_ = tileMap_.getTileAbove(prevTile_);
                    break;
                case Direction::Down:
                    targetTile_ = tileMap_.getTileBelow(prevTile_);
                    break;
            }

            //A tile outside the grid bounds has the index {-1, -1}
            if (targetTile_.getIndex().row < 0 || targetTile_.getIndex().colm < 0) {
                targetTile_ = prevTile_;
                targetDirection_ = Direction::None;
                eventEmitter_.emit("gridCollision");
                return;
            }

            if (targetTile_.isSolid()) {
                auto tile = targetTile_;
                targetTile_ = prevTile_;
                targetDirection_ = Direction::None;
                eventEmitter_.emit("tileCollision", target_, tile);
                return;
            }
            movable->move();
        } else if (movable->isMoving()) {
            auto velocity = movable->getSpeed() * deltaTime;
            if (targetDirection_ == Direction::Left || targetDirection_ == Direction::Right) {
                auto horizontalDistToTarget = std::abs(targetTile_.getPosition().x - target_->getPosition().x);
                if (velocity >= horizontalDistToTarget)
                    snap();
            }

            if (targetDirection_ == Direction::Up || targetDirection_ == Direction::Down) {
                auto verticalDistToTarget = std::abs(targetTile_.getPosition().y - target_->getPosition().y);
                if (velocity >= verticalDistToTarget)
                    snap();
            }

            if (reachedTarget_) {
                reachedTarget_ = false;
                tileMap_.forEachChildInTile(targetTile_, [this](EntityPtr entity) {
                    if (entity->isCollidable()) {
                        switch (entity->getType()) {
                            case Entity::Type::Unknown:
                                break;
                            case Entity::Type::Player:
                                eventEmitter_.emit("playerCollision", target_, entity);
                                break;
                            case Entity::Type::Enemy:
                                eventEmitter_.emit("enemyCollision", target_, entity);
                                break;
                            case Entity::Type::Collectable:
                                eventEmitter_.emit("collectableCollision", target_, entity);
                                break;
                            case Entity::Type::Obstacle:
                                targetTile_ = prevTile_;
                                targetDirection_ = Direction::None;
                                tileMap_.moveChild(target_, prevTile_.getIndex());
                                eventEmitter_.emit("obstacleCollision", target_, entity);
                                break;
                        }
                    }
                });
                eventEmitter_.emit("destinationReached", targetTile_.getPosition().x, targetTile_.getPosition().y);
            }
        }
    }

    void GridMover::teleportTargetToDestination() {
        snap();
    }

    void GridMover::snap() {
        if (target_ && target_->getPosition() != targetTile_.getPosition()) {
            tileMap_.moveChild(target_, targetTile_);
            std::dynamic_pointer_cast<IMovable>(target_)->stop();
            targetDirection_ = Direction::None;
            reachedTarget_ = true;
        }
    }

    int GridMover::onTileCollision(Callback<Graphics::Tile &> callback) {
        return eventEmitter_.addEventListener("tileCollision", std::move(callback));
    }

    int GridMover::onTargetChanged(Callback<GridMover::EntityPtr> callback) {
        return eventEmitter_.addEventListener("targetChange", std::move(callback));
    }

    int GridMover::onGridBorderCollision(Callback<> callback) {
        return eventEmitter_.addEventListener("gridCollision", std::move(callback));
    }

    int GridMover::onDestinationReached(Callback<float, float> callback) {
        return eventEmitter_.addEventListener("destinationReached", std::move(callback));
    }

    int GridMover::onObstacleCollision(Callback<EntityPtr, EntityPtr> callback) {
        return eventEmitter_.addEventListener("obstacleCollision", std::move(callback));
    }

    int GridMover::onCollectableCollision(Callback<EntityPtr, EntityPtr> callback) {
        return eventEmitter_.addEventListener("collectableCollision", std::move(callback));
    }

    int GridMover::onEnemyCollision(Callback<EntityPtr, EntityPtr> callback) {
        return eventEmitter_.addEventListener("enemyCollision", std::move(callback));
    }

    int GridMover::onPlayerCollision(Callback<GridMover::EntityPtr, GridMover::EntityPtr> callback) {
        return eventEmitter_.addEventListener("playerCollision", std::move(callback));
    }
}
