#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <PuzzleSize size>
        struct Gaschnig {

            Gaschnig(const puzzle::Puzzle<size> & goal) {
                for (PuzzleSize i = 0; i < size * size; ++i) {
                    indexes[goal.grid[i]] = i;
                    values[i] = goal.grid[i];
                }
            }

            auto operator()(const puzzle::Puzzle<size> & puzzle) const {
                uint cost = 0;
                uint puzzleGrid[size * size];
                uint puzzleIndexes[size * size];

                for (uint i = 0; i < size * size; ++i) {
                    puzzleGrid[i] = puzzle.grid[i];
                    puzzleIndexes[puzzle.grid[i]] = i;
                }

                while (std::memcmp(puzzleGrid, values, sizeof(puzzleGrid))) {
                    uint swapper = puzzleIndexes[0];

                    if (puzzleIndexes[values[swapper]] == indexes[0]) {
                        for (uint i = 1; i < size * size; ++i) {
                            if (puzzleIndexes[i] != indexes[i] && puzzleIndexes[i] != indexes[0]) {
                                std::swap(puzzleGrid[swapper], puzzleGrid[puzzleIndexes[i]]);
                                std::swap(puzzleIndexes[0], puzzleIndexes[i]);
                                break ;
                            }
                            if (i == size * size - 1) {
                                std::swap(puzzleGrid[swapper], puzzleGrid[puzzleIndexes[values[swapper]]]);
                                std::swap(puzzleIndexes[0], puzzleIndexes[values[swapper]]);
                            }
                        }
                    }
                    else {
                        std::swap(puzzleGrid[swapper], puzzleGrid[puzzleIndexes[values[swapper]]]);
                        std::swap(puzzleIndexes[0], puzzleIndexes[values[swapper]]);
                        cost += 1;
                    }
                }

                return cost;
            }

        private:

            uint indexes[size * size];
            uint values[size * size];

        };

    }

}
