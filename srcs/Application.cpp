#include "Application.hpp"

#include "options/Options.hpp"

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);
}

void Application::run(void) {
    std::cout << "Starting with: " << Options::inputFile << std::endl;
}
