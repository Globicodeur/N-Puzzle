#pragma once

#include "parsing/forwards.hpp"

#include "exceptions.hpp"
#include "astar.hpp"

#include "tools/Benchmark.hpp"

namespace algorithm {

    using parsing::ParsedPuzzle;

    template <template <uint> class Heuristic>
    struct Solver {

        template <class F>
        void solve(const ParsedPuzzle & parsed,
                   boost::optional<ParsedPuzzle> goal,
                   F onSolved) {
            if (parsed.size() == 0)
                throw error::EmptyPuzzle { };
            findAndApplyStaticSolver<1>(parsed, goal, onSolved);
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
        findAndApplyStaticSolver(const ParsedPuzzle & parsed,
                                 boost::optional<ParsedPuzzle> goal,
                                 F onSolved) {
            if (parsed.size() != size)
                throw error::PuzzleSizeTooLarge { parsed.size() };
            solve<size>(parsed, goal, onSolved);
        }

        template <uint size, class F>
        static std::enable_if_t<size < MAX_PUZZLE_SIZE>
        findAndApplyStaticSolver(const ParsedPuzzle & parsed,
                                 boost::optional<ParsedPuzzle> goal,
                                 F onSolved) {
            if (parsed.size() == size)
                return solve<size>(parsed, goal, onSolved);
            return findAndApplyStaticSolver<size + 1>(parsed, goal, onSolved);
        }

        template <uint size, class F>
        static void solve(const ParsedPuzzle & parsed,
                          boost::optional<ParsedPuzzle> goal,
                          F onSolved) {
            Puzzle<size> start, end;

            start = buildStaticPuzzle<size>(parsed);
            if (goal) {
                if (goal->size() != size)
                    throw error::GoalSizeMismatch { goal->size(), size };
                end = buildStaticPuzzle<size>(*goal);
            }
            else
                end = puzzle::makeSnail<size>();

            std::cout << "Initial state:\n\n" << start << "\n\n";
            std::cout << "Final state:\n\n" << end << "\n";
            std::cout << "==============\n" << std::endl;

            if (!isSolvable(start, end))
                throw error::PuzzleNotSolvable { };

            tools::Benchmark bench { "Computation time" };
            onSolved(astar<Heuristic>(start, end));
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
