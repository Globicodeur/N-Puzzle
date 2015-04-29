#include "Application.hpp"

#include "options/Options.hpp"
#include "parsing/Parser.hpp"
#include "algorithm/Solver.hpp"

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);
}

void Application::run(void) {
    std::cout << "Starting with: " << Options::inputFile << std::endl;

    parsing::Parser parser;
    algorithm::Solver solver;

    // Add configuration here

    auto parsed = parser.parse(Options::inputFile);
    auto solution = solver.solve(parsed);

    (void)solution;
}
