#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <uint size>
        struct MisplacedTiles {

            MisplacedTiles(const puzzle::Puzzle<size> & goal) {
                for (uint i = 0; i < size * size; ++i)
                    indexes[goal.grid[i]] = i;
            }

            auto operator()(const puzzle::Puzzle<size> & puzzle) const {
                uint cost = 0;

                for (uint i = 0; i < size * size; ++i) {
                    auto goalIdx = indexes[puzzle.grid[i]];
                    cost += goalIdx != i;
                }

                return cost;
            }

        private:

            uint indexes[size * size];

        };

    }

}
