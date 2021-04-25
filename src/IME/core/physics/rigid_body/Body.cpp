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

#include "IME/core/physics/rigid_body/Body.h"
#include "IME/core/game_object/GameObject.h"
#include "IME/core/physics/World.h"
#include "IME/utility/Helpers.h"
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>

namespace ime {
    Body::Body(const World::Ptr& world, Type bodyType) {
        IME_ASSERT(world, "Cannot construct body from a nullptr")
        world_ = world;

        auto b2Definition = std::make_unique<b2BodyDef>();
        b2Definition->type = static_cast<b2BodyType>(bodyType);
        b2Definition->userData.pointer = getObjectId();

        // b2Body has a private destructor
        auto b2BodyDeleter = [](b2Body* body) {
            body->GetWorld()->DestroyBody(body);
        };

        body_ = std::unique_ptr<b2Body, std::function<void(b2Body*)>>(
            world_->getInternalWorld()->CreateBody(b2Definition.get()), std::move(b2BodyDeleter));
    }

    Body::Body(Body&&) noexcept = default;
    Body &Body::operator=(Body &&) noexcept = default;

    Body::Ptr Body::copy() const {
        auto body = world_->createBody(getType());

        // b2Body does not have any public constructors, so we use setters to simulate a copy
        body->setPosition(getPosition());
        body->setRotation(getRotation());
        body->setFixedRotation(isFixedRotation());
        body->setLinearVelocity(getLinearVelocity());
        body->setAngularVelocity(getAngularVelocity());
        body->setLinearDamping(getLinearDamping());
        body->setAngularDamping(getAngularDamping());
        body->setFastBody(isFastBody());
        body->setEnabled(isEnabled());
        body->setAwake(isAwake());
        body->setGravityScale(getGravityScale());
        body->setSleepingAllowed(isSleepingAllowed());
        body->setTag(getTag());

        for (const auto& [id, collider] : colliders_) {
            IME_UNUSED(id);
            body->attachCollider(collider->clone());
        }

        return body;
    }

    std::string Body::getClassName() const {
        return "Body";
    }

    void Body::attachCollider(Collider::Ptr collider) {
        IME_ASSERT(collider, "Cannot attach a nullptr to a rigid body")
        IME_ASSERT(!collider->isAttachedToBody(), "The collider is already attached to another rigid body: One body per collider")
        if (!world_->isLocked()) {
            collider->setBody(*this);
            colliders_.insert({collider->getObjectId(), std::move(collider)});
            emit("attachCollider");
        } else {
            IME_PRINT_WARNING("Operation ignored: AttachCollider() called inside a world callback")
        }
    }

    const Collider::Ptr& Body::getColliderById(unsigned int id) {
        if (utility::findIn(colliders_, id))
            return colliders_.at(static_cast<int>(id));
        return null_ptr;
    }

    void Body::removeColliderWithId(unsigned int id) {
        if (!world_->isLocked()) {
            if (colliders_.find(static_cast<int>(id)) != colliders_.end()) {
                colliders_.erase(static_cast<int>(id));
                emit("removeCollider");
            }
        } else {
            IME_PRINT_WARNING("Operation ignored: removeColliderWithId() called inside a world callback")
        }
    }

    void Body::setPosition(Vector2f position) {
        body_->SetTransform(
            {utility::pixelsToMetres(position.x), utility::pixelsToMetres(position.y)},
            body_->GetAngle());
        emitChange(Property{"position", position});
    }

    Vector2f Body::getPosition() const {
        return utility::metresToPixels({body_->GetPosition().x, body_->GetPosition().y});
    }

    void Body::setRotation(float angle) {
        if (isFixedRotation())
            return;

        body_->SetTransform(body_->GetTransform().p, utility::degToRad(angle));
        emitChange(Property{"rotation", angle});
    }

    float Body::getRotation() const {
        return utility::radToDeg(body_->GetAngle());
    }

