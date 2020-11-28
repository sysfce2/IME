////////////////////////////////////////////////////////////////////////////////
// IME - Infinite Motion Engine
//
// Copyright (c) 2020 Kwena Mashamaite (kmash.ime@gmail.com)
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

/**
 * @brief Class for moving an entity to a specific position in the grid
 */

#ifndef IME_TARGETGRIDMOVER_H
#define IME_TARGETGRIDMOVER_H

#include "GridMover.h"
#include "IME/core/path/IGridPathFinder.h"

namespace IME {
    class IME_API TargetGridMover : public GridMover {
    public:
        /**
         * @brief Create a random grid mover object
         * @param tileMap Grid to target in
         * @param target Entity to be moved in the grid
         *
         * The target must be placed in the grid prior to grid mover construction
         */
        explicit TargetGridMover(TileMap &tileMap, EntityPtr target = nullptr);

        /**
         * @brief Set the path finder
         * @param pathFinder New path finder
         *
         * The default path finder is Breadth First Search (BFSPathFinder)
         */
        void setPathFinder(std::unique_ptr<IGridPathFinder> pathFinder);

        /**
         * @brief Set the index of the tile the target should go to
         *
         * The specified index must be within the the bounds of the grid and
         * the tile at index must be reachable from the targets current tile,
         * otherwise, the target will not move, since it cannot establish a
         * path to the destination
         */
        void setDestination(Index index);

        /**
         * @brief Set the position the target should go to
         * @param position New target position
         *
         * The specified position must be within the grid and the tile at that
         * position must be reachable from the targets current tile, otherwise
         * the target will not move since it cannot establish a path to the
         * destination
         */
        void setDestination(Vector2f position);

        /**
         * @brief Get the destination position of the target
         * @return The position that the target must reach
         *
         * This destination will be returned event if the target has reached it
         */
        Index getDestination() const;

        /**
         * @brief Start moving the target to its destination tile
         *
         * This function has no effect if the targets movement is not
         * stopped
         *
         * @see stopMovement
         */
        void startMovement();

        /**
         * @brief Stop the targets movement
         *
         * Since a child in the grid can never be in between tiles, the
         * targets movement will be stopped after it completes its current
         * move. The targets movement is stopped by default
         *
         * @see startMovement
         */
        void stopMovement();

        /**
         * @brief Adaptively avoid solid tiles and obstacles
         * @param isAdaptive True to enable, otherwise false
         *
         * When enabled the target will adaptively avoid collisions with
         * solid tiles and obstacles. The targets path is updated every
         * time it moves from one tile to the next. This makes the target
         * aware of tile state changes before it reaches them
         *
         * When disabled, the target will continue moving in its current
         * path to the destination until it either collides with something
         * or reaches the destination. The targets path is updated each
         * time the destination changes. Therefore if it remains fixed the
         * target will not know of any tile state changes until it gets to
         * that tile
         *
         * Adaptive movement is disabled by default
         */
        void enableAdaptiveMovement(bool isAdaptive);

        /**
         * @brief Add an event listener to a destination reached event
         * @param callback Function to execute when the target reaches its
         *          destination tile
         *
         * This event is fired when the target reaches the final target tile.
         * To get notified as the target progresses towards the destination
         * @see onAdjacentTileReached
         *
         * The callback is passed the destination tile of the target after it
         * reaches it
         */
        int onDestinationReached(Callback<Graphics::Tile> callback);

    private:
        /**
         * @brief Generates the targets new direction of motion based on the
         *         next target position
         * @param nextPos The next target position
         */
        void generateNewDirOfMotion(Index nextPos);

        /**
         * @brief Generate path to target
         */
        void generatePath();

        /**
         * @brief Move the target
         */
        void moveTarget();

    private:
        //Finds the path from the source to the target
        std::unique_ptr<IGridPathFinder> pathFinder_;
        //Target tile position
        Index targetTileIndex_;
        //Stores the path to the target tile
        std::stack<Index> pathToTargetTile_;
        int obstacleHandlerId_;
        int solidTileHandlerId_;
        Callback<> adjacentTileHandler_;
        //Flags whether the target has been stopped or not
        bool movementStarted_;
        //Flags whether the target tile was changed while target in motion
        bool targetTileChangedWhileMoving_;
    };
}

#endif
