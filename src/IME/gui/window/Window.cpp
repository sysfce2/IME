#include "IME/gui/window/Window.h"
#include "IME/utility/Helpers.h"
#include "IME/core/managers/ResourceManager.h"
#include <SFML/Window/Event.hpp>
#include <cassert>

namespace IME::Gui {
    Dimensions Window::dimensions_{0u, 0u};

    Window::Window() {
        assert(!isInstantiated_ && "Only a single instance of Window can be instantiated");
        isInstantiated_ = true;
    }

    void Window::create(const std::string& title, float width, float height, Style style) {
        assert(width >= 0.0f && "Window width cannot be negative");
        assert(height >= 0.0f && "Window height cannot be negative");
        dimensions_ = {width, height};
        window_.create(sf::VideoMode(static_cast<unsigned int>(width),
            static_cast<unsigned int>(height)), title, static_cast<unsigned int>(style)
        );
    }

    bool Window::setIcon(const std::string &filename) {
        try {
            auto icon = ResourceManager::getInstance()->getImage(filename);
            window_.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
            return true;
        } catch (...) {} // Use current icon, If not set prior, OS icon will be used
        return false;
    }

    void Window::setFramerateLimit(unsigned int framerateLimit) {
        window_.setFramerateLimit(framerateLimit);
    }

    void Window::setCursorType(Window::CursorType cursorType) {
        auto static cursor = sf::Cursor();
        if (cursor.loadFromSystem(static_cast<sf::Cursor::Type>(cursorType)))
            window_.setMouseCursor(cursor);
    }

    bool Window::isOpen() const{
        return window_.isOpen();
    }

    bool Gui::Window::pollEvent(sf::Event& event){
        return window_.pollEvent(event);
    }

    void Window::close() {
        window_.close();
    }

    void Window::display() {
        window_.display();
    }

    void Window::clear(Colour colour) {
        window_.clear(Utility::convertTo3rdPartyColour(colour));
    }

    void Window::draw(const sf::Drawable &drawable) {
        window_.draw(drawable);
    }

    void Window::draw(IDrawable &drawable) {
        drawable.draw(*this);
    }

    Window::~Window() {
        isInstantiated_ = false;
    }

    Dimensions Window::getDimensions() {
        return dimensions_;
    }
}
