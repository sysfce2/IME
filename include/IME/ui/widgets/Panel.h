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

#ifndef IME_PANEL_H
#define IME_PANEL_H

#include "IME/Config.h"
#include "IContainer.h"
#include "IME/ui/renderers/PanelRenderer.h"
#include <TGUI/Widgets/Panel.hpp>
#include <memory>

namespace ime {
    namespace ui {
        /**
         * @brief A widget container that has a background colour and
         *        optional borders
         */
        class IME_API Panel : public IContainer {
        public:
            using sharedPtr = std::shared_ptr<Panel>; //!< Shared widget pointer
            using constSharedPtr = std::shared_ptr<const Panel>; //!< const shared widget pointer

            /**
             * @brief Constructor
             * @param width Width of the panel relative to the size of its
             *        parent
             * @param height Height of the panel relative to the size of its
             *        parent
             *
             * The relative size is specified in percentages as shown:
             *
             * @code
             * Panel({"50%", "20%"});
             * @endcode
             * 
             * By default, the panel is the same size as its parent
             */
            explicit Panel(const std::string& width = "100%", 
                const std::string& height = "100%");

            /**
             * @brief Create a new Panel widget
             * @param width Width of the panel relative to the size of its
             *        parent
             * @param height Height of the panel relative to the size of its
             *        parent
             * @return The new Panel
             *
             * * The relative size is specified in percentages as shown:
             *
             * @code
             * Panel::create({"50%", "20%"});
             * @endcode
             * 
             * By default, the new panel is the same size as its parent
             */
            static sharedPtr create(const std::string& width = "100%",
                const std::string& height = "100%");

            /**
             * @brief Create a copy of another panel
             * @param other The panel to copy
             * @param shareRenderer True if the new panel should have the
             *          same renderer as the copied panel
             * @return The new panel widget
             *
             * When the panels share a renderer, changes in a render
             * property of one of the panels automatically reflect on
             * the other panel, otherwise each panel has its own renderer
             * and changes in render properties are isolated to the specific
             * panel.
             *
             * @note when the panels don't share a render, the renderer of
             * the new panel widget will initially have the properties of
             * the copied panel such that the two look the same after this
             * function returns
             *
             * By default, the panels share a renderer
             *
             * @warning Once a renderer is shared, it cannot be unshared at
             * a later time
             */
            static sharedPtr copy(constSharedPtr other, bool shareRenderer = true);

            /**
             * @brief Set the panels renderer
             * @param renderer The new renderer
             *
             * The renderer determines how the panel is displayed.
             *
             * @note The panel has a default renderer
             *
             * @see getRenderer
             */
            void setRenderer(PanelRenderer::sharedPtr renderer);

            /**
             * @brief Get the panels renderer
             * @return The panels renderer
             *
             * The renderer gives access to functions that determine how the
             * panel is displayed. It allows you to manipulate things such
             * as the background colour, border colour etc...
             *
             * @see setRenderer
             */
            PanelRenderer::sharedPtr getRenderer();

            /**
             * @brief Set the character size of the text
             * @param charSize New character size
             */
            void setTextSize(unsigned int charSize) override;

            /**
             * @brief Get the character size of the text
             * @return The character size of the text
             */
            unsigned int getTextSize() const override;

            /**
             * @brief Set the size of the panel
             * @param width The width of the panel
             * @param height The height of the panel
             */
            void setSize(float width, float height) override;

            /**
             * @brief Set the size of the panel relative to the size of
             *        its parent
             * @param width The new width of the panel
             * @param height The new height of the panel
             *
             * The size is specified in percentages as shown below:
             *
             * @code
             * panel->setSize({"20%", "5%"});
             * @endcode
             */
            void setSize(const std::string& width, const std::string& height) override;

            /**
             * @brief Get the size of the panel
             * @return Current size of the panel
             *
             * This function only returns the size of the panel (It does
             * not accommodate margin, outline thickness etc ...)
             *
             * @see getAbsoluteSize
             */
            Vector2f getSize() const override;

            /**
             * @brief Get the absolute size of the panel
             * @return The absolute size of the panel
             *
             * The absolute size includes the size of the panel, the padding,
             * margin and outline thickness
             *
             * @see getSize
             */
            Vector2f getAbsoluteSize() override;

            /**
             * @brief Set the width of the panel
             * @param width New width of the panel
             * 
             * This function sets the width while keeping the height
             * the same
             * 
             * @see setSize
             */
            void setWidth(float width) override;

