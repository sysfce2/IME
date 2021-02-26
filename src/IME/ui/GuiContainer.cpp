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

#include "IME/ui/GuiContainer.h"
#include "IME/core/resources/ResourceManager.h"
#include "IME/graphics/Window.h"
#include "../utility/Helpers.h"
#include "../graphics/WindowImpl.h"
#include <TGUI/Backends/SFML/GuiSFML.hpp>
#include <unordered_map>

namespace ime::ui {
    //////////////////////////////////////////////////////////////////////////
    // GuiContainer implementation
    //////////////////////////////////////////////////////////////////////////
    class GuiContainer::GuiContainerImpl {
    public:
        GuiContainerImpl() = default;

        GuiContainerImpl(Window &window) :
            sfmlGui_{window.getImpl()->getSFMLWindow()}
        {}

        void setAbsoluteViewport(const FloatRect &viewport) {
            sfmlGui_.setAbsoluteViewport({viewport.left, viewport.top,
                viewport.width, viewport.height});
        }

        void setRelativeViewport(const FloatRect &viewport) {
            sfmlGui_.setRelativeViewport({viewport.left, viewport.top,
                viewport.width, viewport.height});
        }

        FloatRect getViewport() const {
            return {sfmlGui_.getViewport().getPosition().x,
                    sfmlGui_.getViewport().getPosition().y,
                    sfmlGui_.getViewport().getSize().x,
                    sfmlGui_.getViewport().getSize().y};
        }

        void setAbsoluteView(const FloatRect &view) {
            sfmlGui_.setAbsoluteView({view.left, view.top, view.width, view.height});
        }

        void setRelativeView(const FloatRect &view) {
            sfmlGui_.setRelativeView({view.left, view.top, view.width, view.height});
        }

        FloatRect getView() const {
            return {sfmlGui_.getView().getPosition().x,
                    sfmlGui_.getView().getPosition().y,
                    sfmlGui_.getView().getSize().x,
                    sfmlGui_.getView().getSize().y};
        }

        void handleEvent(Event event) {
            sfmlGui_.handleEvent(utility::convertToSFMLEvent(event));
        }

        void setTabKeyUsageEnabled(bool enabled) {
            sfmlGui_.setTabKeyUsageEnabled(enabled);
        }

        bool isTabKeyUsageEnabled() const {
            return sfmlGui_.isTabKeyUsageEnabled();
        }

        void draw() {
            sfmlGui_.draw();
        }

        void setFont(const std::string& filename) {
            sfmlGui_.setFont(utility::get_TGUI_Font(filename));
        }

        void removeAllWidgets() {
            sfmlGui_.removeAllWidgets();
            widgets_.clear();
        }

        void setTarget(Window &window) {
            sfmlGui_.setTarget(window.getImpl()->getSFMLWindow());
        }

        std::shared_ptr<Widget> getWidgetBelowMouseCursor(Vector2f mousePos) const {
            auto widget = sfmlGui_.getWidgetBelowMouseCursor({static_cast<int>(mousePos.x), static_cast<int>(mousePos.y)});
            if (widget)
                return widgets_.at(widget->getWidgetName().toStdString());
            return nullptr;
        }

        void unfocusAllWidgets() {
            sfmlGui_.unfocusAllWidgets();
        }

        void setOpacity(float opacity) {
            sfmlGui_.setOpacity(opacity);
        }

        float getOpacity() const {
            return sfmlGui_.getOpacity();
        }

        void setMouseCursor(CursorType cursorType) {
            sfmlGui_.setOverrideMouseCursor(static_cast<tgui::Cursor::Type>(cursorType));
        }

        void restoreMouseCursor() {
            sfmlGui_.restoreOverrideMouseCursor();
        }

        void requestMouseCursor(CursorType cursorType) {
            sfmlGui_.requestMouseCursor(static_cast<tgui::Cursor::Type>(cursorType));
        }

        bool addWidget(std::shared_ptr<Widget> widget, const std::string &widgetName)
        {
            IME_ASSERT(widget, "Cannot add nullptr to a GUI container");
            if (widgets_.insert({widgetName, widget}).second) {
                sfmlGui_.add(std::static_pointer_cast<tgui::Widget>(widget->getInternalPtr()), widgetName);
                return true;
            }
            return false;
        }

