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

#include "IME/ui/widgets/ChildWindow.h"
#include "IME/utility/Helpers.h"

namespace ime::ui {
    ChildWindow::ChildWindow(const std::string &title, unsigned int titleButtons) :
        window_{tgui::ChildWindow::create(title, titleButtons)},
        renderer_{std::make_shared<ChildWindowRenderer>()}
    {
        renderer_->setInternalPtr(window_->getRenderer());
        setAsContainer(true);
        initEvents();
    }

    ChildWindow::sharedPtr ChildWindow::create(const std::string& title,
        unsigned int titleButtons)
    {
        return std::make_shared<ChildWindow>(title, titleButtons);
    }

    ChildWindow::sharedPtr ChildWindow::copy(ChildWindow::constSharedPtr other,
        bool shareRenderer)
    {
        auto widget = create();
        widget->window_ = widget->window_->copy(other->window_);

        if (!shareRenderer)
            widget->window_->setRenderer(other->window_->getRenderer()->clone());
        widget->renderer_->setInternalPtr(other->window_->getRenderer());

        return widget;
    }

    void ChildWindow::setRenderer(std::shared_ptr<ChildWindowRenderer> renderer) {
        IME_ASSERT(renderer, "Cannot set nullptr as renderer");
        renderer_ = renderer;
        window_->setRenderer(renderer->getInternalPtr()->getData());
    }

    std::shared_ptr<ChildWindowRenderer> ChildWindow::getRenderer() {
        return renderer_;
    }

    void ChildWindow::setClientSize(Vector2f size) {
        window_->setClientSize({size.x, size.y});
    }

    Vector2f ChildWindow::getClientSize() const {
        return {window_->getClientSize().x, window_->getClientSize().y};
    }

    void ChildWindow::setMaximumSize(Vector2f size) {
        window_->setMaximumSize({size.x, size.y});
    }

    Vector2f ChildWindow::getMaximumSize() const {
        return {window_->getMaximumSize().x, window_->getMaximumSize().y};
    }

    void ChildWindow::setMinimumSize(Vector2f size) {
        window_->setMinimumSize({size.x, size.y});
    }

    Vector2f ChildWindow::getMinimumSize() const {
        return {window_->getMinimumSize().x, window_->getMinimumSize().y};
    }

    void ChildWindow::setTitle(const std::string &title) {
        window_->setTitle(title);
    }

    std::string ChildWindow::getTitle() const {
        return window_->getTitle().toAnsiString();
    }

    void ChildWindow::setTitleTextSize(unsigned int size) {
        window_->setTitleTextSize(size);
    }

    unsigned int ChildWindow::getTitleTextSize() const {
        return window_->getTitleTextSize();
    }

    void ChildWindow::setTitleAlignment(ChildWindow::TitleAlignment alignment) {
        window_->setTitleAlignment(static_cast<tgui::ChildWindow::TitleAlignment>(alignment));
    }

    ChildWindow::TitleAlignment ChildWindow::getTitleAlignment() const {
        return static_cast<TitleAlignment>(window_->getTitleAlignment());
    }

    void ChildWindow::setTitleButtons(unsigned int buttons) {
        window_->setTitleButtons(buttons);
    }

    void ChildWindow::close() {
        window_->close();
    }

    void ChildWindow::destroy() {
        window_->destroy();
    }

    void ChildWindow::setResizable(bool resizable) {
        window_->setResizable(resizable);
    }

    bool ChildWindow::isResizable() const {
        return window_->isResizable();
    }

    void ChildWindow::setDraggable(bool draggable) {
        window_->setPositionLocked(!draggable);
    }

    bool ChildWindow::isDraggable() const {
        return !window_->isPositionLocked();
    }

    void ChildWindow::setKeepInParent(bool enabled) {
        window_->setKeepInParent(enabled);
    }

    bool ChildWindow::isKeptInParent() const {
        return window_->isKeptInParent();
    }

    void ChildWindow::setTextSize(unsigned int charSize) {
        window_->setTextSize(charSize);
    }

