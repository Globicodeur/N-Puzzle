#include "Application.hpp"

#include "options/Options.hpp"
#include "parsing/Parser.hpp"

#include "algorithm/Solver.hpp"
#include "algorithm/heuristics.hpp"
#include "algorithm/heuristic_composition.hpp"

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);
}

template <uint size>
using Heuristics = algorithm::heuristics::Composition<
    // algorithm::heuristics::MisplacedRowsAndColumns,
    algorithm::heuristics::ManhattanDistance
    // algorithm::heuristics::LinearConflict
>::Composer<size>;

void Application::run(void) {
    std::cout << "Starting with: " << Options::inputFile << std::endl;

    parsing::Parser parser;
    auto initialState = parser.parse(Options::inputFile);

    algorithm::Solver<Heuristics> solver;

    auto showResults = [](const auto & solution) {
        std::cout << solution << std::endl;
    };

    solver.solve(initialState, showResults);
}