        std::shared_ptr<Widget> getWidget(const std::string &widgetName) const {
            if (utility::findIn(widgets_, widgetName))
                return widgets_.at(widgetName);
            return utility::findRecursively(widgets_, widgetName);
        }

        bool removeWidget(const std::string &widget) {
            if (utility::findIn(widgets_, widget)) {
                sfmlGui_.remove(std::static_pointer_cast<tgui::Widget>(widgets_[widget]->getInternalPtr()));
                widgets_.erase(widget);
                return true;
            }
            return false;
        }

        std::shared_ptr<Widget> getFocusedWidget() const {
            auto widget = sfmlGui_.getFocusedChild();
            if (widget)
                return widgets_.at(widget->getWidgetName().toStdString());
            return nullptr;
        }

        std::shared_ptr<Widget> getFocusedLeaf() const {
            auto widget = sfmlGui_.getFocusedLeaf();
            if (widget)
                return widgets_.at(widget->getWidgetName().toStdString());
            return nullptr;
        }

        std::shared_ptr<Widget> getWidgetAtPosition(Vector2f pos) const {
            auto widget = sfmlGui_.getWidgetAtPosition({pos.x, pos.y});
            if (widget)
                return widgets_.at(widget->getWidgetName().toStdString());
            return nullptr;
        }

        bool focusNextWidget(bool recursive) {
            return sfmlGui_.focusNextWidget(recursive);
        }

        bool focusPreviousWidget(bool recursive) {
            return sfmlGui_.focusPreviousWidget(recursive);
        }

        void moveWidgetToFront(const std::shared_ptr<Widget> &widget) {
            sfmlGui_.moveWidgetToFront(std::static_pointer_cast<tgui::Widget>(widget->getInternalPtr()));
        }

        void moveWidgetToBack(const std::shared_ptr<Widget> &widget) {
            sfmlGui_.moveWidgetToBack(std::static_pointer_cast<tgui::Widget>(widget->getInternalPtr()));
        }

        size_t moveWidgetForward(std::shared_ptr<Widget> widget) {
            return sfmlGui_.moveWidgetForward(std::static_pointer_cast<tgui::Widget>(widget->getInternalPtr()));
        }

        size_t moveWidgetBackward(std::shared_ptr<Widget> widget) {
            return sfmlGui_.moveWidgetBackward(std::static_pointer_cast<tgui::Widget>(widget->getInternalPtr()));
        }

        void setTextSize(unsigned int size) {
            sfmlGui_.setTextSize(size);
        }

        unsigned int getTextSize() const {
            return sfmlGui_.getTextSize();
        }

        bool isTargetSet() const {
            return sfmlGui_.getTarget() != nullptr;
        }

    private:
        tgui::GuiSFML sfmlGui_; //!< Gui controller and renderer
        std::unordered_map<std::string, Widget::Ptr> widgets_; //!< Widgets container
    }; // class Impl

    //////////////////////////////////////////////////////////////////////////
    // GuiContainer class delegation
    //////////////////////////////////////////////////////////////////////////

    GuiContainer::GuiContainer() :
        pimpl_{std::make_unique<GuiContainerImpl>()}
    {}

    GuiContainer::GuiContainer(Window &window) :
        pimpl_{std::make_unique<GuiContainerImpl>(window)}
    {}

    GuiContainer::GuiContainer(GuiContainer &&) noexcept = default;

    GuiContainer &GuiContainer::operator=(GuiContainer &&) noexcept = default;

    void GuiContainer::setAbsoluteViewport(const FloatRect &viewport) {
        pimpl_->setAbsoluteViewport(viewport);
    }

    void GuiContainer::setRelativeViewport(const FloatRect &viewport) {
        pimpl_->setRelativeViewport(viewport);
    }

    FloatRect GuiContainer::getViewport() const {
        return pimpl_->getViewport();
    }

    void GuiContainer::setAbsoluteView(const FloatRect &view) {
        pimpl_->setAbsoluteView(view);
    }