    void ChildWindow::setSize(float width, float height) {
        window_->setSize({width, height});
    }

    void ChildWindow::setSize(const std::string &width, const std::string &height) {
        window_->setSize({width.c_str(), height.c_str()});
    }

    Vector2f ChildWindow::getSize() const {
        return {window_->getSize().x, window_->getSize().y};
    }

    Vector2f ChildWindow::getAbsoluteSize() {
        return {window_->getFullSize().x, window_->getFullSize().y};
    }

    void ChildWindow::setWidth(float width) {
        window_->setWidth(width);
    }

    void ChildWindow::setWidth(const std::string &width) {
        window_->setWidth(width.c_str());
    }

    void ChildWindow::setHeight(float height) {
        window_->setHeight(height);
    }

    void ChildWindow::setHeight(const std::string &height) {
        window_->setHeight(height.c_str());
    }

    void ChildWindow::setMouseCursor(CursorType cursor) {
        window_->setMouseCursor(static_cast<tgui::Cursor::Type>(static_cast<int>(cursor)));
    }

    CursorType ChildWindow::getMouseCursor() const {
        return static_cast<CursorType>(static_cast<int>(window_->getMouseCursor()));
    }

    unsigned int ChildWindow::getTextSize() const {
        return window_->getTextSize();
    }

    std::string ChildWindow::getWidgetType() const {
        return "ChildWindow";
    }

    void ChildWindow::showWithEffect(ShowAnimationType type, int duration) {
        window_->showWithEffect(static_cast<tgui::ShowAnimationType>(type), duration);
    }

    void ChildWindow::hideWithEffect(ShowAnimationType type, int duration) {
        window_->hideWithEffect(static_cast<tgui::ShowAnimationType>(type), duration);
    }

    bool ChildWindow::isAnimationPlaying() const {
        return window_->isAnimationPlaying();
    }

    void ChildWindow::setVisible(bool visible) {
        window_->setVisible(visible);
    }

    bool ChildWindow::isVisible() const {
        return window_->isVisible();
    }

    void ChildWindow::toggleVisibility() {
        window_->setVisible(!window_->isVisible());
    }

    bool ChildWindow::contains(float x, float y) const {
        return window_->isMouseOnWidget({x, y});
    }

    void ChildWindow::setPosition(float x, float y) {
        window_->setPosition({x, y});
    }

    void ChildWindow::setPosition(const std::string &x, const std::string &y) {
        window_->setPosition({x.c_str(), y.c_str()});
    }

    void ChildWindow::setPosition(Vector2f position) {
        setPosition(position.x, position.y);
    }

    void ChildWindow::setRotation(float angle) {
        window_->setRotation(angle);
    }

    void ChildWindow::setScale(float factorX, float factorY) {
        window_->setScale({factorX, factorY});
    }

    void ChildWindow::setOrigin(float x, float y) {
        window_->setOrigin({x, y});
    }

    Vector2f ChildWindow::getPosition() const {
        return {window_->getPosition().x, window_->getPosition().y};
    }

    Vector2f ChildWindow::getAbsolutePosition() const {
        return {window_->getAbsolutePosition().x, window_->getAbsolutePosition().y};
    }

    Vector2f ChildWindow::getOrigin() const {
        return {window_->getOrigin().x, window_->getOrigin().y};
    }

    float ChildWindow::getRotation() const {
        return window_->getRotation();
    }

    void ChildWindow::move(float offsetX, float offsetY) {
        window_->setPosition(getPosition().x + offsetX,
                             getPosition().y + offsetY);
    }

    void ChildWindow::rotate(float angle) {
        setRotation(getRotation() + angle);
    }

    void ChildWindow::scale(float factorX, float factorY) {
        setScale(window_->getScale().x + factorX, window_->getScale().y + factorY);
    }

    bool ChildWindow::addWidget(std::shared_ptr<IWidget> widget,
        const std::string &widgetName)
    {
        IME_ASSERT(widget, "Cannot add nullptr to a widget container");
        if (widgets_.insert({widgetName, widget}).second) {
            window_->add(widget->getInternalPtr(), widgetName);
            return true;
        }
        return false;
    }

