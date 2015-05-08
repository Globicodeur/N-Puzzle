#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <uint size>
        struct LinearConflict {

            LinearConflict(const puzzle::Puzzle<size> & goal) {
                for (uint i = 0; i < size * size; ++i)
                    goalIndexes[goal.grid[i]] = i;
            }

            auto operator()(const puzzle::Puzzle<size> & puzzle) const {
                uint cost = 0;

                uint currentIndexes[size * size];

                for (uint i = 0; i < size * size; ++i)
                    currentIndexes[puzzle.grid[i]] = i;

                std::unordered_set<uint> rows;
                std::unordered_set<uint> cols;
                for (uint i = 1; i < size * size; ++i) {
                    uint ipx = currentIndexes[i] % size;
                    uint ipy = currentIndexes[i] / size;
                    uint igx = goalIndexes[i] % size;
                    uint igy = goalIndexes[i] / size;
                    if (rows.count(i) == 0) {
                        for (uint x = 0; x < size; ++x) {
                            uint j = puzzle.grid[ipy * size + x];
                            if (i != j && rows.count(j) == 0) {
                                uint jpx = currentIndexes[j] % size;
                                uint jpy = currentIndexes[j] / size;
                                uint jgx = goalIndexes[j] % size;
                                uint jgy = goalIndexes[j] / size;
                                if (ipy == igy == jpy == jgy && ((ipx < jpx && igx > jgx) || (ipx > jpx && igx < jgx))) {
                                    rows.insert(i);
                                    rows.insert(j);
                                    cost += 2;
                                }
                            }
                        }
                    }
                    if (cols.count(i) == 0) {
                        for (uint y = 0; y < size; ++y) {
                            uint j = puzzle.grid[y * size + ipx];
                            if (i != j && cols.count(j) == 0) {
                                uint jpx = currentIndexes[j] % size;
                                uint jpy = currentIndexes[j] / size;
                                uint jgx = goalIndexes[j] % size;
                                uint jgy = goalIndexes[j] / size;
                                if (ipx == igx == jpx == jgx && ((ipy < jpy && igy > jgy) || (ipy > jpy && igy < jgy))) {
                                    cols.insert(i);
                                    cols.insert(j);
                                    cost += 2;
                                }
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
