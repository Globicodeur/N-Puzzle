#include "Application.hpp"

#include "options/Options.hpp"

#include "parsing/Parser.hpp"

#include "tools/type_traits.hpp"

#include "algorithm/Solver.hpp"
#include "algorithm/heuristics.hpp"
#include "algorithm/heuristic_composition.hpp"

using parsing::ParsedPuzzle;

using algorithm::heuristics::HEURISTICS_COUNT;

// This is just a helper class that will unroll runtime options to call a
// matching static solver on the input
// /!\ Since there are a lot of possible static solvers to generate, this class
// can slow down compilation by a lot!
class RuntimeSolver {

    using ParsedGoal = boost::optional<parsing::ParsedPuzzle>;

public:

    RuntimeSolver(const ParsedPuzzle & initial, const ParsedGoal & goal):
        initial { initial },
        goal    { goal }
    { }

    void solve() const {
        addHeuristic<0, 0, boost::mpl::set<>>();
    }

private:

    ParsedPuzzle    initial;
    ParsedGoal      goal;

    // This is the unrolling endpoint
    template <HClass... Hs>
    void solve(std::tuple<tools::Wrapper<Hs>...>) const {
        using Heuristics = algorithm::heuristics::Composition<Hs...>;
        using StaticSolver = algorithm::Solver<Heuristics::template Composer>;

        StaticSolver solver { initial, goal };

        solver.solve([](const auto & solution) {
            std::cout << solution << std::endl;
        });
    }

    // Checks a runtime heuristic name and inserts its matching class into the
    // heuristic set if it is valid
    template <std::size_t opt_i, std::size_t h_i, class Set>
    bool optionMatch() const {
        using Heuristic = algorithm::heuristics::HeuristicAt<h_i>;
        using NewSet = typename boost::mpl::insert<
            Set,
            tools::Wrapper<Heuristic::template type>
        >::type;

        if (Heuristic::name == Options::heuristics.at(opt_i)) {
            solveWithHeuristics<opt_i + 1, NewSet>();
            return true;
        }
        return false;
    }

    // Calls the endpoint if all the names are processed. Adds the next
    // heuristic otherwise
    template <std::size_t i, class Set>
    std::enable_if_t<i <= HEURISTICS_COUNT>
    solveWithHeuristics() const {
        using HeuristicsTuple = typename tools::Unify<
            std::tuple,
            Set,
            boost::mpl::size<Set>::type::value
        >::type;

        if (i == Options::heuristics.size())
            return solve(HeuristicsTuple { });

        addHeuristic<i, 0, Set>();
    }

    // Edge unrolling case
    template <std::size_t i, class Set>
    std::enable_if_t<(i > HEURISTICS_COUNT)>
    solveWithHeuristics() const {
        throw std::out_of_range { "Too many heuristics given" };
    }

    // Trying to match the `h_i`-th heuristic name
    template <std::size_t opt_i, std::size_t h_i, class Set>
    std::enable_if_t<h_i != HEURISTICS_COUNT - 1>
    addHeuristic() const {
        if (!optionMatch<opt_i, h_i, Set>())
            addHeuristic<opt_i, h_i + 1, Set>();
    }

    // Trying to match the last heuristic name. Throwing if it does not
    template <std::size_t opt_i, std::size_t h_i, class Set>
    std::enable_if_t<h_i == HEURISTICS_COUNT - 1>
    addHeuristic() const {
        if (!optionMatch<opt_i, h_i, Set>())
            throw Application::UnknownHeuristic { Options::heuristics.at(opt_i) };
    }

};

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);
}

void Application::run(void) {
    parsing::Parser parser;
    ParsedPuzzle initialState;
    boost::optional<ParsedPuzzle> goalState;

    initialState = parser.parse(Options::initialFile);
    if (!Options::goalFile.empty())
        goalState = parser.parse(Options::goalFile);

#ifndef DEBUG
    // RUNTIME composition
    RuntimeSolver solver { initialState, goalState };
    solver.solve(); // This line is the nightmare of every compiler
#else
    // STATIC composition (for debugging purposes and because fuck compilers)
    using StaticHeuristics = algorithm::heuristics::Composition<
          algorithm::heuristics::ManhattanDistance
        , algorithm::heuristics::LinearConflict
        // , algorithm::heuristics::MisplacedTiles
        // , algorithm::heuristics::MisplacedRowsAndColumns
    >;
    using StaticSolver = algorithm::Solver<StaticHeuristics::template Composer>;
    StaticSolver debugSolver { initialState, goalState };
    debugSolver.solve([](const auto & solution) {
        std::cout << solution << std::endl;
    });
#endif
}