            /**
             * @brief Set the width of the panel relative to its parent
             * @param width New width
             * 
             * The relative width is given in percentages as shown:
             * 
             * @code
             * panel->setWidth("10%");
             * @endcode
             * 
             * This function sets the width of the panel while keeping the
             * height the same
             * 
             * @see setSize
             */
            void setWidth(const std::string& width) override;

            /**
             * @brief Set the height of the panel
             * @param height New height of the panel
             * 
             * This function sets the height while keeping the width 
             * the same
             * 
             * @see setSize
             */
            void setHeight(float height) override;

            /**
             * @brief Set the height of the panel relative to its parent
             * @param height New height
             * 
             * The relative height is given in percentages as shown:
             * 
             * @code
             * panel->setHeight("10%");
             * @endcode
             * 
             * This function sets the height of the panel while keeping the
             * width the same
             * 
             * @see setSize
             */
            void setHeight(const std::string& height) override;

            /**
             * @brief Set the mouse cursor that is displayed when the mouse
             *        is on top of the panel
             * @param cursor The cursor to be shown
             *
             * By default, the arrow cursor is shown
             */
            void setMouseCursor(CursorType cursor) override;

            /**
             * @brief Get the mouse cursor that is displayed when the mouse
             *        is on top of the panel
             * @return The cursor shown when hovering above the panel
             */
            CursorType getMouseCursor() const override;

            /**
             * @brief Get the type of the panel
             * @return The type of the panel
             */
            std::string getWidgetType() const override;

            /**
             * @brief Show the panel with an animation
             * @param type Type of the animation
             * @param duration Duration of the animation in milliseconds
             *
             * The animation will be played if the panel currently
             * visible
             *
             * @note During the animation the position, size and/or opacity
             * opacity may change. Once the animation is done the panel
             * will be back in the state in which it was when this function
             * was called
             *
             * @see hideWithEffect
             * @see isAnimationPlaying
             */
            void showWithEffect(ShowAnimationType type, int duration) override;

            /**
             * @brief Hide the panel with an animation
             * @param type Type of the animation
             * @param duration Duration of the animation in milliseconds
             *
             * The animation will also be played if the panel currently
             * hidden but it will not be seen
             *
             * @note During the animation the position, size and/or opacity
             * opacity may change. Once the animation is done the panel
             * will be back in the state in which it was when this function
             * was called
             *
             * @see showWithEffect
             * @see isAnimationPlaying
             */
            void hideWithEffect(ShowAnimationType type, int duration) override;

            /**
             * @brief Check whether or not an animation is currently playing
             * @return True if an animation is playing, otherwise false
             *
             * @see showWithEffect
             * @see hideWithEffect
             */
            bool isAnimationPlaying() const override;

            /**
             * @brief Show or hide a panel
             * @param visible True to show or false to hide
             *
             * If the panel is hidden, it won't receive events
             * (and thus won't send callbacks) nor will it be drawn
             *
             * The panel is visible by default.
             */
            void setVisible(bool visible) override;

            /**
             * @brief Check if the panel is visible or not
             * @return True if the panel is visible or false if hidden
             */
            bool isVisible() const override;

            /**
             * @brief Toggle the visibility of the panel
             *
             * This function will hide the panel if its currently
             * visible and vice versa
             * 
             * @see setVisible
             */
            void toggleVisibility() override;

            /**
             * @brief Check if coordinates lie inside the panel
             * @param x X coordinate to be checked
             * @param y Y coordinate to be checked
             * @return true if coordinates lie inside the panel, false if
             *         coordinates do not lie inside the panel
             */
            bool contains(float x, float y) const override;

            /**
             * @brief Set the position of the panel
             * @param x X coordinate of the new position
             * @param y Y coordinate of the new position
             *
             * This function completely overwrites the previous position.
             * use move function to apply an offset based on the previous
             * position instead
             *
             * The default position of a the panel is (0, 0)
             *
             * @see move
             */
            void setPosition(float x, float y) override;

            /**
             * @brief Set the position of the panel
             * @param position New position
             *
             * This function completely overwrites the previous position.
             * Use the move function to apply an offset based on the previous
             * position instead.
             *
             * The default position of the panel is (0, 0)
             *
             * @see move
             */
            void setPosition(Vector2f position) override;

