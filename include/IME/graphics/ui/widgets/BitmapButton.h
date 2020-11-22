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

/**
 * @brief Button widget with an image displayed next to the text (or centered
 *        in the button without text)
 */

#ifndef IME_BITMAPBUTTON_H
#define IME_BITMAPBUTTON_H

#include "IClickableWidget.h"
#include "IME/graphics/ui/renderers/ButtonRenderer.h"
#include <TGUI/Widgets/BitmapButton.hpp>

namespace IME {
    namespace Graphics::UI {
        class IME_API BitmapButton : public IClickableWidget {
        public:
            /**
             * @brief Construct an empty bitmap button
             */
            BitmapButton();

            /**
             * @brief Create a bitmap button
             * @param buttonText Text to be displayed inside the button
             */
            BitmapButton(const std::string &buttonText);

            /**
             * @brief Set the buttons renderer
             * @param renderer The new renderer
             *
             * The renderer determines how the button is displayed. The button
             * has a default renderer which can be manipulated using the
             * @see getRenderer() function
             */
            void setRenderer(std::shared_ptr<ButtonRenderer> renderer);

            /**
             * @brief Get the buttons renderer
             * @return The buttons renderer
             *
             * The renderer gives access to functions that determine how the
             * button is displayed. It allows you to manipulate things such
             * as the background colour, text colour, border colour etc...
             */
            std::shared_ptr<ButtonRenderer> getRenderer();

            /**
             * @brief Set the image that should be displayed next to the text
             * @param filename of the image to display
             * @throws FileNotFound if the specified image cannot be found on
             *         the disk
             */
            void setImage(const std::string& filename);

            /**
             * @brief Set the relative size of the image to display next to the
             *        text
             * @param relativeHeight height of the image compared to the button
             *        height
             *
             * The relative height must be in the range [0, 1] If set to 0
             * (default), the image will always have its original size, no
             * matter what the button size is
             */
            void setImageScaling(float relativeHeight);

            /**
             * @brief Get the relative size of the image displayed next to the
             *        text
             * @return The relative size of the image displayed next to the text
             */
            float getImageScaling() const;

            /**
             * @brief Set the position of the widget
             * @param X coordinate of the new position
             * @param Y coordinate of the new position
             *
             * This function completely overwrites the previous position.
             * See the move function to apply an offset based on the previous
             * position instead. The default position of a transformable widget
             * is (0, 0).
             */
            void setPosition(float x, float y) override;

            /**
             * @brief Set the position of the widget
             * @param position New position
             *
             * This function completely overwrites the previous position.
             * See the move function to apply an offset based on the previous
             * position instead. The default position of a transformable widget
             * is (0, 0).
             */
            void setPosition(Vector2f position) override;

            /**
             * @brief Set the orientation of the widget
             * @param angle New rotation, in degrees
             *
             * This function completely overwrites the previous rotation.
             * See the rotate function to add an angle based on the previous
             * rotation instead. The default rotation of a transformable widget
             * is 0.
             */
            void setRotation(float angle) override;

            /**
             * @brief Set the scale factors of the widget
             * @param factorX New horizontal scale factor
             * @param factorY New vertical scale factor
             *
             * This function completely overwrites the previous scale.
             */
            void setScale(float factorX, float factorY) override;

            /**
             * @brief Set the scale factors of the object
             * @param scale New scale
             * 
             * This function completely overwrites the previous scale
             * 
             * @see scale
             */
            void setScale(Vector2f scale) override;

            /**
             * @brief Get the current scale of the object
             * @return Current scale of the object
             */
            Vector2f getScale() const override;

            /**
             * @brief set the local origin of the object
             * @param origin New origin
             *
             * The origin of an object defines the center point for
             * all transformations (position, scale, rotation).
             * The coordinates of this point must be relative to the
             * top-left corner of the object, and ignore all
             * transformations (position, scale, rotation).
             * The default origin of a transformable object is (0, 0)
             */
            void setOrigin(Vector2f origin) override;

            /**
             * @brief Move the object by a given offset
             * @param offset Offset to apply
             *
             * This function adds to the current position of the object,
             * unlike @see setPosition which overwrites it
             */
            void move(Vector2f offset) override;

            /**
             * @brief Scale the object by an offset
             * @param offset Offset to apply
             *
             * This function multiplies the current scale of the object,
             * unlike @see setScale which overwrites it
             */
            void scale(Vector2f offset) override;

            /**
             * @brief Set the local origin of the widget
             * @param x X coordinate of the new origin
             * @param y Y coordinate of the new origin
             *
             * The origin of an widget defines the center point for
             * all transformations (position, scale, rotation).
             * The coordinates of this point must be relative to the
             * top-left corner of the widget, and ignore all
             * transformations (position, scale, rotation).
             * The default origin of a transformable widget is (0, 0).
             */
            void setOrigin(float x, float y) override;

            /**
             * @brief Get the position of the widget
             * @return Current position of the widget
             */
            Vector2f getPosition() const override;

            /**
             * @brief Get the local origin of the widget
             * @return get the local origin of the widget
             */
            Vector2f getOrigin() const override;

            /**
             * @brief Get the orientation of the widget
             * @return Current rotation, in degrees
             *
             * The rotation is always in the range [0, 360].
             */
            float getRotation() const override;

