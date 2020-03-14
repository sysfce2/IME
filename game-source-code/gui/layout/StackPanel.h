/**
 * @brief UI layout that stacks panel vertically or horizontally
 */

#ifndef STACKPANEL_H
#define STACKPANEL_H

#include "gui/layout/Panel.h"

namespace Gui {
    enum class Orientation{
        Vertical,
        Horizontal
    };

    class StackPanel : public Panel{
    public:
        /**
         * @brief Create a new Panel object
         * @param x X coordinate of the panel
         * @param y Y coordinate of the panel
         * @param orientation Specify how the UI elements are stacked to each other
         */
        StackPanel(float x, float y, Orientation orientation);

        /**
         * @brief Add a UI element to the panel
         * @param guiElement Element to be added
         */
        void addElement(std::shared_ptr<UIElement> guiElement) override;

    private:
        //Panel orientation state
        bool isOrientationVertical_;

        /**
         * @brief Set the position of a UI element relative to another UI element
         * @param uiElem UI element to position relative to another
         * @param refElem Reference UI element
         *
         * The element whose position is changed will have the same orientation
         * (vertical or horizontal) as the element which is used as reference.
         */
        void setElemPosRelativeTo(std::shared_ptr<UIElement> uiElem, std::shared_ptr<UIElement> refUIElem);
    };
}

#endif
