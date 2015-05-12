#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <uint size>
        struct ManhattanDistance {

            static constexpr std::size_t id = 0;

            ManhattanDistance(const puzzle::Puzzle<size> & goal) {
                for (uint i = 0; i < size * size; ++i)
                    indexes[goal.grid[i]] = i;
            }

            auto operator()(const puzzle::Puzzle<size> & puzzle) const {
                uint cost = 0;

                for (uint i = 0; i < size * size; ++i) {
                    if (puzzle.grid[i] == 0)
                        continue ;
                    uint gi = indexes[puzzle.grid[i]];

                    int gx = gi % size;
                    int gy = gi / size;

                    int x = i % size;
                    int y = i / size;

                    int dx = x - gx;
                    int dy = y - gy;

                    cost += std::abs(dx) + std::abs(dy);
                }

                return cost;
            }

        private:

            uint indexes[size * size];

        };

    }

}
