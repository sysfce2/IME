#include "UIElement.h"
#include "resources/ResourceManager.h"
#include <algorithm>

Gui::UIElement::UIElement(const std::string &content, const std::string &font, unsigned int textCharSize)
    : numOfLinesInString_{0u}
{
    //Resize element when text changes
    eventEmitter_.addListener("textChanged", Callback<std::string>([this](const std::string &content) {
        if (content.empty()) {
            numOfLinesInString_ = 0;
            return;
        }
        numOfLinesInString_ = std::count(content.begin(), content.end(), '\n');
        numOfLinesInString_++; //Account for one line/last line (both dont have terminating character)
        resize();
    }));
    //Resize element when the character size changes
    eventEmitter_.addListener("charSizeChanged", Callback<>([this](){
        resize();
    }));
    //Update element on dimension changes (padding and margin changes)
    eventEmitter_.addListener("dimensionsChanged", Callback<>([this](){
        resize();
        setPosition(getPosition().x, getPosition().y);
    }));

    setTextFont(font);
    setTextCharSize(textCharSize);
    setText(content);
    border_.setOutlineThickness(0.6f);
    border_.setOutlineColor(sf::Color::Transparent);
    parentRectangle_.setFillColor(sf::Color::Transparent);
    border_.setFillColor(sf::Color::Black);
    contentRectangle_.setFillColor(sf::Color::Transparent);
    textContent_.setFillColor(sf::Color::White);
}

void Gui::UIElement::setPosition(float x, float y) {
    parentRectangle_.setPosition(x, y);
    border_.setPosition(
      	parentRectangle_.getPosition().x + margin_.left,
      	parentRectangle_.getPosition().y + margin_.top
    );
    contentRectangle_.setPosition(
      	border_.getPosition().x + padding_.left,
      	border_.getPosition().y + padding_.top
    );
    textContent_.setPosition(
      	contentRectangle_.getPosition().x
        + contentRectangle_.getGlobalBounds().width / 2.0f
		- textContent_.getGlobalBounds().width / 2.0f,
        contentRectangle_.getPosition().y
      	+ contentRectangle_.getGlobalBounds().height / 2.0f
        - textContent_.getGlobalBounds().height / 2.0f
	);
}

void Gui::UIElement::setMargin(float margin) {
    margin_.left = margin_.right = margin_.bottom = margin_.top = margin;
    eventEmitter_.emit("dimensionsChanged");
}

void Gui::UIElement::setPadding(float padding) {
    padding_.left = padding_.right = padding_.top = padding_.bottom = padding;
    eventEmitter_.emit("dimensionsChanged");
}

void Gui::UIElement::setMargin(const Gui::Margin &margin) {
    margin_ = margin;
    eventEmitter_.emit("dimensionsChanged");
}

void Gui::UIElement::setPadding(const Gui::Padding &padding) {
    padding_ = padding;
    eventEmitter_.emit("dimensionsChanged");
}

void Gui::UIElement::setFillColour(Gui::Colour fillColour) {
    border_.setFillColor(sf::Color(
        fillColour.red, fillColour.green,
        fillColour.blue, fillColour.opacity
    ));
}

void Gui::UIElement::setTextFont(const std::string &contentFont) {
    textContent_.setFont(ResourceManager::getFont(contentFont));
}

void Gui::UIElement::setTextCharSize(unsigned int charSize) {
    textContent_.setCharacterSize(textContent_.getFont()->getGlyph(
        L'A', charSize, false).bounds.height
    );
    eventEmitter_.emit("charSizeChanged");
}

void Gui::UIElement::setText(const std::string &content) {
    textContent_.setString(content);
    eventEmitter_.emit("textChanged");
}

void Gui::UIElement::setTextFillColour(Gui::Colour textFillColour) {
    textContent_.setFillColor(sf::Color(
        textFillColour.red, textFillColour.green,
        textFillColour.blue, textFillColour.opacity
    ));
}

Position Gui::UIElement::getPosition() const {
    return Position{
        parentRectangle_.getPosition().x,
        parentRectangle_.getPosition().y
    };
}

Dimensions Gui::UIElement::getDimensions() const {
    return Dimensions{
        static_cast<unsigned int>(parentRectangle_.getGlobalBounds().width),
        static_cast<unsigned int>(parentRectangle_.getGlobalBounds().height)
    };
}

bool Gui::UIElement::contains(float x, float y) const{
    return (x >= border_.getPosition().x && x <= border_.getPosition().x +
        border_.getGlobalBounds().width) && (y >= border_.getPosition().y &&
        y <= border_.getPosition().y + border_.getGlobalBounds().height);
}

void Gui::UIElement::draw(Gui::Window &renderTarget) {
    renderTarget.draw(parentRectangle_);
    renderTarget.draw(border_);
    renderTarget.draw(contentRectangle_);
    renderTarget.draw(textContent_);
}

void Gui::UIElement::resize() {
    auto textHeight = (textContent_.getCharacterSize() * numOfLinesInString_ <
                      textContent_.getGlobalBounds().height) ?
                      textContent_.getGlobalBounds().height :
                      textContent_.getCharacterSize() * numOfLinesInString_;

    contentRectangle_.setSize(
        sf::Vector2f(textContent_.getGlobalBounds().width, textHeight
    ));
    border_.setSize(sf::Vector2f(
        contentRectangle_.getGlobalBounds().width + padding_.left + padding_.right,
        contentRectangle_.getGlobalBounds().height + padding_.top + padding_.bottom
    ));
    parentRectangle_.setSize(sf::Vector2f(
		border_.getGlobalBounds().width + margin_.left + margin_.right,
        border_.getGlobalBounds().height + margin_.top + margin_.bottom
    ));
}

Gui::UIElement::~UIElement() = default;
