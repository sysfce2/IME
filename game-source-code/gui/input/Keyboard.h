
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <unordered_map>
#include "event/EventEmitter.h"

class Keyboard{
public:
    /**
     * @brief keyboard keys
     */
    enum class Key{
        A , B, C,
        D, E, F, G,
        H, I, J, K,
        L, M, N, O,
        P, Q, R, S,
        T, U, V, W,
        X, Y, Z, Num0,
        Num1, Num2, Num3, Num4,
        Num5, Num6, Num7, Num8,
        Num9, Escape, LControl, LShift,
        LAlt, LSystem, RControl, RShift,
        RAlt, RSystem, Menu, LBracket,
        RBracket, Semicolon, Comma, Period,
        Quote, Slash, Backslash, Tilde,
        Equal, Hyphen, Space, Enter,
        Backspace, Tab, PageUp, PageDown,
        End, Home, Insert, Delete,
        Add, Subtract, Multiply, Divide,
        Left, Right, Up, Down,
        Numpad0, Numpad1, Numpad2, Numpad3,
        Numpad4, Numpad5, Numpad6, Numpad7,
        Numpad8, Numpad9, F1, F2,
        F3, F4, F5, F6,
        F7, F8, F9, F10,
        F11, F12, F13, F14,
        F15, Pause
    };

    /**
     * @brief Constructor
     */
    Keyboard();

    /**
     * @brief Check if key is pressed or not.
     * @param key Key to check.
     * @return true if key is pressed, false if it not pressed.
     *
     * This function checks the state of a key in real time
     */
    static bool isKeyPressed(Key key);

    /**
      * @brief Add an event listener to a key down event
      * @tparam Args Template parameter pack name
      * @param key Key to listen for
      * @param callback Function to execute when the key is down
      * @return Event listener's identification number
      *
      * A key down event will fire if a key is pressed and remains pressed.
      * The event will not fire If the key is pressed and immediately released
      * @see onKeyPressed(Key, Callback) function
      */
    template <typename...Args>
    static int onKeyDown(Key key, Callback<Args...> callback) {
        if (!isEventEmitterInitialized_) //Function accessed without instantiation
            initEventEmitter();
        eventEmitter_.addEventListener(std::to_string(static_cast<int>(key)) + "Down", callback);
    }

    /**
      * @brief Add an event listener to a key press event
      * @tparam Args Template parameter pack name
      * @param key Key to listen for
      * @param callback Function to execute when the key is pressed
      * @return Event listener's identification number
      *
      * A key pressed event fires when a key is pressed and immediately released
      * or after a "keyDown" event (held key is released)
      */
    template <typename...Args>
    static int onKeyPressed(Key key, Callback<Args...> callback) {
        if (!isEventEmitterInitialized_) //Function accessed without instantiation
            initEventEmitter();
        eventEmitter_.addEventListener(std::to_string(static_cast<int>(key)) + "Pressed", callback);
    }

    /**
      * @brief  Remove an event listener from an event
      * @param  event Event to remove listener from
      * @param  listenerId Identification number of the listener
      *         to be removed
      * @return True if a listener was removed from an event,
      *         false if the specified event does not have a
      *         listener with the specified id
       */
    static bool removeEventListener(const std::string& event, int callbackId){
        return eventEmitter_.removeEventListener(event, callbackId);
    }

private:
    /**
     * @brief Initialize the event emitter for event publishing
     */
    static void initEventEmitter();

private:
    //Holds the state of a key when it was last checked
    inline static std::unordered_map<Key, bool> prevKeyStateMap_{};
    //Event Emitter/publisher
    inline static EventEmitter eventEmitter_{};
    //State of the event emitter (Since class can be used without instance)
    inline static auto isEventEmitterInitialized_{false};
};

#endif