    Vector2f Body::getWorldCenter() const {
        return utility::metresToPixels({body_->GetWorldCenter().x, body_->GetWorldCenter().y});
    }

    Vector2f Body::getLocalCenter() const {
        return {utility::metresToPixels(body_->GetLocalCenter().x),
                utility::metresToPixels(body_->GetLocalCenter().y)};
    }

    void Body::setLinearVelocity(Vector2f velocity) {
        body_->SetLinearVelocity({utility::pixelsToMetres(velocity.x), utility::pixelsToMetres(velocity.y)});
        emitChange(Property{"linearVelocity", velocity});
    }

    Vector2f Body::getLinearVelocity() const {
        return {utility::metresToPixels(body_->GetLinearVelocity().x), utility::metresToPixels(body_->GetLinearVelocity().y)};
    }

    void Body::setAngularVelocity(float degrees) {
        body_->SetAngularVelocity(utility::degToRad(degrees));
        emitChange(Property{"angularVelocity",  degrees});
    }

    float Body::getAngularVelocity() const {
        return utility::radToDeg(body_->GetAngularVelocity());
    }

    void Body::applyForce(Vector2f force, Vector2f point, bool wake) {
        body_->ApplyForce({force.x, force.y},
            {utility::pixelsToMetres(point.x), utility::pixelsToMetres(point.y)},
            wake);
    }

    void Body::applyForceToCenter(Vector2f force, bool wake) {
        body_->ApplyForceToCenter({force.x, force.y}, wake);
    }

    void Body::applyTorque(float torque, bool wake) {
        body_->ApplyTorque(torque, wake);
    }

    void Body::applyLinearImpulse(Vector2f impulse, Vector2f point, bool wake) {
        body_->ApplyLinearImpulse({impulse.x, impulse.y},
            {utility::pixelsToMetres(point.x), utility::pixelsToMetres(point.y)},
            wake);
    }

    void Body::applyLinearImpulseToCenter(Vector2f impulse, bool wake) {
        body_->ApplyLinearImpulseToCenter({impulse.x, impulse.y}, wake);
    }

    void Body::applyAngularImpulse(float impulse, bool wake) {
        body_->ApplyAngularImpulse(impulse, wake);
    }

    float Body::getMass() const {
        return body_->GetMass();
    }

    float Body::getInertia() const {
        return body_->GetInertia();
    }

    Vector2f Body::getWorldPoint(Vector2f localPoint) const {
        auto worldPoint = body_->GetWorldPoint({utility::pixelsToMetres(localPoint.x),
            utility::pixelsToMetres(localPoint.y)});

        return {utility::metresToPixels(worldPoint.x),
                utility::metresToPixels(worldPoint.y)};
    }

    Vector2f Body::getWorldRotation(Vector2f localVector) const {
        auto worldVector = body_->GetWorldVector({utility::pixelsToMetres(localVector.x),
            utility::pixelsToMetres(localVector.y)});

        return {utility::radToDeg(worldVector.x),
                utility::radToDeg(worldVector.y)};
    }

    Vector2f Body::getLocalPoint(Vector2f worldPoint) const {
        auto localPoint = body_->GetLocalPoint({utility::pixelsToMetres(worldPoint.x),
            utility::pixelsToMetres(worldPoint.y)});

        return {utility::metresToPixels(localPoint.x),
                utility::metresToPixels(localPoint.y)};
    }

    Vector2f Body::getLocalRotation(Vector2f worldVector) const {
        auto localVector = body_->GetLocalVector({utility::pixelsToMetres(worldVector.x),
            utility::pixelsToMetres(worldVector.y)});

        return {utility::radToDeg(localVector.x),
                utility::radToDeg(localVector.y)};
    }

    Vector2f Body::getLinearVelocityFromWorldPoint(Vector2f worldPoint) const {
        auto velocity = body_->GetLinearVelocityFromLocalPoint({utility::pixelsToMetres(worldPoint.x),
            utility::pixelsToMetres(worldPoint.y)});

        return {utility::metresToPixels(velocity.x), utility::metresToPixels(velocity.y)};
    }

