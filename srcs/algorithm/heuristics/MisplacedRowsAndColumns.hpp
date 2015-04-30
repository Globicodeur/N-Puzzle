#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <uint size>
        struct MisplacedRowsAndColumns {

            MisplacedRowsAndColumns(const puzzle::Puzzle<size> & goal) {
                for (uint i = 0; i < size * size; ++i)
                    indexes[goal.grid[i]] = i;
            }

            auto operator()(const puzzle::Puzzle<size> & puzzle) const {
                uint cost = 0;

                for (uint i = 0; i < size * size; ++i) {
                    auto goalIdx = indexes[puzzle.grid[i]];
                    auto goalX = goalIdx % size;
                    auto goalY = goalIdx / size;
                    auto x = i % size;
                    auto y = i / size;
                    cost += (goalX != x) + (goalY != y);
                }

                return cost;
            }

        private:

            uint indexes[size * size];

        };

    }

}
