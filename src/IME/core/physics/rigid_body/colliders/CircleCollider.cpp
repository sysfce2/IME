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

#include "IME/core/physics/rigid_body/colliders/CircleCollider.h"
#include "IME/utility/Helpers.h"
#include <box2d/b2_circle_shape.h>

namespace ime {
    CircleCollider::CircleCollider(float radius) :
        Collider(Collider::Type::Circle),
        circle_{std::make_unique<b2CircleShape>()}
    {
        setRadius(radius);
    }

    CircleCollider::CircleCollider(const CircleCollider& other) :
        Collider(other),
        circle_{std::make_unique<b2CircleShape>(*other.circle_)}
    {}

    CircleCollider &CircleCollider::operator=(const CircleCollider& rhs) {
        if (this != &rhs) {
            auto temp{rhs};
            circle_ = std::move(temp.circle_);
        }

        return *this;
    }

    CircleCollider::CircleCollider(CircleCollider&&) noexcept = default;
    CircleCollider &CircleCollider::operator=(CircleCollider&&) noexcept = default;

    CircleCollider::Ptr CircleCollider::copy() const {
        return std::static_pointer_cast<CircleCollider>(clone());
    }

    Collider::Ptr CircleCollider::clone() const {
        return CircleCollider::Ptr(new CircleCollider(*this));
    }

    CircleCollider::Ptr CircleCollider::create(float radius) {
        return std::make_shared<CircleCollider>(radius);
    }

    std::string CircleCollider::getClassName() const {
        return "CircleCollider";
    }

    void CircleCollider::setPosition(Vector2f position) {
        circle_->m_p.x = utility::pixelsToMetres(position.x);
        circle_->m_p.y = utility::pixelsToMetres(position.y);
        emitChange(Property{"position", position});
    }

    Vector2f CircleCollider::getPosition() const {
        return {utility::metresToPixels(circle_->m_p.x), utility::metresToPixels(circle_->m_p.y)};
    }

    void CircleCollider::setRadius(float radius) {
        circle_->m_radius = utility::pixelsToMetres(radius);
        emitChange(Property{"radius", radius});
    }

    float CircleCollider::getRadius() const {
        return utility::metresToPixels(circle_->m_radius);
    }

    b2Shape& CircleCollider::getInternalShape() {
        return *circle_;
    }

    const b2Shape& CircleCollider::getInternalShape() const {
        return *circle_;
    }

    CircleCollider::~CircleCollider() = default;
}

