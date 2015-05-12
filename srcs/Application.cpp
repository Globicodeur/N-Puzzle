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

    struct UnknownHeuristic: public std::exception {

        UnknownHeuristic(const std::string & name):
            what_ { "Unknown heuristic: " + name }
        { }

        virtual const char * what() const noexcept {
            return what_.c_str();
        }

    private:
        std::string what_;
    };

    struct UnknownStrategy: public std::exception {

        UnknownStrategy(const std::string & name):
            what_ { "Unknown strategy: " + name }
        { }

        virtual const char * what() const noexcept {
            return what_.c_str();
        }

    private:
        std::string what_;
    };

    struct UnknownAStarVariant: public std::exception {

        UnknownAStarVariant(const std::string & name):
            what_ { "Unknown A* variant: " + name }
        { }

        virtual const char * what() const noexcept {
            return what_.c_str();
        }

    private:
        std::string what_;
    };

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

    // This is the astar variant endpoint. It applies the chosen astar function
    template <bool uniform, HClass... Hs>
    void solveWithAstarVariant() const {
        if (Options::astarVariant == "astar")
            solve<false, uniform, Hs...>();
        else if (Options::astarVariant == "idastar")
            solve<true, uniform, Hs...>();
        else
            throw UnknownAStarVariant { Options::astarVariant };
    }

    // This is the heuristic endpoint. It applies the runtime strategy
    template <HClass... Hs>
    void solveWithStrategy(std::tuple<tools::Wrapper<Hs>...>) const {
        if (Options::searchStrategy == "uniform")
            solveWithAstarVariant<true, Hs...>();
        else if (Options::searchStrategy == "greedy")
            solveWithAstarVariant<false, Hs...>();
        else
            throw UnknownStrategy { Options::searchStrategy };
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

    // Calls the heuristic endpoint if all the names are processed. Adds the
    // next heuristic otherwise
    template <std::size_t i, class Set>
    std::enable_if_t<i <= HEURISTICS_COUNT>
    solveWithHeuristics() const {
        using HeuristicsTuple = typename tools::Unify<
            std::tuple,
            Set,
            boost::mpl::size<Set>::type::value
        >::type;

        if (i == Options::heuristics.size())
            return solveWithStrategy(HeuristicsTuple { });

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
            throw UnknownHeuristic { Options::heuristics.at(opt_i) };
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
    constexpr bool uniform = true;
    constexpr bool ida = false;
    using StaticHeuristics = algorithm::heuristics::Composition<
          algorithm::heuristics::ManhattanDistance
        , algorithm::heuristics::LinearConflict
        // , algorithm::heuristics::MisplacedTiles
        // , algorithm::heuristics::MisplacedRowsAndColumns
    >;
    using StaticSolver = algorithm::Solver<
        StaticHeuristics::template Composer,
        uniform,
        ida
    >;

    StaticSolver debugSolver { initialState, goalState };
    debugSolver.solve([](const auto & solution) {
        std::cout << solution << std::endl;
    });
#endif
}
