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

#include "IME/core/input/Mouse.h"
#include "IME/core/event/Event.h"
#include <SFML/Window/Mouse.hpp>

namespace ime::input {
    bool Mouse::isButtonPressed(Button button){
        return sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(button));
    }

    int Mouse::onButtonUp(Callback<Mouse::Button, int, int> callback) {
        return eventEmitter_.on("mouseUp", std::move(callback));
    }

    int Mouse::onButtonUp(Callback<Mouse::Button> callback) {
        return eventEmitter_.on("mouseUp", std::move(callback));
    }

    int Mouse::onButtonDown(Callback<Mouse::Button, int, int> callback) {
        return eventEmitter_.on("mouseDown", std::move(callback));
    }

    int Mouse::onButtonDown(Callback<Mouse::Button> callback) {
        return eventEmitter_.on("mouseDown", std::move(callback));
    }

    int Mouse::onMouseMove(Callback<int, int> callback) {
        return eventEmitter_.on("mouseMove", std::move(callback));
    }

    int Mouse::onWheelScroll(Callback<Mouse::Wheel, float, int, int> callback) {
        return eventEmitter_.on("mouseWheelScroll", std::move(callback));
    }

    bool Mouse::unsubscribe(MouseEvent event, int id) {
        switch (event) {
            case MouseEvent::MouseDown:
                return eventEmitter_.removeEventListener("mouseDown", id);
            case MouseEvent::MouseUp:
                return eventEmitter_.removeEventListener("mouseUp", id);
            case MouseEvent::MouseMove:
                return eventEmitter_.removeEventListener("mouseMove", id);
            case MouseEvent::MouseWheelScroll:
                return eventEmitter_.removeEventListener("mouseWheelScroll", id);
            default:
                return false;
        }
    }

    void Mouse::handleEvent(Event event) {
        switch (event.type) {
            case Event::MouseWheelScrolled:
                eventEmitter_.emit("mouseWheelScroll", event.mouseWheelScroll.wheel,
                    event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
                break;
            case Event::MouseButtonPressed:
                eventEmitter_.emit("mouseDown", event.mouseButton.button);
                eventEmitter_.emit("mouseDown", event.mouseButton.button,
                    event.mouseButton.x, event.mouseButton.y);
                break;
            case Event::MouseButtonReleased:
                eventEmitter_.emit("mouseUp", event.mouseButton.button);
                eventEmitter_.emit("mouseUp", event.mouseButton.button,
                    event.mouseButton.x, event.mouseButton.y);
                break;
            case Event::MouseMoved:
                eventEmitter_.emit("mouseMove", event.mouseMove.x, event.mouseMove.y);
                break;
            default:
                break;
        }
    }
}
