#include "IME/graphics/ui/layout/ChildWindow.h"
#include "IME/utility/Helpers.h"
#include <cassert>

namespace IME::Graphics::UI {
    ChildWindow::ChildWindow(const std::string &title, unsigned int titleButtons)
        : window_{tgui::ChildWindow::create(title, titleButtons)},
          renderer_{std::make_shared<ChildWindowRenderer>()}
    {
        renderer_->setInternalPtr(window_->getRenderer());
        initEvents();
    }

    void ChildWindow::setRenderer(std::shared_ptr<ChildWindowRenderer> renderer) {
        assert(renderer && "A nullptr cannot be set as a renderer");
        renderer_ = renderer;
        window_->setRenderer(renderer->getInternalPtr()->getData());
    }

    std::shared_ptr<ChildWindowRenderer> ChildWindow::getRenderer() {
        return renderer_;
    }

    void ChildWindow::setClientSize(Dimensions size) {
        window_->setClientSize({size.width, size.height});
    }

    Dimensions ChildWindow::getClientSize() const {
        return {window_->getClientSize().x, window_->getClientSize().y};
    }

    void ChildWindow::setMaximumSize(Dimensions size) {
        window_->setMaximumSize({size.width, size.height});
    }

    Dimensions ChildWindow::getMaximumSize() const {
        return {window_->getMaximumSize().x, window_->getMaximumSize().y};
    }

    void ChildWindow::setMinimumSize(Dimensions size) {
        window_->setMinimumSize({size.width, size.height});
    }

    Dimensions ChildWindow::getMinimumSize() const {
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
        window_->setPositionLocked(draggable);
    }

    bool ChildWindow::isDraggable() const {
        return window_->isPositionLocked();
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

    void ChildWindow::setText(const std::string &content) {

    }

    void ChildWindow::setSize(float width, float height) {
        window_->setSize({width, height});
    }

    Dimensions ChildWindow::getSize() const {
        return {window_->getSize().x, window_->getSize().y};
    }

    Dimensions ChildWindow::getAbsoluteSize() {
        return {window_->getFullSize().x, window_->getFullSize().y};
    }

    std::string ChildWindow::getText() const {
        return "";
    }

    unsigned int ChildWindow::getTextSize() const {
        return window_->getTextSize();
    }

    std::string ChildWindow::getType() const {
        return "ChildWindow";
    }

    void ChildWindow::toggleVisibility() {
        if (isHidden())
            show();
        else
            hide();
    }

    bool ChildWindow::contains(float x, float y) const {
        return window_->isMouseOnWidget({x, y});
    }

    void ChildWindow::hide() {
        window_->hideWithEffect(tgui::ShowAnimationType::Fade, fadeAnimDuration_);
    }

    void ChildWindow::show() {
        window_->showWithEffect(tgui::ShowAnimationType::Fade, fadeAnimDuration_);
    }

    bool ChildWindow::isHidden() const {
        return !window_->isVisible();
    }

    void ChildWindow::setPosition(float x, float y) {
        window_->setPosition({x, y});
    }

    void ChildWindow::setPosition(Position position) {
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

    Position ChildWindow::getPosition() const {
        return {window_->getPosition().x, window_->getPosition().y};
    }

    Position ChildWindow::getOrigin() const {
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

    bool ChildWindow::addWidget(std::shared_ptr<IWidget> widgetPtr,
        const std::string &widgetName)
    {
        assert(widgetPtr && "Cannot add null widget to Child window container");
        if (widgets_.insert({widgetName, widgetPtr}).second) {
            window_->add(widgetPtr->getInternalPtr(), widgetName);
            return true;
        }
        return false;
    }

    std::shared_ptr<IWidget>
    ChildWindow::getWidget(const std::string &widgetName) const {
        if (Utility::findIn(widgets_, widgetName))
            return widgets_.at(widgetName);
        return nullptr;
    }

    const std::vector<IWidget> &ChildWindow::getWidgets() const {

    }

    bool ChildWindow::removeWidget(const std::string &widget) {
        if (Utility::findIn(widgets_, widget)) {
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

    std::shared_ptr<IWidget> ChildWindow::getWidgetAtPosition(Position pos) const {
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
}