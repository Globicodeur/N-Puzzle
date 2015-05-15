#pragma once

#include "parsing/forwards.hpp"

#include "options/Options.hpp"

#include "exceptions.hpp"
#include "astar.hpp"

#include "tools/Benchmark.hpp"

namespace algorithm {

    template <HClass Heuristic, bool uniform, bool ida>
    class Solver {

        using MaybeState = boost::optional<parsing::ParsedPuzzle>;

    public:

        Solver(const MaybeState & initial, const MaybeState & goal):
            initial { initial },
            goal    { goal } {

            if (initial)
                modelState = *initial;
            else if (goal)
                modelState = *goal;
        }

        template <class F>
        void solve(F onSolved) const {
            // Generate both
            if (!initial && !goal) {
                if (Options::generationSize == 0)
                    throw error::ZeroGeneration { };

                findAndApplyStaticSolver<1>(Options::generationSize, onSolved);
            }
            // Or use the model
            else {
                if (modelState.size() == 0)
                    throw error::EmptyPuzzle { };

                findAndApplyStaticSolver<1>(modelState.size(), onSolved);
            }
        }

    private:

        MaybeState initial, goal;
        parsing::ParsedPuzzle modelState;

        // This is the solver endpoint
        template <uint size, class F>
        void solve(F onSolved) const {
            Puzzle<size> start, end;

            // Building the puzzles that are fixed
            if (initial)
                start = puzzle::buildStaticPuzzle<size>(*initial);
            if (goal) {
                if (goal->size() != size)
                    throw error::GoalSizeMismatch { goal->size(), size };
                end = puzzle::buildStaticPuzzle<size>(*goal);
            }

            // Handling all the cases in order to always have a solvable pair
            auto snail = puzzle::makeSnail<size>();
            if (!initial) {
                if (goal) // Fixed goal -> match it
                    start = puzzle::generateMatch(end);
                else if (Options::randomGoal) // Random goal -> no constraints
                    start = puzzle::generate<size>();
                else // Snail goal -> match it
                    start = puzzle::generateMatch(snail);
            }
            if (!goal) {
                if (Options::randomGoal) // Fixed or not, match the start
                    end = puzzle::generateMatch(start);
                else // Start matches the snail so it is ok
                    end = snail;
            }

            std::cout << "Initial state:\n\n" << start << "\n\n";
            std::cout << "Final state:\n\n" << end << "\n";
            std::cout << "==============\n" << std::endl;

            if (!isSolvable(start, end))
                throw error::PuzzleNotSolvable { };

            tools::Benchmark bench { "Computation time" };
            if (ida)
                onSolved(idastar<Heuristic, uniform>(start, end));
            else
                onSolved(astar<Heuristic, uniform>(start, end));
        }

        // Runtime value unrolling
        // -> iterating over the puzzle size (determined at runtime) to apply a
        // solving function with a static size (i.e. known at compile time)
        // This should allow some badass optimizations
        // (BTW, fuck clang 3.6 for still not supporting non type template
        // parameter packs)
        template <uint size, class F>
        std::enable_if_t<(size > MAX_PUZZLE_SIZE)>
        findAndApplyStaticSolver(uint runtimeSize, F) const {
            throw error::PuzzleSizeTooLarge { runtimeSize };
        }

        template <uint size, class F>
        std::enable_if_t<size <= MAX_PUZZLE_SIZE>
        findAndApplyStaticSolver(uint runtimeSize, F onSolved) const {
            if (runtimeSize == size)
                return solve<size>(onSolved);

            findAndApplyStaticSolver<size + 1>(runtimeSize, onSolved);
        }

    };

}
