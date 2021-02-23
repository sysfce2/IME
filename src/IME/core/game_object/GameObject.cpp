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

#include "IME/core/game_object/GameObject.h"
#include "IME/core/scene/Scene.h"
#include "IME/core/physics/rigid_body/Body.h"
#include "IME/core/physics/World.h"

unsigned int ime::GameObject::prevEntityId_{0};

namespace ime {
    GameObject::GameObject(Scene& scene, Type type) :
        scene_{scene},
        type_{type},
        id_{prevEntityId_++},
        state_{-1},
        isVulnerable_{true},
        isActive_{true},
        isCollidable_{false},
        direction_{Direction::Unknown}
    {
        initTransformEvents();

        // Synchronize the game object with its rigid body
        postStepId_ = scene_.get().on_("postStep", Callback<>([this] {
            if (body_) {
                transform_.setPosition(body_->getPosition());
                transform_.setRotation(body_->getRotation());
            }
        }));
    }

    GameObject::GameObject(const GameObject &other) :
        scene_{other.scene_},
        type_{other.type_},
        id_{prevEntityId_++},
        state_{other.state_},
        name_{other.name_},
        isVulnerable_{other.isVulnerable_},
        isActive_{other.isActive_},
        isCollidable_{other.isCollidable_},
        direction_{other.direction_},
        eventEmitter_{other.eventEmitter_},
        transform_{other.transform_}
        //body_{other.body_->clone()}
    {
        initTransformEvents();
    }

    GameObject &GameObject::operator=(const GameObject &other) {
        if (this != &other) { // Copy swap not applicable - class is polymorphic
            scene_ = other.scene_;
            type_ = other.type_;
            state_ = other.state_;
            name_ = other.name_;
            isVulnerable_ = other.isVulnerable_;
            isActive_ = other.isActive_;
            isCollidable_ = other.isCollidable_;
            direction_ = other.direction_;
            transform_ = other.transform_;
            eventEmitter_ = other.eventEmitter_;
            //body_ = other.body_->clone();
            initTransformEvents();
        }

        return *this;
    }

    void GameObject::setState(int state) {
        if (state_ == state)
            return;
        state_ = state;
        dispatchEvent("stateChange", sprite_);
    }

    int GameObject::getState() const {
        return state_;
    }

    void GameObject::setName(const std::string &name) {
        if (name_ == name)
            return;

        name_ = name;
        dispatchEvent("nameChange", name_);
    }

    const std::string &GameObject::getName() const {
        return name_;
    }

    void GameObject::setDirection(Direction dir) {
        if (direction_ != dir) {
            direction_ = dir;
            dispatchEvent("directionChange", direction_);
        }
    }

    Direction GameObject::getDirection() const {
        return direction_;
    }

    void GameObject::setActive(bool isActive) {
        if (isActive_ == isActive || (isActive_ && !isVulnerable_))
            return;
        isActive_ = isActive;

        if (body_)
            body_->setEnabled(isActive_);
        dispatchEvent("statusChange", isActive_);
    }

    void GameObject::setVulnerable(bool isVulnerable) {
        if (isVulnerable_ != isVulnerable) {
            isVulnerable_ = isVulnerable;
            if (isVulnerable_)
                dispatchEvent("vulnerable");
            else
                dispatchEvent("inVulnerable");
        }
    }

    void GameObject::setCollidable(bool isCollidable) {
        if (isCollidable_ != isCollidable) {
            isCollidable_ = isCollidable;
            if (body_) {
                body_->forEachFixture([isCollidable](Fixture::sharedPtr fixture) {
                    fixture->setCollidable(isCollidable);
                });
            }

            if (isCollidable_)
                dispatchEvent("collisionEnable");
            else
                dispatchEvent("collisionDisable");
        }
    }

    bool GameObject::isActive() const {
        return isActive_;
    }

    bool GameObject::isCollidable() const {
        return isCollidable_;
    }

    void GameObject::setType(GameObject::Type type) {
        type_ = type;
    }

    GameObject::Type GameObject::getType() const {
        return type_;
    }

    bool GameObject::isVulnerable() const {
        return isVulnerable_;
    }

    unsigned int GameObject::getObjectId() const {
        return id_;
    }

    void GameObject::attachRigidBody(Body::sharedPtr body) {
        IME_ASSERT(body, "Invalid rigid body, cannot attach a nullptr to an entity");
        IME_ASSERT(!body_, "Entity already has a rigid body attached, remove it first before attaching another one");
        body_ = body;
        resetSpriteOrigin();
        body_->setPosition(transform_.getPosition());
        transform_.setRotation(body->getRotation());
    }

    Body::sharedPtr &GameObject::getRigidBody() {
        return body_;
    }

    const Body::sharedPtr &GameObject::getRigidBody() const {
        return body_;
    }

    void GameObject::removeRigidBody() {
        if (body_) {
            body_->getWorld()->destroyBody(body_);
            body_.reset();
        }
    }

    bool GameObject::hasRigidBody() const {
        return body_ != nullptr;
    }

    Transform &GameObject::getTransform() {
        return transform_;
    }

    const Transform &GameObject::getTransform() const {
        return transform_;
    }

    void GameObject::resetSpriteOrigin() {
        transform_.setOrigin(sprite_.getLocalBounds().width / 2.0f, sprite_.getLocalBounds().height / 2.0f);
    }

    Sprite &GameObject::getSprite() {
        return sprite_;
    }

    const Sprite &GameObject::getSprite() const {
        return sprite_;
    }

    void GameObject::update(Time deltaTime) {
        sprite_.updateAnimation(deltaTime);
    }

    bool GameObject::unsubscribe(const std::string &event, int id) {
        return eventEmitter_.removeEventListener(event, id);
    }

    bool GameObject::operator==(const GameObject &rhs) {
        return id_ == rhs.id_;
    }

    bool GameObject::operator!=(const GameObject &rhs) {
        return !(*this == rhs);
    }

    void GameObject::initTransformEvents() {
        transform_.onPropertyChange([this](std::string property, std::any) {
            if (property == "position") {
                sprite_.setPosition(transform_.getPosition());
                dispatchEvent("positionChange", transform_.getPosition());
                dispatchEvent("positionChange", transform_.getPosition().x, transform_.getPosition().y);
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

    GameObject::~GameObject() {
        scene_.get().unsubscribe_("postStep", postStepId_);
    }
}