    std::shared_ptr<IWidget> ChildWindow::getWidget(const std::string &name) const {
        if (utility::findIn(widgets_, name))
            return widgets_.at(name);
        return utility::findRecursively(widgets_, name);;
    }

    bool ChildWindow::removeWidget(const std::string &widget) {
        if (utility::findIn(widgets_, widget)) {
            window_->remove(widgets_[widget]->getInternalPtr());
            widgets_.erase(widget);
            return true;
        }
        return false;
    }

    void ChildWindow::removeAllWidgets() {
        window_->removeAllWidgets();
        widgets_.clear();
    }

    void ChildWindow::moveWidgetToFront(std::shared_ptr<IWidget> widget) {
        window_->moveWidgetToFront(widget->getInternalPtr());
    }

    void ChildWindow::moveWidgetToBack(std::shared_ptr<IWidget> widget) {
        window_->moveWidgetToBack(widget->getInternalPtr());
    }

    size_t ChildWindow::moveWidgetForward(std::shared_ptr<IWidget> widget) {
        return window_->moveWidgetForward(widget->getInternalPtr());
    }

    size_t
    ChildWindow::moveWidgetBackward(std::shared_ptr<IWidget> widget) {
        return window_->moveWidgetBackward(widget->getInternalPtr());
    }

    std::shared_ptr<IWidget> ChildWindow::getFocusedWidget() const {
        auto widget = window_->getFocusedChild();
        if (widget)
            return widgets_.at(widget->getWidgetName().toAnsiString());
        return nullptr;
    }

    std::shared_ptr<IWidget> ChildWindow::getFocusedLeaf() const {
        auto widget = window_->getFocusedLeaf();
        if (widget)
            return widgets_.at(widget->getWidgetName().toAnsiString());
        return nullptr;
    }

    std::shared_ptr<IWidget> ChildWindow::getWidgetAtPosition(Vector2f pos) const {
        auto widget = window_->getWidgetAtPosition({pos.x, pos.y});
        if (widget)
            return widgets_.at(widget->getWidgetName().toAnsiString());
        return nullptr;
    }

    bool ChildWindow::focusNextWidget(bool recursive) {
        return window_->focusNextWidget(recursive);
    }

    bool ChildWindow::focusPreviousWidget(bool recursive) {
        return window_->focusPreviousWidget(recursive);
    }

    std::shared_ptr<tgui::Widget> ChildWindow::getInternalPtr() {
        return window_;
    }

    void ChildWindow::setScale(Vector2f scale) {
        setScale(scale.x, scale.y);
    }

    void ChildWindow::setOrigin(Vector2f origin) {
        setOrigin(origin.x, origin.y);
    }

    void ChildWindow::move(Vector2f offset) {
        move(offset.x, offset.y);
    }

    void ChildWindow::scale(Vector2f offset) {
        scale(offset.x, offset.y);
    }

    void ChildWindow::initEvents() {
        window_->onMouseEnter([this]{emit("mouseEnter");});
        window_->onMouseLeave([this]{emit("mouseLeave");});
        window_->onFocus([this]{emit("focus");});
        window_->onUnfocus([this]{emit("unfocus");});
        window_->onAnimationFinish([this]{emit("animationFinish");});
        window_->onMousePress([this] {emit("leftMouseDown");});
        window_->onClose([this]{emit("closed");});
        window_->onMinimize([this]{emit("minimize");});
        window_->onMaximize([this]{emit("maximize");});
        window_->onEscapeKeyPress([this]{emit("escapeKeyPress");});
        window_->onSizeChange([this](tgui::Vector2f newSize) {
            emit("sizeChange", newSize.x, newSize.y);
        });

        window_->onPositionChange([this](tgui::Vector2f newPos) {
            emit("positionChange", newPos.x, newPos.y);
        });
    }

    Vector2f ChildWindow::getScale() const {
        return {window_->getScale().x, window_->getScale().y};
    }
}