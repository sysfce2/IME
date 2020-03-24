/**
 * @brief Helper functions
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Graphics.hpp>
#include "gui/control/UIElement.h"

namespace Utility{
    /**
     * @brief Change the opacity of a colour
     * @param colour Colour to change opacity of
     * @param opacity New Opacity
     * @return A new colour object with the same colour as the
     *         one provided as the argument but with a different opacity
     */
    static sf::Color changeOpacity(sf::Color colour, int opacity){
        return sf::Color(colour.r, colour.g, colour.b, opacity);
    }

    /**
     * @brief Make Drawable object invisible
     * @param drawable Drawable to make invisible
     *
     * This function will make any sf:: object invisible. This means
     * that if the said instance is drawn on a render target and a call to
     * display is made, the instance will not be shown on the render target
     */
    template <class T>
    void makeInvisible(T& shape){
        shape.setFillColor(changeOpacity(shape.getFillColor(), 0));
        shape.setOutlineColor(changeOpacity(shape.getOutlineColor(), 0));
    }

    /**
     * @brief Make a sf::Drawable visible
     * @param shape Shape to make visible
     */
    template <class T>
    void makeVisible(T& shape) {
        if (shape.getFillColor() != sf::Color::Transparent)
            shape.setFillColor(changeOpacity(shape.getFillColor(), 255));
        if (shape.getOutlineColor() != sf::Color::Transparent)
            shape.setOutlineColor(changeOpacity(shape.getOutlineColor(), 255));
    }

    /**
     * @brief Convert own Colour object to SFML colour object
     * @param colour Colour to be converted to SFML colour object
     * @return SFML colour object
     *
     * This function only converts the object and not the actual
     * colour. This means that the colour after the conversion is
     * the same as the one before the conversion
     */
    static sf::Color convertOwnColourToSFMLColour(Gui::Colour color){
        return {static_cast<sf::Uint8>(color.red),
                static_cast<sf::Uint8>(color.green),
                static_cast<sf::Uint8>(color.blue),
                static_cast<sf::Uint8>(color.opacity)};
    }

    /**
     * @brief Convert SFML colour object to own colour object
     * @param SFMLColour Colour to be converted to own colour object
     * @return Own colour object
     *
     * This function only converts the object and not the actual
     * colour. This means that the colour after the conversion is
     * the same as the one before the conversion
     */
    static Gui::Colour convertSFMLColourToOwnColour(sf::Color SFMLColour){
        return {SFMLColour.r, SFMLColour.g, SFMLColour.b, SFMLColour.a};
    }
}

#endif
