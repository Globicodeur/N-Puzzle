#pragma once

#include "parsing/forwards.hpp"

#include "exceptions.hpp"
#include "astar.hpp"

#include "tools/Benchmark.hpp"

namespace algorithm {

    using parsing::ParsedPuzzle;

    template <HClass Heuristic, bool uniform, bool ida>
    class Solver {

        using ParsedGoal = boost::optional<ParsedPuzzle>;

    public:

        Solver(const ParsedPuzzle & initial, const ParsedGoal & goal):
            initial { initial },
            goal    { goal }
        { }

        template <class F>
        void solve(F onSolved) {
            if (initial.size() == 0)
                throw error::EmptyPuzzle { };
            findAndApplyStaticSolver<1>(onSolved);
        }

    private:

        ParsedPuzzle    initial;
        ParsedGoal      goal;

        // Runtime value unrolling
        // -> iterating over the puzzle size (determined at runtime) to apply a
        // solving function with a static size (i.e. known at compile time)
        // This should allow some badass optimizations
        // (BTW, fuck clang 3.6 for still not supporting non type template
        // parameter packs)
        template <uint size, class F>
        std::enable_if_t<size == MAX_PUZZLE_SIZE>
        findAndApplyStaticSolver(F onSolved) const {
            if (initial.size() != size)
                throw error::PuzzleSizeTooLarge { initial.size() };

            solve<size>(onSolved);
        }

        template <uint size, class F>
        std::enable_if_t<size < MAX_PUZZLE_SIZE>
        findAndApplyStaticSolver(F onSolved) const {
            if (initial.size() == size)
                return solve<size>(onSolved);

            findAndApplyStaticSolver<size + 1>(onSolved);
        }

        template <uint size, class F>
        void solve(F onSolved) const {
            Puzzle<size> start, end;

            start = buildStaticPuzzle<size>(initial);
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
            if (ida)
                onSolved(idastar<Heuristic, uniform>(start, end));
            else
                onSolved(astar<Heuristic, uniform>(start, end));
        }



        }

    };

}
