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

#ifndef IME_SHAPE_H
#define IME_SHAPE_H

#include "IME/common/ITransformable.h"
#include "IME/graphics/IDrawable.h"
#include "IME/graphics/Colour.h"
#include "IME/common/Rect.h"

namespace ime {
    /**
     * @brief Abstract base class for all geometric figures
     *
     * Some common geometric figures are squares, rectangles and triangles
     */
    class IME_API Shape : public IDrawable, public ITransformable {
    public:
        /**
         * @brief The types of shapes
         */
        enum class Type {
            Rectangle, //!< Rectangle shape
            Circle,    //!< Circle shape
            Convex     //!< Convex polygon shape
        };

        /**
         * @brief Constructor
         * @param scene The scene the shape belongs to
         * @param type The type of this shape
         */
        explicit Shape(Type type);

        /**
         * @brief Get the type of this shape
         * @return The type of the shape
         */
        Type getShapeType() const;

        /**
         * @brief Set the fill colour of the shape
         * @param colour The new colour of the shape
         *
         * By default, the shape's fill colour is opaque white.
         */
        virtual void setFillColour(const Colour& colour) = 0;

        /**
         * @brief Get the fill colour of the shape
         * @return The fill colour of the shape
         */
        virtual Colour getFillColour() const = 0;

        /**
         * @brief Set the outline colour of the shape
         * @param colour The new outline colour
         *
         * By default, the shape's outline colour is opaque white.
         */
        virtual void setOutlineColour(const Colour& colour) = 0;

        /**
         * @brief Get the outline colour of the shape
         * @return The outline colour of the shape
         */
        virtual Colour getOutlineColour() const = 0;

        /**
         * @brief Set the thickness of the shape's outline
         * @param thickness The new outline thickness
         *
         * Note that negative values are allowed (so that the outline
         * expands towards the center of the shape), and using zero
         * disables the outline.
         *
         * By default, the outline thickness is 0.
         */
        virtual void setOutlineThickness(float thickness) = 0;

        /**
         * @brief Get the outline thickness of the shape
         * @return The outline thickness of the shape
         */
        virtual float getOutlineThickness() const = 0;

        /**
         * @brief Get the local bounding rectangle of the shape
         * @return The local bounding rectangle of the shape
         *
         * The returned rectangle is in local coordinates, which means
         * that it ignores the transformations (translation, rotation,
         * scale, ...) that are applied to the entity
         */
        virtual FloatRect getLocalBounds() const = 0;

        /**
         * @brief Get the global bounding rectangle of the shape
         * @return The global bounding rectangle of the shape
         *
         * The returned rectangle is in global coordinates, which means
         * that it takes into account the transformations (translation,
         * rotation, scale, ...) that are applied to the entity.
         * In other words, this function returns the bounds of the
         * shape in the global 2D world's coordinate system.
         */
        virtual FloatRect getGlobalBounds() const = 0;

        /**
         * @brief Destructor
         */
        virtual ~Shape() = default;

    private:
        Type type_;    //!< The type of this shape
    };
}

#endif //IME_SHAPE_H