    void GuiContainer::setRelativeView(const FloatRect &view) {
        pimpl_->setRelativeView(view);
    }

    FloatRect GuiContainer::getView() const {
        return pimpl_->getView();
    }

    void GuiContainer::handleEvent(Event event) {
        pimpl_->handleEvent(event);
    }

    void GuiContainer::setTabKeyUsageEnabled(bool enabled) {
        pimpl_->setTabKeyUsageEnabled(enabled);
    }

    bool GuiContainer::isTabKeyUsageEnabled() const {
        return pimpl_->isTabKeyUsageEnabled();
    }

    void GuiContainer::draw() {
        pimpl_->draw();
    }

    void GuiContainer::setFont(const std::string& filename) {
        pimpl_->setFont(filename);
    }

    void GuiContainer::removeAllWidgets() {
        pimpl_->removeAllWidgets();
    }

    void GuiContainer::setTarget(Window &window) {
        pimpl_->setTarget(window);
    }

    std::shared_ptr<Widget> GuiContainer::getWidgetBelowMouseCursor(Vector2f mousePos) const {
        return pimpl_->getWidgetBelowMouseCursor(mousePos);
    }

    void GuiContainer::unfocusAllWidgets() {
        pimpl_->unfocusAllWidgets();
    }

    void GuiContainer::setOpacity(float opacity) {
        pimpl_->setOpacity(opacity);
    }

    float GuiContainer::getOpacity() const {
        return pimpl_->getOpacity();
    }

    void GuiContainer::setMouseCursor(CursorType cursorType) {
        pimpl_->setMouseCursor(cursorType);
    }

    void GuiContainer::restoreMouseCursor() {
        pimpl_->restoreMouseCursor();
    }

    void GuiContainer::requestMouseCursor(CursorType cursorType) {
        pimpl_->requestMouseCursor(cursorType);
    }

    bool GuiContainer::addWidget(std::shared_ptr<Widget> widget,
         const std::string &widgetName)
    {
        return pimpl_->addWidget(std::move(widget), widgetName);
    }

    std::shared_ptr<Widget> GuiContainer::getWidget(const std::string &widgetName) const {
        return pimpl_->getWidget(widgetName);
    }

    bool GuiContainer::removeWidget(const std::string &widget) {
        return pimpl_->removeWidget(widget);
    }

    std::shared_ptr<Widget> GuiContainer::getFocusedWidget() const {
        return pimpl_->getFocusedWidget();
    }

    std::shared_ptr<Widget> GuiContainer::getFocusedLeaf() const {
        return pimpl_->getFocusedLeaf();
    }

    std::shared_ptr<Widget> GuiContainer::getWidgetAtPosition(Vector2f pos) const {
        return pimpl_->getWidgetAtPosition(pos);
    }

    bool GuiContainer::focusNextWidget(bool recursive) {
        return pimpl_->focusNextWidget(recursive);
    }

    bool GuiContainer::focusPreviousWidget(bool recursive) {
        return pimpl_->focusPreviousWidget(recursive);
    }

    void GuiContainer::moveWidgetToFront(const std::shared_ptr<Widget> &widget) {
        pimpl_->moveWidgetToFront(widget);
    }

    void GuiContainer::moveWidgetToBack(const std::shared_ptr<Widget> &widget) {
        pimpl_->moveWidgetToBack(widget);
    }

    size_t GuiContainer::moveWidgetForward(std::shared_ptr<Widget> widget) {
        return pimpl_->moveWidgetForward(std::move(widget));
    }

    size_t GuiContainer::moveWidgetBackward(std::shared_ptr<Widget> widget) {
        return pimpl_->moveWidgetBackward(std::move(widget));
    }

    void GuiContainer::setTextSize(unsigned int size) {
        pimpl_->setTextSize(size);
    }

    unsigned int GuiContainer::getTextSize() const {
        return pimpl_->getTextSize();
    }

    bool GuiContainer::isTargetSet() const {
        return pimpl_->isTargetSet();
    }

    GuiContainer::~GuiContainer() = default;
}