    Vector2f Body::getLinearVelocityFromLocalPoint(Vector2f localPoint) const {
        auto velocity = body_->GetLinearVelocityFromLocalPoint({utility::pixelsToMetres(localPoint.x),
            utility::pixelsToMetres(localPoint.y)});

        return {utility::metresToPixels(velocity.x), utility::metresToPixels(velocity.y)};
    }

    void Body::setLinearDamping(float damping) {
        body_->SetLinearDamping(damping);
        emitChange(Property{"linearDamping", damping});
    }

    float Body::getLinearDamping() const {
        return body_->GetLinearDamping();
    }

    void Body::setAngularDamping(float damping) {
        body_->SetAngularDamping(damping);
        emitChange(Property{"angularDamping", damping});
    }

    float Body::getAngularDamping() const {
        return body_->GetAngularDamping();
    }

    void Body::setGravityScale(float scale) {
        body_->SetGravityScale(scale);
        emitChange(Property{"gravityScale", scale});
    }

    float Body::getGravityScale() const {
        return body_->GetGravityScale();
    }

    void Body::setType(Type type) {
        if (world_->isLocked()) {
            IME_PRINT_WARNING("Operation ignored: setType() called inside a world callback")
            return;
        }

        body_->SetType(static_cast<b2BodyType>(type));
        emitChange(Property{"type", type});
    }

    Body::Type Body::getType() const {
        return static_cast<Type>(body_->GetType());
    }

    void Body::setFastBody(bool fast) {
        body_->SetBullet(fast);
        emitChange(Property{"fastBody", fast});
    }

    bool Body::isFastBody() const {
        return body_->IsBullet();
    }

    void Body::setSleepingAllowed(bool sleeps) {
        body_->SetSleepingAllowed(sleeps);
        emitChange(Property{"sleepingAllowed", sleeps});
    }

    bool Body::isSleepingAllowed() const {
        return body_->IsSleepingAllowed();
    }

    void Body::setAwake(bool awake) {
        body_->SetAwake(awake);
        emitChange(Property{"awake", awake});
    }

    bool Body::isAwake() const {
        return body_->IsAwake();
    }

    void Body::setEnabled(bool enable) {
        if (world_->isLocked()) {
            IME_PRINT_WARNING("Operation ignored: setEnabled() called inside a world callback")
            return;
        }

        body_->SetEnabled(enable);
        emitChange(Property{"enable", enable});
    }

    bool Body::isEnabled() const {
        return body_->IsEnabled();
    }

    void Body::setFixedRotation(bool rotate) {
        body_->SetFixedRotation(rotate);
        emitChange(Property{"fixedRotation", rotate});
    }

    bool Body::isFixedRotation() const {
        return body_->IsFixedRotation();
    }

    void Body::setGameObject(GameObject::Ptr gameObject) {
        gameObject_ = std::move(gameObject);
    }

    const GameObject::Ptr& Body::getGameObject() {
        return gameObject_;
    }

    const GameObject::Ptr& Body::getGameObject() const {
        return gameObject_;
    }

    const Body::WorldPtr& Body::getWorld() {
        return world_;
    }

    const Body::WorldPtr &Body::getWorld() const {
        return world_;
    }

    PropertyContainer &Body::getUserData() {
        return userData_;
    }

    void Body::forEachCollider(const Callback<const Collider::Ptr&>& callback) {
        std::for_each(colliders_.begin(), colliders_.end(), [&callback](auto& pair) {
            callback(pair.second);
        });
    }

    std::size_t Body::getColliderCount() const {
        return colliders_.size();
    }

    std::unique_ptr<b2Body, std::function<void(b2Body*)>>& Body::getInternalBody() {
        return body_;
    }

    const std::unique_ptr<b2Body, std::function<void(b2Body*)>>& Body::getInternalBody() const {
        return body_;
    }

    Body::~Body() {
        emit("destruction");
    }
}