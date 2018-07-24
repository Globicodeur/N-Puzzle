#pragma once

#include "parsing/forwards.hpp"

#include "options/Options.hpp"

#include "exceptions.hpp"
#include "heuristics.hpp"

#include "path_finding/astar.hpp"
#include "path_finding/idastar.hpp"

#include "tools/Benchmark.hpp"
#include "tools/ansi.hpp"
#include "tools/type_traits.hpp"

namespace algorithm {

    template <HClass Heuristic, bool uniform, bool ida>
    class Solver {

        using MaybeState = std::optional<parsing::ParsedPuzzle>;

    public:

        Solver(MaybeState initial, MaybeState goal):
            initial { initial },
            goal    { goal }
        {
            if (initial)
                modelState = *initial;
            else if (goal)
                modelState = *goal;
        }

        template <class F>
        void solve(F onSolved) const {
            auto puzzle_size = [=]() -> PuzzleSize {
                // Generate both
                if (!initial && !goal) {
                    if (Options::generationSize == 0)
                        throw error::ZeroGeneration { };

                    return Options::generationSize;
                }
                // Or use the model
                else {
                    if (modelState.size() == 0)
                        throw error::EmptyPuzzle { };

                    return modelState.size();
                }
            }();

            try {
                tools::reify_range<1, MAX_PUZZLE_SIZE>(puzzle_size)(
                    [=](auto size) { this->solveParsed<size>(onSolved); }
                );
            }
            catch (std::out_of_range &) {
                throw error::PuzzleSizeTooLarge { puzzle_size };
            }
        }

    private:

        MaybeState initial, goal;
        parsing::ParsedPuzzle modelState;

        // This is the solver endpoint
        template <PuzzleSize size, class F>
        void solve(Puzzle<size> start, Puzzle<size> end, F onSolved) const {
            std::cout << "==============\n\n";
            std::cout << "Initial state:\n\n"
                      << ansi::BOLD << start << ansi::RESET << "\n\n";
            std::cout << "Final state:\n\n"
                      << ansi::BOLD << end << ansi::RESET << "\n";
            std::cout << "==============\n\n";

            if (!isSolvable(start, end))
                throw error::PuzzleNotSolvable { };

            std::cout << "A* variant:      " << ansi::BOLD << ansi::YELLOW
                      << Options::astarVariant << ansi::RESET << "\n";
            std::cout << "Search strategy: " << ansi::BOLD << ansi::YELLOW
                      << Options::searchStrategy << ansi::RESET << "\n";
            prettyPrintHeuristics();
            if (!initial || (!goal && Options::randomGoal))
                std::cout << "Random seed:     " << ansi::BOLD
                          << Options::randomSeed << ansi::RESET << "\n";
            std::cout << "\n==============\n" << std::endl;

            tools::Benchmark bench { "Computation time" };
            if (ida)
                onSolved(idastar<Heuristic, uniform>(start, end));
            else
                onSolved(astar<Heuristic, uniform>(start, end));

            std::cout << "==============\n\n";
        }

        static void prettyPrintHeuristics() {
            using namespace algorithm::heuristics;
            std::set<std::string> heuristics {
                Options::heuristics.begin(),
                Options::heuristics.end()
            };

            std::cout << "Heuristics:      " << ansi::BOLD << ansi::YELLOW
                      << prettyNames[*heuristics.begin()];
            auto it = std::next(heuristics.begin());
            for (; it != heuristics.end(); ++it)
                std::cout << " + " << prettyNames[*it];
            std::cout << ansi::RESET << "\n";

            // All of our heuristics are admissible in standalone
            bool admissible = (heuristics.size() == 1);
            // Special case for Manhattan + Linear which is also admissible
            if (heuristics.size() == 2
                && heuristics.count(manhattanName)
                && heuristics.count(linearName))
                admissible = true;

            std::cout << "Admissible:      " << ansi::BOLD
                      << (admissible ? ansi::GREEN : ansi::RED)
                      << (admissible ? "Yes" : "No")
                      << ansi::RESET << "\n";
        }

        // Calls the endpoint with static puzzles generated from either parsed
        // puzzles or a random generator
        template <PuzzleSize size, class F>
        void solveParsed(F onSolved) const {
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

            solve(start, end, onSolved);
        }
    };

}
