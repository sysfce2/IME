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

#include "IME/core/entity/Entity.h"

std::size_t ime::Entity::prevEntityId_{0};

namespace ime {
    Entity::Entity() :
        Entity({0, 0})
    {}

    Entity::Entity(const Vector2u &boundingBoxSize, Type type) :
        type_{type},
        id_{prevEntityId_++},
        state_{-1},
        boundingRect_{boundingBoxSize},
        isVulnerable_{true},
        isActive_{true},
        isCollidable_{false},
        direction_{Direction::Unknown}
    {
        initTransformEvents();
    }

    Entity::Entity(const Entity &other) :
        type_{other.type_},
        id_{prevEntityId_++},
        state_{other.state_},
        name_{other.name_},
        boundingRect_{other.boundingRect_},
        isVulnerable_{other.isVulnerable_},
        isActive_{other.isActive_},
        isCollidable_{other.isCollidable_},
        direction_{other.direction_},
        eventEmitter_{other.eventEmitter_},
        transform_{other.transform_}
    {
        initTransformEvents();
    }

    Entity &Entity::operator=(const Entity &other) {
        if (this != &other) { // Copy swap not applicable - class is polymorphic
            type_ = other.type_;
            state_ = other.state_;
            name_ = other.name_;
            boundingRect_ = other.boundingRect_;
            isVulnerable_ = other.isVulnerable_;
            isActive_ = other.isActive_;
            isCollidable_ = other.isCollidable_;
            direction_ = other.direction_;
            transform_ = other.transform_;
            eventEmitter_ = other.eventEmitter_;
            initTransformEvents();
        }

        return *this;
    }

    void Entity::setState(int state) {
        if (state_ == state)
            return;
        state_ = state;
        dispatchEvent("stateChange", sprite_);
    }

    int Entity::getState() const {
        return state_;
    }

    void Entity::setName(const std::string &name) {
        if (name_ == name)
            return;

        name_ = name;
        dispatchEvent("nameChange", name_);
    }

    const std::string &Entity::getName() const {
        return name_;
    }

    void Entity::setDirection(Direction dir) {
        if (direction_ != dir) {
            direction_ = dir;
            dispatchEvent("directionChange", direction_);
        }
    }

    Direction Entity::getDirection() const {
        return direction_;
    }

    void Entity::setSize(Vector2u size) {
        if (boundingRect_ != size) {
            boundingRect_ = size;
            eventEmitter_.emit("sizeChange", boundingRect_);
        }
    }

    Vector2u Entity::getSize() const {
        return boundingRect_;
    }

    void Entity::setActive(bool isActive) {
        if (isActive_ == isActive || (isActive_ && !isVulnerable_))
            return;
        isActive_ = isActive;

        dispatchEvent("statusChange", isActive_);
    }

    void Entity::setVulnerable(bool isVulnerable) {
        if (isVulnerable_ != isVulnerable) {
            isVulnerable_ = isVulnerable;
            if (isVulnerable_)
                dispatchEvent("vulnerable");
            else
                dispatchEvent("inVulnerable");
        }
    }

    void Entity::setCollidable(bool isCollidable) {
        if (isCollidable_ != isCollidable) {
            isCollidable_ = isCollidable;
            if (isCollidable_)
                dispatchEvent("collisionEnable");
            else
                dispatchEvent("collisionDisable");
        }
    }

    bool Entity::isActive() const {
        return isActive_;
    }

    bool Entity::isCollidable() const {
        return isCollidable_;
    }

    void Entity::setType(Entity::Type type) {
        type_ = type;
    }

    Entity::Type Entity::getType() const {
        return type_;
    }

    bool Entity::isVulnerable() const {
        return isVulnerable_;
    }

    std::size_t Entity::getObjectId() const {
        return id_;
    }

    Transform &Entity::getTransform() {
        return transform_;
    }

    Sprite &Entity::getSprite() {
        return sprite_;
    }

    bool Entity::unsubscribe(const std::string &event, int id) {
        return eventEmitter_.removeEventListener(event, id);
    }

    bool Entity::operator==(const Entity &rhs) {
        return id_ == rhs.id_;
    }

    bool Entity::operator!=(const Entity &rhs) {
        return !(*this == rhs);
    }

    void Entity::initTransformEvents() {
        transform_.onPropertyChange([this](std::string property, std::any) {
            if (property == "position") {
                sprite_.setPosition(transform_.getPosition());
                dispatchEvent("positionChange", transform_.getPosition());
            } else if (property == "origin") {
                sprite_.setOrigin(transform_.getOrigin());
                dispatchEvent("originChange", transform_.getOrigin());
            } else if (property == "scale") {
                sprite_.setScale(transform_.getScale());
                dispatchEvent("scaleChange", transform_.getScale());
            } else if (property == "rotation") {
                sprite_.setRotation(transform_.getRotation());
                dispatchEvent("rotationChange", transform_.getRotation());
            }
        });
    }
}
