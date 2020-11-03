/**
 * @brief Interface for all widget containers
 */

#include "IME/graphics/ui/widgets/IWidget.h"
#include <memory>

#ifndef ICONTAINER_H
#define ICONTAINER_H

namespace IME {
    namespace Graphics::UI {
        class IContainer : public IWidget {
        public:
            /**
             * @brief Add a widget to the container
             * @param widgetPtr Pointer to the widget to be added
             * @param widgetName Unique Name of the widget
             * @return True if the widget was added to the container or false
             *         if the container already has a widget with the same name
             *         as the specified widget name
             *
             * The name of the widget must not contain whitespaces
             */
            virtual bool addWidget(std::shared_ptr<IWidget> widgetPtr,
                const std::string& widgetName) = 0;
            
            /**
             * @brief Get a pointer to a widget in the container
             * @param widgetName Name of the widget to retrieve
             * @return Pointer to the specified widget or a nullptr if the 
             *         container does not have a widget with the specified 
             *         name
             *
             * The container will first search for widgets that are direct 
             * children of it, but when none of the child widgets match the 
             * given name, a recursive search will be performed.
             */
            virtual std::shared_ptr<IWidget> getWidget(const std::string& widgetName) const = 0;

            /**
             * @brief Get a list of all widgets in the container
             * @return A vector of all widgets in the container
             */
            virtual const std::vector<IWidget>& getWidgets() const = 0;

            /**
             * @brief Remove a widget from the container
             * @param widget Name of the widget to be removed from the container
             * @return True if the widget was removed or false if the widget
             *         does not exist in the container
             */
            virtual bool removeWidget(const std::string &widget) = 0;

            /**
             * @brief Remove all widgets from the container
             */
            virtual void removeAllWidgets() = 0;

            /**
             * @brief Place a widget before all other widgets, to the front
             *        of the z-order
             * @param widget The widget that should be moved to the front
             */
            virtual void moveWidgetToFront(std::shared_ptr<IWidget> widget) = 0;

            /**
             * @brief Place a widget behind all other widgets, to the back
             *        of the z-order
             * @param widget The widget that should be moved to the front
             */
            virtual void moveWidgetToBack(std::shared_ptr<IWidget> widget) = 0;

            /**
             * @brief Place a widget one step forward in the z-order
             * @param widget The widget that should be moved one step forward
             * @return New index in the widgets list (one higher than the old
             *         index or the same if the widget was already in front),
             */
            virtual std::size_t moveWidgetForward(std::shared_ptr<IWidget> widget) = 0;

            /**
             * @brief Place a widget one step backwards in the z-order
             * @param widget The widget that should be moved one step backward
             * @return New index in the widgets list (one higher than the old
             *         index or the same if the widget was already in front),
             */
            virtual std::size_t moveWidgetBackward(std::shared_ptr<IWidget> widget) = 0;

            /**
             * @brief Get the currently focused widget inside the container
             * @return Pointer to the focused child widget or a nullptr if none
             *         of the widgets are currently focused
             *
             * @note If the focused widget is a container, then a pointer to
             * the container is returned rather than a pointer to the focused
             * widget inside that container. @see getFocusedLeaf() should be
             * used to get the widget that is focused inside a container
             */
            virtual std::shared_ptr<IWidget> getFocusedWidget() const = 0;
            
            /**
             * @brief Get the currently focused widget inside the container
             * @return Pointer to the focused child widget or a nullptr if none
             *         of the widgets are currently focused
             *
             * @note Unlike @see getFocusedWidget() which returns a pointer to
             * a container when the focused widget is a child of another
             * container within the container, this function will always return 
             * the focused widget regardless of whether its a direct child of 
             * the container or not
             */
            virtual std::shared_ptr<IWidget> getFocusedLeaf() const = 0;

            /**
             * @brief Get a widget at a given position
             * @param pos The position of the widget relative to the container 
             *          view
             * @return Pointer to the widget at the specified position or a
             *         nullptr if there is no widget at that position
             */
            virtual std::shared_ptr<IWidget> getWidgetAtPosition(Position pos) const = 0;

            /**
             * @brief Focus the next widget in the container
             * @param recursive Set true to focus next widget when the currently
             *        focused widget is a container or false to focus the sibling
             *        of that container
             * @return True if the next widget was focused, otherwise false
             */
            virtual bool focusNextWidget(bool recursive = true) = 0;

            /**
             * @brief Focus the previous widget in the container
             * @param recursive Set true to focus next widget when the currently
             *        focused widget is a container or false to focus the sibling
             *        of that container
             * @return True if the next widget was focused, otherwise false
             */
            virtual bool focusPreviousWidget(bool recursive = true) = 0;
        };
    }
}

#endif