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

#include "IME/core/physics/grid/CyclicGridMover.h"

namespace ime {
    CyclicGridMover::CyclicGridMover(TileMap &tilemap, GameObject *target) :
        GridMover(Type::Cyclic, tilemap, target),
        direction_{CycleDirection::Clockwise}
    {
        setHandlerIntakeAsInternal(true);

        ///@brief Move the target in the next possible direction
        onAdjacentMoveEnd([this](ime::Index) {
            static auto swapValues = [](const Vector2i& dir) {
                return Vector2i{dir.y, dir.x};
            };

            Vector2i currentDir = getDirection();
            if (direction_ == CycleDirection::Clockwise)
                moveTarget(getDirection(), currentDir.x == 0 ? swapValues(currentDir) * -1 : swapValues(currentDir));
            else
                moveTarget(getDirection(), std::abs(currentDir.x) == 1 ? swapValues(currentDir) * -1 : swapValues(currentDir));
        });

        setHandlerIntakeAsInternal(false);
    }

    void CyclicGridMover::setCycleDirection(CyclicGridMover::CycleDirection direction) {
        if (direction_ != direction) {
            direction_ = direction;
            emitChange(Property{"cycleDirection", direction});
        }
    }

    CyclicGridMover::CycleDirection CyclicGridMover::getCycleDirection() const {
        return direction_;
    }

    std::string CyclicGridMover::getClassName() const {
        return "WallFollowerGridMover";
    }

    void CyclicGridMover::moveTarget(const Vector2i &curDir, const Vector2i &newDir) {
        if (!isBlockedInDirection(newDir).first)
            requestDirectionChange(newDir);
        else if (!isBlockedInDirection(curDir).first)
            requestDirectionChange(curDir);
        else
            requestDirectionChange(newDir * -1);
    }
}