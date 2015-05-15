#include "Application.hpp"

#include "options/Options.hpp"

#include "parsing/Parser.hpp"

#include "runtime/Solver.hpp"

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);
}

void Application::run(void) {
    using MaybeState = boost::optional<parsing::ParsedPuzzle>;

    parsing::Parser parser;
    MaybeState initial, goal;

    if (!Options::initialFile.empty())
        initial = parser.parse(Options::initialFile);
    if (!Options::goalFile.empty())
        goal = parser.parse(Options::goalFile);

#ifdef DEBUG
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

    StaticSolver debugSolver { initial, goal };
    debugSolver.solve([](const auto & solution) {
        std::cout << solution << std::endl;
    });
#else
    // RUNTIME composition
    runtime::Solver solver { initial, goal };
    solver.solve(); // This line is the nightmare of every compiler
#endif
}
