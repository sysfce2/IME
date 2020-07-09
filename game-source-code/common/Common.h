/**
 * @brief Stores shared/common definitions and/or declarations (structs, constants, events etc)
 */

#ifndef COMMON_H
#define COMMON_H

namespace Common{
    /**
     * @brief Dimensions
     */
    struct Dimensions{
        float width;
        float height;
    };

    /**
     * @brief Position
     */
    struct Position{
        float x;
        float y;
    };

    /**
     * @brief Stores all game entity types
     *
     * Any Entity object that exists in the game must also be added
     * here
     */
    enum class EntityType{
        Player //An example
    };
};

#endif