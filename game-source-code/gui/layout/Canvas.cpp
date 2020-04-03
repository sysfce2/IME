#include "Canvas.h"

Gui::Canvas::Canvas(float x, float y) : Panel(x, y) {}

void Gui::Canvas::addElement(const std::string &alias, std::unique_ptr<UIElement> guiElement) {
    auto isElementInsidePanel = [&](){
        return guiElement->getPosition().x >= getPosition().x
               && guiElement->getPosition().x <= getPosition().x + getDimensions().width
               && guiElement->getPosition().y >= getPosition().y
               && guiElement->getPosition().y <= getPosition().y + getDimensions().height
               && guiElement->getDimensions().width <= getDimensions().width
               && guiElement->getDimensions().height <= getDimensions().height;
    }();

    if (isElementInsidePanel)
        add(alias, std::move(guiElement));
}
