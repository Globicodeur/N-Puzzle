#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <uint size>
        struct MisplacedTiles {

            static constexpr std::size_t id = 2;

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
