#include "IME/graphics/ui/widgets/ProgressBar.h"
#include "IME/core/managers/ResourceManager.h"
#include "IME/utility/Helpers.h"
#include <TGUI/GuiBase.hpp>
#include <cassert>

namespace IME::Graphics::UI {
    ProgressBar::ProgressBar()
        : progressBar_{tgui::ProgressBar::create()},
          renderer_{std::make_shared<ProgressBarRenderer>()}
    {
        renderer_->setInternalPtr(progressBar_->getRenderer());
        initEvents();
    }

    void ProgressBar::setRenderer(std::shared_ptr<ProgressBarRenderer> renderer) {
        assert(renderer && "A nullptr cannot be set as a renderer");
        renderer_ = renderer;
        progressBar_->setRenderer(renderer->getInternalPtr()->getData());
    }

    std::shared_ptr<ProgressBarRenderer> ProgressBar::getRenderer() {
        return renderer_;
    }

    void ProgressBar::setMinimumValue(unsigned int minValue) {
        progressBar_->setMinimum(minValue);
    }

    unsigned int ProgressBar::getMinimumValue() const {
        return progressBar_->getMinimum();
    }

    void ProgressBar::setMaximumValue(unsigned int maxValue) {
        progressBar_->setMaximum(maxValue);
    }

    unsigned int ProgressBar::getMaximumValue() const {
        return progressBar_->getMaximum();
    }

    void ProgressBar::setValue(unsigned int value) {
        progressBar_->setValue(value);
    }

    unsigned int ProgressBar::getValue() const {
        return progressBar_->getValue();
    }

    unsigned int ProgressBar::incrementValue() {
        return progressBar_->incrementValue();
    }

    void ProgressBar::setFillDirection(ProgressBar::FillDirection direction) {
        progressBar_->setFillDirection(
            static_cast<tgui::ProgressBar::FillDirection>(direction));
    }

    ProgressBar::FillDirection ProgressBar::getFillDirection() const {
        return static_cast<FillDirection>(progressBar_->getFillDirection());
    }

    void ProgressBar::setPosition(float x, float y) {
        progressBar_->setPosition({x, y});
    }

    void ProgressBar::setRotation(float angle) {
        progressBar_->setRotation(angle);
    }

    void ProgressBar::setScale(float factorX, float factorY) {
        progressBar_->setScale({factorX, factorY});
    }

    void ProgressBar::setOrigin(float x, float y) {
        progressBar_->setOrigin({x, y});
    }

    Position ProgressBar::getPosition() const {
        return {progressBar_->getPosition().x, progressBar_->getPosition().y};
    }

    Position ProgressBar::getOrigin() const {
        return {progressBar_->getOrigin().x, progressBar_->getOrigin().y};
    }

    float ProgressBar::getRotation() const {
        return progressBar_->getRotation();
    }

    void ProgressBar::move(float xOffset, float yOffset) {
        progressBar_->setPosition(getPosition().x + xOffset,
                             getPosition().y + yOffset);
    }

    void ProgressBar::rotate(float offset) {
        progressBar_->setRotation(progressBar_->getRotation() + offset);
    }

    void ProgressBar::scale(float factorX, float factorY) {
        progressBar_->setScale({progressBar_->getScale().x + factorX,
                           progressBar_->getScale().y + factorY});
    }

    void ProgressBar::hide() {
        progressBar_->hideWithEffect(tgui::ShowAnimationType::Fade,
                                fadeAnimDuration_);
    }

    void ProgressBar::show() {
        progressBar_->showWithEffect(tgui::ShowAnimationType::Fade,
                                fadeAnimDuration_);
    }

    bool ProgressBar::isHidden() const {
        return !progressBar_->isVisible();
    }

    void ProgressBar::setEnabled(bool isEnable) {
        progressBar_->setEnabled(isEnable);
    }

    bool ProgressBar::isEnabled() const {
        return progressBar_->isEnabled();
    }

    void ProgressBar::toggleEnabled() {
        setEnabled(!isEnabled());
    }

    bool ProgressBar::isMouseOverElement() const {
        //@todo
        return false;
    }

    void ProgressBar::setPosition(Position position) {
        setPosition(position.x, position.y);
    }

    void ProgressBar::setTextSize(unsigned int charSize) {
        progressBar_->setTextSize(charSize);
    }

    void ProgressBar::setText(const std::string &content) {
        progressBar_->setText(content);
    }

    void ProgressBar::setSize(float width, float height) {
        progressBar_->setSize({width, height});
    }

    Dimensions ProgressBar::getSize() const {
        return {progressBar_->getSize().x, progressBar_->getSize().y};
    }

    std::string ProgressBar::getText() const {
        return progressBar_->getText().toAnsiString();
    }

    unsigned int ProgressBar::getTextSize() const {
        return progressBar_->getTextSize();
    }

    std::string ProgressBar::getType() const {
        return "ProgressBar";
    }

    void ProgressBar::toggleVisibility() {
        progressBar_->setVisible(!progressBar_->isVisible());
    }

    bool ProgressBar::contains(float x, float y) const {
        return progressBar_->isMouseOnWidget({x, y});
    }

    void ProgressBar::setFocused(bool isFocused) {
        progressBar_->setFocused(isFocused);
    }

    bool ProgressBar::isFocused() const {
        return progressBar_->isFocused();
    }

    std::shared_ptr<tgui::Widget> ProgressBar::getInternalPtr() {
        return progressBar_;
    }

    Dimensions ProgressBar::getAbsoluteSize() {
        return {progressBar_->getFullSize().x, progressBar_->getFullSize().y};
    }

    void ProgressBar::initEvents() {
        progressBar_->onMouseEnter([this]{emit("mouseEnter");});
        progressBar_->onMouseLeave([this]{emit("mouseLeave");});
        progressBar_->onFocus([this]{emit("focus");});
        progressBar_->onUnfocus([this]{emit("unfocus");});
        progressBar_->onAnimationFinish([this]{emit("animationFinish");});
        progressBar_->onFull([this]{emit("full");});

        progressBar_->onValueChange([this](unsigned int newValue) {
            emit("valueChange", newValue);
        });

        progressBar_->onSizeChange([this](tgui::Vector2f newSize) {
            emit("sizeChange", newSize.x, newSize.y);
        });

        progressBar_->onPositionChange([this](tgui::Vector2f newPos) {
            emit("positionChange", newPos.x, newPos.y);
        });

        //Events triggered by left mouse button
        progressBar_->onClick([this](tgui::Vector2f mousePos){
            emit("click");
            emit("click", mousePos.x, mousePos.y);
        });

        progressBar_->onMousePress([this](tgui::Vector2f mousePos) {
            emit("leftMouseDown");
            emit("leftMouseDown", mousePos.x, mousePos.y);
        });

        progressBar_->onMouseRelease([this](tgui::Vector2f mousePos) {
            emit("leftMouseUp");
            emit("leftMouseUp", mousePos.x, mousePos.y);
        });

        //Events triggered by right mouse button
        progressBar_->onRightMousePress([this](tgui::Vector2f mousePos){
            emit("rightMouseDown");
            emit("rightMouseDown", mousePos.x, mousePos.y);
        });

        progressBar_->onRightMouseRelease([this](tgui::Vector2f mousePos){
            emit("rightMouseUp");
            emit("rightMouseUp", mousePos.x, mousePos.y);
        });

        progressBar_->onRightClick([this](tgui::Vector2f mousePos){
            emit("rightClick");
            emit("rightClick", mousePos.x, mousePos.y);
        });
    }
}