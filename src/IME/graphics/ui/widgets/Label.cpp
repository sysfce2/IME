#include "IME/graphics/ui/widgets/Label.h"
#include "IME/core/managers/ResourceManager.h"
#include "IME/utility/Helpers.h"
#include <TGUI/GuiBase.hpp>
#include <cassert>

namespace IME::Graphics::UI {
    Label::Label() : Label("")
    {}

    Label::Label(const std::string &text) : label_{tgui::Label::create(text)},
                                            renderer_{std::make_shared<LabelRenderer>()}
    {
        renderer_->setInternalPtr(label_->getRenderer());
    }

    std::shared_ptr<LabelRenderer> Label::getRenderer() {
        return renderer_;
    }

    void Label::setRenderer(std::shared_ptr<LabelRenderer> renderer) {
        assert(renderer && "A nullptr cannot be set as a renderer");
        renderer_ = renderer;
        label_->setRenderer(renderer->getInternalPtr()->getData());
    }

    std::string Label::getType() const {
        return "Label";
    }

    void Label::setPosition(float x, float y) {
        label_->setPosition({x, y});
    }

    void Label::setPosition(Position position) {
        setPosition(position.x, position.y);
    }

    void Label::setRotation(float angle) {
        label_->setRotation(angle);
    }

    void Label::setScale(float factorX, float factorY) {
        label_->setScale({factorX, factorY});
    }

    void Label::setOrigin(float x, float y) {
        label_->setOrigin({x, y});
    }

    Position Label::getPosition() const {
        return {label_->getPosition().x, label_->getPosition().y};
    }

    Position Label::getOrigin() const {
        return {label_->getOrigin().x, label_->getOrigin().y};
    }

    float Label::getRotation() const {
        return label_->getRotation();
    }

    void Label::move(float xOffset, float yOffset) {
        label_->setPosition(getPosition().x + xOffset,
            getPosition().y + yOffset);
    }

    void Label::rotate(float offset) {
        label_->setRotation(label_->getRotation() + offset);
    }

    void Label::scale(float factorX, float factorY) {
        label_->setScale({label_->getScale().x + factorX,
            label_->getScale().y + factorY});
    }

    void Label::hide() {
        label_->hideWithEffect(tgui::ShowAnimationType::Fade,
            fadeAnimDuration_);
    }

    void Label::show() {
        label_->showWithEffect(tgui::ShowAnimationType::Fade,
            fadeAnimDuration_);
    }

    bool Label::isHidden() const {
        return !label_->isVisible();
    }

    void Label::setTextSize(unsigned int charSize) {
        label_->setTextSize(charSize);
    }

    void Label::setText(const std::string &content) {
        label_->setText(content);
    }

    void Label::setSize(float width, float height) {
        label_->setSize({width, height});
    }

    Dimensions Label::getSize() const {
        return {label_->getSize().x, label_->getSize().y};
    }

    Dimensions Label::getAbsoluteSize() {
        return {label_->getFullSize().x, label_->getFullSize().y};;
    }

    std::string Label::getText() const {
        return label_->getText().toAnsiString();
    }

    unsigned int Label::getTextSize() const {
        return label_->getTextSize();
    }

    void Label::toggleVisibility() {
        label_->setVisible(!label_->isVisible());
    }

    bool Label::contains(float x, float y) const {
        return label_->isMouseOnWidget({x, y});;
    }

    std::shared_ptr<tgui::Widget> Label::getInternalPtr() {
        return label_;
    }
}