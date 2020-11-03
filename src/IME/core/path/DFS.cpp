#include "IME/core/path/DFS.h"

namespace IME {
    DFSPathFinder::DFSPathFinder(Dimensions gridSize) {
        for (auto i = 0; i < gridSize.height; i++) {
            auto innerVector = std::vector<bool>{};
            for (auto j = 0; j < gridSize.width; j++)
                innerVector.push_back(false);
            visited_.push_back(innerVector);
        }
    }

    std::stack<Index>
    DFSPathFinder::findPath(TileMap &grid, Index sourceTile, Index targetTile) {
        if (sourceTile == targetTile)
            return std::stack<Index>{};
        adjacencyList_.generateFrom(grid);
        auto exploredPath = std::vector<Node>{};
        auto nodesToVisit = std::stack<Node>();
        nodesToVisit.push({sourceTile, sourceTile});
        while (!nodesToVisit.empty()) {
            auto node = nodesToVisit.top();
            visited_[node.index.row][node.index.colm] = true;
            nodesToVisit.pop();
            if (node.index == targetTile) {
                exploredPath.push_back(node);
                break;
            } else {
                exploredPath.push_back(node);
                for (auto& neighbour : adjacencyList_.getNeighbours(node.index)) {
                    if (!visited_[neighbour.row][neighbour.colm])
                        nodesToVisit.push({node.index, neighbour});
                }
            }
        }

        reset();
        if (exploredPath.back().index == targetTile) { //Found target
            auto path = std::stack<Index>{};
            backtrack(exploredPath, path);
            return path;
        } else
            return std::stack<Index>{};
    }

    void DFSPathFinder::reset() {
        for (auto i = 0; i < visited_.size(); i++) {
            for (auto j = 0; j < visited_[0].size(); j++)
                visited_[i][j] = false;
        }
    }

    std::string DFSPathFinder::getType() const {
        return "DFS";
    }
}
