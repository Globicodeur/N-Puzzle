#pragma once

#include "parsing/forwards.hpp"

#include "exceptions.hpp"
#include "astar.hpp"

namespace algorithm {

    using parsing::ParsedPuzzle;

    template <template <uint> class Heuristic>
    struct Solver {

        template <class F>
        void solve(const ParsedPuzzle & parsed, F onSolved) {
            if (parsed.size() == 0)
                throw error::EmptyPuzzle { };
            findAndApplyStaticSolver<1>(parsed, onSolved);
        }

    private:

        // Runtime value unrolling
        // -> iterating over the puzzle size (determined at runtime) to apply a
        // solving function with a static size (i.e. known at compile time)
        // This should allow some badass optimizations
        // (BTW, fuck clang 3.6 for still not supporting non type template
        // parameter packs)
        template <uint size, class F>
        static std::enable_if_t<size == MAX_PUZZLE_SIZE>
        findAndApplyStaticSolver(const ParsedPuzzle & parsed, F onSolved) {
            if (parsed.size() != size)
                throw error::PuzzleSizeTooLarge { parsed.size() };
            solve<size>(parsed, onSolved);
        }

        template <uint size, class F>
        static std::enable_if_t<size < MAX_PUZZLE_SIZE>
        findAndApplyStaticSolver(const ParsedPuzzle & parsed, F onSolved) {
            if (parsed.size() == size)
                return solve<size>(parsed, onSolved);
            return findAndApplyStaticSolver<size + 1>(parsed, onSolved);
        }

        template <uint size, class F>
        static void solve(const ParsedPuzzle & parsed, F onSolved) {
            auto start = buildStaticPuzzle<size>(parsed);
            std::cout << start << std::endl;

            auto goal = puzzle::makeSnail<size>();
            std::cout << goal << std::endl;

            if (!isSolvable(start, goal))
                throw error::PuzzleNotSolvable { };

            onSolved(astar<Heuristic>(start, goal));
        }

        // Builds a puzzle of known size from nested vectors of values
        template <uint size>
        static auto buildStaticPuzzle(const ParsedPuzzle & parsed) {
            puzzle::Puzzle<size> puzzle;

            for (uint y = 0; y < size; ++y)
                for (uint x = 0; x < size; ++x)
                    puzzle.grid[y * size + x] = parsed.at(y).at(x);

            return puzzle;
        }

    };

}
