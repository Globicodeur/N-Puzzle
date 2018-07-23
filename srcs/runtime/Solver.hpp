#pragma once

#include "exceptions.hpp"

#include "parsing/forwards.hpp"

#include "tools/type_traits.hpp"

#include "algorithm/Solver.hpp"
#include "algorithm/heuristics.hpp"

namespace runtime {

    using algorithm::heuristics::HEURISTICS_COUNT;

    // This is just a helper class that will unroll runtime options to call a
    // matching static solver on the input
    // /!\ Since there are a lot of possible static solvers to generate, this
    // class can slow down compilation time by a lot!
    class Solver {

        using MaybeState = std::optional<parsing::ParsedPuzzle>;

    public:

        Solver(const MaybeState & initial, const MaybeState & goal):
            initial { initial },
            goal    { goal }
        { }

        void solve() const {
            addHeuristic<boost::mpl::set<>, 0, 0>();
        }

    private:

        MaybeState initial, goal;

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
        template <class... Traits>
        void solveWithStrategy(std::tuple<Traits...>) const {
            if (Options::searchStrategy == "uniform")
                solveWithAstarVariant<true, Traits::template heuristic...>();
            else if (Options::searchStrategy == "greedy")
                solveWithAstarVariant<false, Traits::template heuristic...>();
            else
                throw error::UnknownStrategy { Options::searchStrategy };
        }

        // Checks a runtime heuristic name and inserts its matching class into
        // the heuristic set if it is valid
        template <class HeuristicsSet, std::size_t opt_i, std::size_t h_i>
        bool optionMatch() const {
            // Adding the heuristic to the set
            using HeuristicTrait = std::tuple_element_t<
                h_i,
                algorithm::heuristics::Heuristics
            >;
            using NewSet = typename boost::mpl::insert<
                HeuristicsSet,
                HeuristicTrait
            >::type;

            if (HeuristicTrait::name == Options::heuristics.at(opt_i)) {
                solveWithHeuristics<NewSet, opt_i + 1>();
                return true;
            }
            return false;
        }

        // Calls the heuristic endpoint if all the names are processed. Adds the
        // next heuristic otherwise
        template <class HeuristicsSet, std::size_t i>
        std::enable_if_t<i <= HEURISTICS_COUNT>
        solveWithHeuristics() const {
            // Just in this scope to avoid cluttering
            using namespace boost::mpl;
            // Sorting the heuristics to avoid irrelevant template instantiations
            using HeuristicsTuple = typename tools::Sort<
                HeuristicsSet,
                std::tuple,
                less<_1, _2>,
                size<HeuristicsSet>::type::value
            >::type;

            if (i == Options::heuristics.size())
                return solveWithStrategy(HeuristicsTuple { });

            addHeuristic<HeuristicsSet, i, 0>();
        }

        // Edge unrolling case
        template <class HeuristicsSet, std::size_t i>
        std::enable_if_t<(i > HEURISTICS_COUNT)>
        solveWithHeuristics() const {
            throw std::out_of_range { "Too many heuristics given" };
        }

        // Trying to match the `h_i`-th heuristic name
        template <class HeuristicsSet, std::size_t opt_i, std::size_t h_i>
        std::enable_if_t<h_i != HEURISTICS_COUNT - 1>
        addHeuristic() const {
            if (!optionMatch<HeuristicsSet, opt_i, h_i>())
                addHeuristic<HeuristicsSet, opt_i, h_i + 1>();
        }

        // Trying to match the last heuristic name. Throwing if it does not
        template <class HeuristicsSet, std::size_t opt_i, std::size_t h_i>
        std::enable_if_t<h_i == HEURISTICS_COUNT - 1>
        addHeuristic() const {
            if (!optionMatch<HeuristicsSet, opt_i, h_i>())
                throw error::UnknownHeuristic { Options::heuristics.at(opt_i) };
        }

    };

}