            /**
             * @brief Set the position of the panel relative to the
             *        size of its parent
             * @param x New x coordinate of the panel
             * @param y New y coordinate of the panel
             * 
             * The position is specified in percentages as shown below:
             * 
             * @code
             * panel->setPosition({"5%", "10%"});
             * @endcode
             * 
             * This function completely overwrites the previous position.
             * Use the move function to apply an offset based on the previous
             * position instead.
             *
             * The default position of the panel is (0, 0)
             *
             * @see move
             */
            void setPosition(const std::string& x, const std::string& y) override;

            /**
             * @brief Get the position of the panel
             * @return Current position of the panel
             */
            Vector2f getPosition() const override;

            /**
             * @brief Get the absolute position of the panel
             * @return The absolute position of the panel
             * 
             * Unlike getPosition, this function returns the absolute 
             * position of the top-left point of the panel instead 
             * of the relative position to its parent
             * 
             * @see setPosition
             */
            Vector2f getAbsolutePosition() const override;

            /**
             * @brief Set the orientation of the panel
             * @param angle New rotation, in degrees
             *
             * This function completely overwrites the previous rotation.
             * See the rotate function to add an angle based on the previous
             * rotation instead.
             *
             * The default rotation of the panel is 0
             *
             * @see rotate
             */
            void setRotation(float angle) override;

            /**
             * @brief Rotate the panel
             * @param angle Angle of rotation, in degrees
             *
             * This function adds to the current rotation of the panel,
             * unlike setRotation which overwrites it
             *
             * @see setRotation
             */
            void rotate(float angle) override;

            /**
             * @brief Get the orientation of the panel
             * @return Current rotation, in degrees
             *
             * The rotation is always in the range [0, 360]
             */
            float getRotation() const override;

            /**
             * @brief Set the scale factors of the panel
             * @param factorX New horizontal scale factor
             * @param factorY New vertical scale factor
             *
             * This function completely overwrites the previous scale
             *
             * @see scale
             */
            void setScale(float factorX, float factorY) override;

            /**
             * @brief Set the scale factor of the panel
             * @param scale New scale
             *
             * This function completely overwrites the previous scale
             *
             * @see scale
             */
            void setScale(Vector2f scale) override;

            /**
             * @brief Scale the panel by an offset
             * @param factorX Horizontal scale factor
             * @param factorY Vertical scale factor
             *
             * This function multiplies the current scale of the panel,
             * unlike setScale which overwrites it
             *
             * @see setScale
             */
            void scale(float factorX, float factorY) override;

            /**
             * @brief Scale the panel by an offset
             * @param offset Offset to apply
             *
             * This function multiplies the current scale of the panel,
             * unlike setScale which overwrites it
             *
             * @see setScale
             */
            void scale(Vector2f offset) override;

            /**
             * @brief Get the current scale of the panel
             * @return Current scale of the panel
             */
            Vector2f getScale() const override;

            /**
             * @brief Set the local origin of the panel
             * @param x X coordinate of the new origin
             * @param y Y coordinate of the new origin
             *
             * The origin of the panel defines the center point for
             * all transformations (position, scale, rotation).
             * The coordinates of this point must be relative to the
             * top-left corner of the panel, and ignore all
             * transformations (position, scale, rotation).
             *
             * The default origin of the panel is (0, 0)
             */
            void setOrigin(float x, float y) override;

            /**
             * @brief Set the local origin of the panel
             * @param origin New origin
             *
             * The origin of the panel defines the center point for
             * all transformations (position, scale, rotation).
             * The coordinates of this point must be relative to the
             * top-left corner of the panel, and ignore all
             * transformations (position, scale, rotation).
             *
             * The default origin of the panel is (0, 0)
             */
            void setOrigin(Vector2f origin) override;

            /**
             * @brief Get the local origin of the panel
             * @return Local origin of the panel
             */
            Vector2f getOrigin() const override;

            /**
             * @brief Move the panel by a given offset
             * @param offsetX Horizontal offset
             * @param offsetY Vertical offset
             *
             * This function adds to the current position of the panel,
             * unlike setPosition which overwrites it
             *
             * @see setPosition
             */
            void move(float offsetX, float offsetY) override;

            /**
             * @brief Move the panel by a given offset
             * @param offset Offset to apply
             *
             * This function adds to the current position of the panel,
             * unlike setPosition which overwrites it
             *
             * @see setPosition
             */
            void move(Vector2f offset) override;

