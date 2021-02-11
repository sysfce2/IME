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

#include "IME/core/physics/rigid_body/joints/DistanceJoint.h"
#include "IME/utility/Helpers.h"
#include "IME/core/physics/rigid_body/Body.h"
#include "IME/core/physics/World.h"
#include <box2d/b2_distance_joint.h>
#include <box2d/b2_world.h>
#include <cmath>

namespace ime {
    //////////////////////////////////////////////////////////////////////////
    // BodyDefinition class stuff
    //////////////////////////////////////////////////////////////////////////

    DistanceJointDefinition::DistanceJointDefinition() :
        bodyALocalAnchorPoint{0.0f, 0.0f},
        bodyBLocalAnchorPoint{0.0f, 0.0f},
        length{utility::metresToPixels(1.0f)},
        minLength{0.0f},
        maxLength{utility::metresToPixels(FLT_MAX)}
    {
        type = JointType::Distance;
    }

    void DistanceJointDefinition::join(DistanceJointDefinition::BodyPtr body1,
        DistanceJointDefinition::BodyPtr body2, Vector2f anchorA, Vector2f anchorB)
    {
        IME_ASSERT(body1, "Two bodies are needed for a joint to occur, Body A is a nullptr");
        IME_ASSERT(body2, "Two bodies are needed for a joint to occur, Body B is a nullptr");
        IME_ASSERT(body1 != body2, "Cannot self join, bodies to be joined must be different objects");

        bodyA = body1;
        bodyB = body2;
        bodyALocalAnchorPoint = body1->getLocalPoint(anchorA);
        bodyBLocalAnchorPoint = body2->getLocalPoint(anchorB);

        auto distance = anchorB - anchorA;
        length = std::max(std::sqrt(distance.x * distance.x + distance.y * distance.y), b2_linearSlop);
        minLength = length;
        maxLength = length;
    }

    //////////////////////////////////////////////////////////////////////////
    // DistanceJoint class stuff
    //////////////////////////////////////////////////////////////////////////

    DistanceJoint::DistanceJoint(const DistanceJointDefinition& definition, World::sharedPtr world) {
        IME_ASSERT(definition.bodyA, "Two bodies are needed for a distance joint, Body A is a nullptr");
        IME_ASSERT(definition.bodyB, "Two bodies are needed for a distance joint, Body B is a nullptr");
        IME_ASSERT(definition.bodyA != definition.bodyB, "Cannot self join, bodies to be joined must be different objects");

        auto b2Definition = new b2DistanceJointDef();
        b2Definition->collideConnected = definition.areBodiesCollidable;
        b2Definition->type = b2JointType::e_distanceJoint;
        b2Definition->length = utility::pixelsToMetres(definition.length);
        b2Definition->minLength = utility::pixelsToMetres(definition.minLength);
        b2Definition->maxLength = utility::pixelsToMetres(definition.maxLength);
        b2Definition->bodyA = definition.bodyA->getInternalBody();
        b2Definition->bodyB = definition.bodyB->getInternalBody();

        b2Definition->localAnchorA = {utility::pixelsToMetres(definition.bodyALocalAnchorPoint.x),
                                      utility::pixelsToMetres(definition.bodyALocalAnchorPoint.y)};

        b2Definition->localAnchorB = {utility::pixelsToMetres(definition.bodyBLocalAnchorPoint.x),
                                      utility::pixelsToMetres(definition.bodyALocalAnchorPoint.y)};

        userData_ = definition.userData;
        world->getInternalWorld()->CreateJoint(b2Definition);

        // Cleanup
        delete b2Definition;
        b2Definition = nullptr;
    }

    float DistanceJoint::setRestLength(float length) {
        return utility::metresToPixels(joint_->SetLength(utility::pixelsToMetres(length)));
    }

    float DistanceJoint::getRestLength() const {
        return utility::metresToPixels(joint_->GetLength());
    }

    float DistanceJoint::setMinimumLength(float minLength) {
        return utility::metresToPixels(joint_->SetMinLength(utility::pixelsToMetres(minLength)));
    }

    float DistanceJoint::getMinimumLength() const {
        return utility::metresToPixels(joint_->GetMinLength());
    }

    float DistanceJoint::setMaxLength(float maxLength) {
        return utility::metresToPixels(utility::pixelsToMetres(joint_->SetMaxLength(maxLength)));
    }

    float DistanceJoint::getMaximumLength() const {
        return utility::metresToPixels(joint_->GetMaxLength());
    }

    float DistanceJoint::getCurrentLength() const {
        return utility::metresToPixels(joint_->GetCurrentLength());
    }

    Vector2f DistanceJoint::getBodyALocalAnchorPoint() const {
        auto [x, y] = joint_->GetLocalAnchorA();
        return {utility::metresToPixels(x), utility::metresToPixels(y)};
    }

    Vector2f DistanceJoint::getBodyAWorldAnchorPoint() const {
        auto [x, y] = joint_->GetAnchorA();
        return {utility::metresToPixels(x), utility::metresToPixels(y)};
    }

    Vector2f DistanceJoint::getBodyBLocalAnchorPoint() const {
        auto [x, y] = joint_->GetLocalAnchorB();
        return {utility::metresToPixels(x), utility::metresToPixels(y)};
    }

    Vector2f DistanceJoint::getBodyBWorldAnchorPoint() const {
        auto [x, y] = joint_->GetAnchorB();
        return {utility::metresToPixels(x), utility::metresToPixels(y)};
    }

    JointType DistanceJoint::getType() const {
        return JointType::Distance;
    }

    Body::sharedPtr DistanceJoint::getBodyA() {
        return bodyA_;
    }

    Body::sharedPtr DistanceJoint::getBodyB() {
        return bodyB_;
    }

    Vector2f DistanceJoint::getReactionForce(float fpsLimit) const {
        auto [x, y] = joint_->GetReactionForce(fpsLimit);
        return {x, y};
    }

    float DistanceJoint::getReactionTorque(float fpsLimit) const {
        return joint_->GetReactionTorque(fpsLimit);
    }

    PropertyContainer &DistanceJoint::getUserData() {
        return userData_;
    }

    const PropertyContainer &DistanceJoint::getUserData() const {
        return userData_;
    }

    bool DistanceJoint::canBodiesCollide() const {
        return joint_->GetCollideConnected();
    }

    b2Joint *DistanceJoint::getInternalJoint() {
        return joint_;
    }

    const b2Joint *DistanceJoint::getInternalJoint() const {
        return joint_;
    }

    DistanceJoint::~DistanceJoint() {
        delete joint_;
        joint_ = nullptr;
    }
}