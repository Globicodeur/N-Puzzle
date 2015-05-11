#include "Application.hpp"

#include "options/Options.hpp"
#include "parsing/Parser.hpp"

#include "algorithm/Solver.hpp"
#include "algorithm/heuristics.hpp"
#include "algorithm/heuristic_composition.hpp"

#include "tools/type_traits.hpp"

// Forwards
using Initial = const parsing::ParsedPuzzle &;
using Goal = boost::optional<parsing::ParsedPuzzle>;
using algorithm::heuristics::HEURISTICS_COUNT;

template <std::size_t opt_i, std::size_t h_i, class Set>
std::enable_if_t<h_i == HEURISTICS_COUNT - 1>
addHeuristic(Initial, Goal);

template <std::size_t opt_i, std::size_t h_i, class Set>
std::enable_if_t<h_i != HEURISTICS_COUNT - 1>
addHeuristic(Initial, Goal);
//

template <HClass... Hs>
void solve(Initial initial, Goal goal, std::tuple<tools::Wrapper<Hs>...>) {
    using Heuristics = algorithm::heuristics::Composition<Hs...>;
    using SolverT = algorithm::Solver<Heuristics::template Composer>;

    SolverT solver { initial, goal };

    solver.solve([](const auto & solution) {
        std::cout << solution << std::endl;
    });
}

template <std::size_t i, class Set>
std::enable_if_t<(i > HEURISTICS_COUNT)>
solveWithHeuristics(Initial, Goal) {
    throw std::out_of_range { "Too many heuristics given" };
}

template <std::size_t i, class Set>
std::enable_if_t<
    boost::mpl::size<Set>::type::value != 0 &&
    i <= HEURISTICS_COUNT
>
solveWithHeuristics(Initial initial, Goal goal) {
    using HeuristicsTuple = typename tools::Unify<
        std::tuple,
        Set,
        boost::mpl::size<Set>::type::value
    >::type;

    if (i == Options::heuristics.size())
        return solve(initial, goal, HeuristicsTuple { });
    addHeuristic<i, 0, Set>(initial, goal);
}

template <std::size_t opt_i, std::size_t h_i, class Set>
bool optionMatch(Initial initial, Goal goal) {
    using Heuristic = algorithm::heuristics::HeuristicAt<h_i>;
    using NewSet = typename boost::mpl::insert<
        Set,
        tools::Wrapper<Heuristic::template type>
    >::type;

    if (Heuristic::name == Options::heuristics.at(opt_i)) {
        solveWithHeuristics<opt_i + 1, NewSet>(initial, goal);
        return true;
    }
    return false;
}

template <std::size_t opt_i, std::size_t h_i, class Set>
std::enable_if_t<h_i == HEURISTICS_COUNT - 1>
addHeuristic(Initial initial, Goal goal) {
    if (!optionMatch<opt_i, h_i, Set>(initial, goal))
        throw Application::UnknownHeuristic { Options::heuristics.at(opt_i) };
}

template <std::size_t opt_i, std::size_t h_i, class Set>
std::enable_if_t<h_i != HEURISTICS_COUNT - 1>
addHeuristic(Initial initial, Goal goal) {
    if (!optionMatch<opt_i, h_i, Set>(initial, goal))
        addHeuristic<opt_i, h_i + 1, Set>(initial, goal);
}

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);
}

template <std::size_t i = 0, HClass... Hs>
std::enable_if_t<sizeof...(Hs) == 0>
solveWithHeuristics(Initial initial, Goal goal) {
    addHeuristic<0, 0, boost::mpl::set<>>(initial, goal);
}

void Application::run(void) {
    parsing::Parser parser;
    parsing::ParsedPuzzle initialState;
    Goal goalState;

    initialState = parser.parse(Options::initialFile);
    if (!Options::goalFile.empty())
        goalState = parser.parse(Options::goalFile);

    // RUNTIME composition
    solveWithHeuristics(initialState, goalState);

    // STATIC composition (because fuck compilers)
    using StaticHeuristics = std::tuple<
          tools::Wrapper<algorithm::heuristics::ManhattanDistance>
        , tools::Wrapper<algorithm::heuristics::LinearConflict>
        // , tools::Wrapper<algorithm::heuristics::MisplacedTiles>
        // , tools::Wrapper<algorithm::heuristics::MisplacedRowsAndColumns>
    >;
    solve(initialState, goalState, StaticHeuristics{});
}