            /**
             * @brief Add a widget to the container
             * @param widget Widget to be added
             * @param name Unique name of the widget
             * @return True if the widget was added to the container or false
             *         if the container already has a widget with the same name
             *         as the specified widget name
             *
             * The name of the widget must not contain whitespaces
             */
            bool addWidget(IWidget::sharedPtr widget, const std::string &name) override;

            /**
             * @brief Get a widget in the container
             * @param name Name of the widget to retrieve
             * @return Pointer to the specified widget or a nullptr if the
             *         container does not have a widget with the specified
             *         name
             *
             * The container will first search for widgets that are direct
             * children of it, but when none of the child widgets match the
             * given name, a recursive search will be performed.
             */
            IWidget::sharedPtr getWidget(const std::string &name) const override;
            using IContainer::getWidget;

            /**
             * @brief Remove a widget from the container
             * @param widget Name of the widget to be removed from the container
             * @return True if the widget was removed or false if the widget
             *         does not exist in the container
             */
            bool removeWidget(const std::string &widget) override;

            /**
             * @brief Remove all widgets from the container
             */
            void removeAllWidgets() override;

            /**
             * @brief Place a widget before all other widgets, to the front
             *        of the z-order
             * @param widget The widget that should be moved to the front
             */
            void moveWidgetToFront(IWidget::sharedPtr widget) override;

            /**
            * @brief Place a widget behind all other widgets, to the back
            *        of the z-order
            * @param widget The widget that should be moved to the front
            */
            void moveWidgetToBack(IWidget::sharedPtr widget) override;

            /**
             * @brief Place a widget one step forward in the z-order
             * @param widget The widget that should be moved one step forward
             * @return New index in the widgets list (one higher than the old
             *         index or the same if the widget was already in front),
             */
            size_t moveWidgetForward(IWidget::sharedPtr widget) override;

            /**
             * @brief Place a widget one step backwards in the z-order
             * @param widget The widget that should be moved one step backward
             * @return New index in the widgets list (one higher than the old
             *         index or the same if the widget was already in front),
             */
            size_t moveWidgetBackward(IWidget::sharedPtr widget) override;

            /**
             * @brief Get the currently focused widget inside the container
             * @return Pointer to the focused child widget or a nullptr if none
             *         of the widgets are currently focused
             *
             * @note If the focused widget is a container, then a pointer to
             * the container is returned rather than a pointer to the focused
             * widget inside that container
             *
             * @see getFocusedLeaf
             */
            IWidget::sharedPtr getFocusedWidget() const override;

            /**
             * @brief Get the currently focused widget inside the container
             * @return Pointer to the focused child widget or a nullptr if none
             *         of the widgets are currently focused
             *
             * @note Unlike getFocusedWidget which returns a pointer to
             * a container when the focused widget is a child of another
             * container within the container, this function will always return
             * the focused widget regardless of whether its a direct child of
             * the container or not
             *
             * @see getFocusedWidget
             */
            IWidget::sharedPtr getFocusedLeaf() const override;

            /**
             * @brief Get a widget at a given position
             * @param pos The position of the widget relative to the container
             *          view
             * @return Pointer to the widget at the specified position or a
             *         nullptr if there is no widget at that position
             */
            IWidget::sharedPtr getWidgetAtPosition(Vector2f pos) const override;

            /**
             * @brief Focus the next widget in the container
             * @param recursive Set true to focus next widget when the currently
             *        focused widget is a container or false to focus the sibling
             *        of that container
             * @return True if the next widget was focused, otherwise false
             */
            bool focusNextWidget(bool recursive) override;

            /**
             * @brief Focus the previous widget in the container
             * @param recursive Set true to focus next widget when the currently
             *        focused widget is a container or false to focus the sibling
             *        of that container
             * @return True if the next widget was focused, otherwise false
             */
            bool focusPreviousWidget(bool recursive) override;

            /**
             * @internal
             * @brief Get the internal pointer to a third party widget object
             * @return The internal pointer to a third party widget object
             *
             * @warning This function is intended for internal use only and
             * must never be called under any circumstance
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
            std::unordered_map<std::string, IWidget::sharedPtr> widgets_; //!< Widgets container
            std::shared_ptr<tgui::Panel> panel_;                           //!< Pointer to third party panel
            PanelRenderer::sharedPtr renderer_;                            //!< Renderer
        };
    }
}

#endif //IME_PANEL_H
