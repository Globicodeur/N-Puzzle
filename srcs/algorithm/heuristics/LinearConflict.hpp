#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <uint size>
        struct LinearConflict {

            LinearConflict(const puzzle::Puzzle<size> & goal) {
                for (uint i = 0; i < size * size; ++i)
                    indexes[goal.grid[i]] = i;
            }

            auto operator()(const puzzle::Puzzle<size> & puzzle) const {
                uint cost = 0;

                for (uint i = 0; i < size; ++i) {
                    for (uint j = 1; j < size; ++j) {
                        auto tj = puzzle.grid[(i * size) + j];
                        auto tk = puzzle.grid[(i * size) + (j - 1)];
                        auto gj = indexes[tj];
                        auto gk = indexes[tk];
                        bool sameLine = (gj / size == gk / size == i);
                        bool conflict = (gj % size < gk % size);
                        cost += (sameLine && conflict);
                    }
                }

                return cost;
            }

        private:

            uint indexes[size * size];

        };

    }

}
