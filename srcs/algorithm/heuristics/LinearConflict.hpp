#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <uint size>
        struct LinearConflict {

            static constexpr std::size_t id = 1;

            LinearConflict(const puzzle::Puzzle<size> & goal) {
                for (uint i = 0; i < size * size; ++i)
                    goalIndexes[goal.grid[i]] = i;
            }

            auto operator()(const puzzle::Puzzle<size> & puzzle) const {
                uint cost = 0;

                using set = bool [size * size];
                set rows{}, cols{};

                rows[0] = true;
                cols[0] = true;
                for (uint i = 0; i < size; ++i) {
                    // line
                    for (uint j = 0; j < size; ++j) {
                        for (uint k = j + 1; k < size; ++k) {
                            auto tj = puzzle.grid[i * size + j];
                            auto tk = puzzle.grid[i * size + k];
                            if (rows[tj] || rows[tk])
                                continue;
                            auto gj = goalIndexes[tj];
                            auto gk = goalIndexes[tk];
                            if (i == gj / size && i == gk / size && gj > gk) {
                                rows[i] = true;
                                rows[j] = true;
                                cost += 2;
                            }
                        }
                    }

                    for (uint j = 0; j < size; ++j) {
                        for (uint k = j + 1; k < size; ++k) {
                            auto tj = puzzle.grid[j * size + i];
                            auto tk = puzzle.grid[k * size + i];
                            if (rows[tj] || rows[tk])
                                continue;
                            auto gj = goalIndexes[tj];
                            auto gk = goalIndexes[tk];
                            if (i == gj % size && i == gk % size && gj > gk) {
                                cols[i] = true;
                                cols[j] = true;
                                cost += 2;
                            }
                        }
                    }
                }

                return cost;
            }

        private:
            uint goalIndexes[size * size];

        };

    }

}
