#pragma once

#include "exceptions.hpp"

#include "parsing/forwards.hpp"

#include "tools/type_traits.hpp"

#include "algorithm/Solver.hpp"
#include "algorithm/heuristics.hpp"
#include "algorithm/heuristic_composition.hpp"

namespace runtime {

    using parsing::ParsedPuzzle;

    using algorithm::heuristics::HEURISTICS_COUNT;

    // This is just a helper class that will unroll runtime options to call a
    // matching static solver on the input
    // /!\ Since there are a lot of possible static solvers to generate, this
    // class can slow down compilation time by a lot!
    class Solver {

        using ParsedGoal = boost::optional<ParsedPuzzle>;

    public:

        Solver(const ParsedPuzzle & initial, const ParsedGoal & goal):
            initial { initial },
            goal    { goal }
        { }

        void solve() const {
            addHeuristic<boost::mpl::set<>, 0, 0>();
        }

    private:

        ParsedPuzzle    initial;
        ParsedGoal      goal;

        // This is the global unrolling endpoint
        template <bool ida, bool uniform, HClass... Hs>
        void solve() const {
            using Heuristics = algorithm::heuristics::Composition<Hs...>;
            using StaticSolver = algorithm::Solver<
                Heuristics::template Composer,
                uniform,
                ida
            >;

            StaticSolver solver { initial, goal };

            solver.solve([](const auto & solution) {
                std::cout << solution << std::endl;
            });
        }

        // This is the astar variant endpoint. It applies the chosen astar
        // function
        template <bool uniform, HClass... Hs>
        void solveWithAstarVariant() const {
            if (Options::astarVariant == "astar")
                solve<false, uniform, Hs...>();
            else if (Options::astarVariant == "idastar")
                solve<true, uniform, Hs...>();
            else
                throw error::UnknownAStarVariant { Options::astarVariant };
        }

        // This is the heuristic endpoint. It applies the runtime strategy
        template <template <HClass> class Wrapper, HClass... Hs>
        void solveWithStrategy(std::tuple<Wrapper<Hs>...>) const {
            if (Options::searchStrategy == "uniform")
                solveWithAstarVariant<true, Hs...>();
            else if (Options::searchStrategy == "greedy")
                solveWithAstarVariant<false, Hs...>();
            else
                throw error::UnknownStrategy { Options::searchStrategy };
        }

        // Checks a runtime heuristic name and inserts its matching class into
        // the heuristic set if it is valid
        template <class HeuristicSet, std::size_t opt_i, std::size_t h_i>
        bool optionMatch() const {
            // Just in this scope to avoid cluttering
            using namespace algorithm::heuristics;
            // Adding the heuristic to the set
            using HeuristicTrait = TraitAt<h_i>;
            using NewSet = typename boost::mpl::insert<
                HeuristicSet,
                HWrapper<HeuristicTrait::template heuristic>
            >::type;

            if (HeuristicTrait::name == Options::heuristics.at(opt_i)) {
                solveWithHeuristics<NewSet, opt_i + 1>();
                return true;
            }
            return false;
        }

        // Calls the heuristic endpoint if all the names are processed. Adds the
        // next heuristic otherwise
        template <class HeuristicSet, std::size_t i>
        std::enable_if_t<i <= HEURISTICS_COUNT>
        solveWithHeuristics() const {
            // Just in this scope to avoid cluttering
            using namespace boost::mpl;
            using namespace algorithm::heuristics;
            // Sorting the heuristics to avoid irrelevant template instantiations
            using HeuristicsTuple = typename tools::Sort<
                HeuristicSet,
                std::tuple,
                less<WrappedId<_1>, WrappedId<_2>>,
                size<HeuristicSet>::type::value
            >::type;

            if (i == Options::heuristics.size())
                return solveWithStrategy(HeuristicsTuple { });

            addHeuristic<HeuristicSet, i, 0>();
        }

        // Edge unrolling case
        template <class HeuristicSet, std::size_t i>
        std::enable_if_t<(i > HEURISTICS_COUNT)>
        solveWithHeuristics() const {
            throw std::out_of_range { "Too many heuristics given" };
        }

        // Trying to match the `h_i`-th heuristic name
        template <class HeuristicSet, std::size_t opt_i, std::size_t h_i>
        std::enable_if_t<h_i != HEURISTICS_COUNT - 1>
        addHeuristic() const {
            if (!optionMatch<HeuristicSet, opt_i, h_i>())
                addHeuristic<HeuristicSet, opt_i, h_i + 1>();
        }

        // Trying to match the last heuristic name. Throwing if it does not
        template <class HeuristicSet, std::size_t opt_i, std::size_t h_i>
        std::enable_if_t<h_i == HEURISTICS_COUNT - 1>
        addHeuristic() const {
            if (!optionMatch<HeuristicSet, opt_i, h_i>())
                throw error::UnknownHeuristic { Options::heuristics.at(opt_i) };
        }

    };

}
