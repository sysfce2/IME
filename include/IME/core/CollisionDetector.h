#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "IME/core/tilemap/Tile.h"

namespace IME {
    class CollisionDetector{
    public:
        /**
         * @brief Check for collision between two tiles
         * @param tileOne The first tile
         * @param tileTwo The second tile
         * @param callback Function to execute when a collision is detected
         *
         * The callback function is not invoked when a collision is not taking
         * place. When a collision is taking place, the colliding tiles will be
         * passed as arguments to the callback in the same order in which they
         * are provided to this function.
         */
        void TileVSTileCollision(Tile& tileOne, Tile& tileTwo, Callback<Tile&, Tile&> callback);

    private:
        /**
         * @brief Check if two objects are colliding using the AABB method
         * @param objectOne The
         * @param objectTwo
         * @return True if the two objects are colliding, otherwise false
         */
        template<typename T1, typename T2>
        bool isCollideAABB(const T1& objectOne, const T2& objectTwo) const {
            return objectOne.getPosition().x + objectOne.getSize().width >= objectTwo.getPosition().x
                && objectOne.getPosition().x <= objectTwo.getPosition().x + objectTwo.getSize().width
                && objectOne.getPosition().y + objectOne.getSize().height >= objectTwo.getPosition().y
                && objectOne.getPosition().y <= objectTwo.getPosition().y + objectTwo.getSize().height;
        }

    private:

    };
} // namespace IME

#endif
