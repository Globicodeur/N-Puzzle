#include "Application.hpp"

#include "options/program_options.hpp"

Application::Application(int argc, char **argv) {
    options::parseFromCommandLine(argc, argv);
}

void Application::run(void) {

}