            /**
             * @brief Move the widget by a given offset
             * @param offsetX Horizontal offset
             * @param offsetY Vertical offset
             *
             * This function adds to the current position of the widget,
             * unlike setPosition which overwrites it.
             */
            void move(float xOffset, float yOffset) override;

            /**
             * @brief Rotate the widget
             * @param angle Angle of rotation, in degrees
             *
             * This function adds to the current rotation of the widget,
             * unlike setRotation() which overwrites it
             */
            void rotate(float offset) override;

            /**
             * @brief Scale the widget
             * @param factorX Horizontal scale factor
             * @param factorY Vertical scale factor
             *
             * This function multiplies the current scale of the widget,
             * unlike setScale() which overwrites it.
             */
            void scale(float factorX, float factorY) override;

            /**
            * @brief Hide widget from a render target
            *
            * A hidden widget will not be drawn on a render target when calling
            * draw()
            */
            void hide() override;

            /**
             * @brief Show a hidden widget
             *
             * This function will reveal an widget that was hidden prior to
             * function call. Calling this function on an widget that is not
             * hidden has no effect
             */
            void show() override;

            /**
             * @brief Check if widget is hidden or not
             * @return True if widget is hidden, false if widget is not hidden
             */
            bool isHidden() const override;

            /**
             * @brief Enable or disable the widget
             * @param isEnable Set true to enable the widget, false to
             *        disable the widget
             *
             * The widget is enabled by default
             *
             * @note Disabling the widget cancels all the interaction events.
             *       That is, the "mouseEnter", "mouseLeave", "click", "mouseUp"
             *       and "mouseDown" events will not fire while the widget is
             *       disabled
             */
            void setEnabled(bool isEnable) override;

            /**
              * @brief Check if widget is enabled or disabled
              * @return True if widget is enabled, false if widget is disabled
              *
              * @note A disabled widget cannot be interacted with using the mouse.
              * @see setEnabled(bool). Also, Hiding an widget disables it, @see hide()
              * in @class IWidget
              */
            bool isEnabled() const override;

            /**
             * @brief Disable widget if its currently enabled and vice versa
             */
            void toggleEnabled() override;

            /**
             * @brief Check if mouse cursor is currently over the widget or not
             * @return True if mouse cursor is over the widget, false otherwise
             */
            bool isMouseOverElement() const override;

            /**
             * @brief Set the character size of the widget's text
             * @param charSize New character size
             *
             * The default character size is 30
             */
            void setTextSize(unsigned int charSize) override;

            /**
             * @brief Set the text content of the widget
             * @param content New text content
             *
             * This function will overwrite any text that was previously
             * set
             */
            void setText(const std::string &content) override;

            /**
             * @brief Set the size of the widget
             * @param width The width of the widget
             * @param height The height of the widget
             */
            void setSize(float width, float height) override;

            /**
             * @brief Get the size of the widget
             * @return Current size of the widget
             *
             * This function only returns the size of the widget, to get the
             * absolute size (with the margin, outline thickness etc...)
             * @see getAbsoluteSize()
             */
            Vector2f getSize() const override;

            /**
             * @brief Get the absolute size of the widget
             * @return The absolute size of the widget
             *
             * The absolute size includes the size of the widget, the padding,
             * margin and outline thickness. To get just the size of the widget
             * use @see getSize()
             */
            Vector2f getAbsoluteSize() override;

            /**
             * @brief Get the widgets text content
             * @return Widgets text content
             */
            std::string getText() const override;

            /**
             * @brief Get the character size of the text
             * @return The character size of the text
             */
            unsigned int getTextSize() const override;

            /**
             * @brief Focus or unfocus widget
             * @param isFocused Set to true to focus or false to unfocus widget
             */
            void setFocused(bool isFocused) override;

            /**
             * @brief Check if widget is focused or not
             * @return True if widget is focused. Otherwise, false
             */
            bool isFocused() const override;

            /**
             * @brief Get the type of the widget
             * @return Type of the widget
             */
            std::string getType() const override;

            /**
             * @brief Toggle the visibility of the widget
             *
             * This function will make the widget invisible if its currently
             * visible and vice versa. The visibility is not automatically
             * reflected on the render target. A call to draw() must be made
             * after calling this function
             */
            void toggleVisibility() override;

            /**
             * @brief Check if coordinates lie inside the widget
             * @param x X coordinate to be checked
             * @param y Y coordinate to be checked
             * @return true if coordinates lie inside the widget, false if
             *         coordinates do not lie inside the widget
             */
            bool contains(float x, float y) const override;

            /**
             * @internal
             * @brief Get the internal pointer to a third party widget
             * @return The internal pointer to a third party widget
             *
             * @warning This function is intended for internal use only and
             * should never be called under any circumstance
             */
            std::shared_ptr<tgui::Widget> getInternalPtr() override;

        private:
            /**
             * @brief Initialize events
             *
             * These events will notify event listeners about an internal state
             * change of the object when that state changes
             */
            void initEvents();

        private:
            //Pointer to third party button
            std::shared_ptr<tgui::BitmapButton> button_;
            //Renderer for this button
            std::shared_ptr<ButtonRenderer> renderer_;
            //How long the button takes before its completely hidden or shown
            static const int fadeAnimDuration_ = 100;
        };
    }
}

#endif
