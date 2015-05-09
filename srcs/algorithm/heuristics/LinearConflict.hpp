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

                for (uint i = 0; i < size; ++i) {
                    // line
                    for (uint j = 0; j < size; ++j) {
                        for (uint k = j + 1; k < size; ++k) {
                            auto tj = puzzle.grid[i * size + j];
                            auto tk = puzzle.grid[i * size + k];
                            if (!tj || !tk)
                                continue;
                            auto gj = goalIndexes[tj];
                            auto gk = goalIndexes[tk];
                            if (i == gj / size && i == gk / size && gj > gk)
                                cost += 2;
                        }
                    }
                }

                return cost;

                // uint currentIndexes[size * size];

                // for (uint i = 0; i < size * size; ++i)
                //     currentIndexes[puzzle.grid[i]] = i;

                // std::unordered_set<uint> rows;
                // std::unordered_set<uint> cols;
                // for (uint i = 1; i < size * size; ++i) {
                //     uint ipx = currentIndexes[i] % size;
                //     uint ipy = currentIndexes[i] / size;
                //     uint igx = goalIndexes[i] % size;
                //     uint igy = goalIndexes[i] / size;
                //     if (rows.count(i) == 0) {
                //         for (uint x = 0; x < size; ++x) {
                //             uint j = puzzle.grid[ipy * size + x];
                //             if (j != 0 && i != j && rows.count(j) == 0) {
                //                 uint jpx = currentIndexes[j] % size;
                //                 uint jpy = currentIndexes[j] / size;
                //                 uint jgx = goalIndexes[j] % size;
                //                 uint jgy = goalIndexes[j] / size;
                //                 if (ipy == igy == jpy == jgy && ((ipx < jpx && igx > jgx) || (ipx > jpx && igx < jgx))) {
                //                     rows.insert(i);
                //                     rows.insert(j);
                //                     // std::cout << "--" << i << "-" << j << "\n";
                //                     cost += 2;
                //                 }
                //             }
                //         }
                //     }
                //     if (cols.count(i) == 0) {
                //         for (uint y = 0; y < size; ++y) {
                //             uint j = puzzle.grid[y * size + ipx];
                //             if (j != 0 && i != j && cols.count(j) == 0) {
                //                 uint jpx = currentIndexes[j] % size;
                //                 uint jpy = currentIndexes[j] / size;
                //                 uint jgx = goalIndexes[j] % size;
                //                 uint jgy = goalIndexes[j] / size;
                //                 if (ipx == igx == jpx == jgx && ((ipy < jpy && igy > jgy) || (ipy > jpy && igy < jgy))) {
                //                     cols.insert(i);
                //                     cols.insert(j);
                //                     // std::cout << "=-" << i << "-" << j << "\n";
                //                     cost += 2;
                //                 }
                //             }
                //         }
                //     }
                // }

                // return cost;
            }

        private:
            uint goalIndexes[size * size];

        };

    }

}
