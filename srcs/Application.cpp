#include "Application.hpp"

#include "options/Options.hpp"

#include "parsing/grammar.hpp"

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);
}

void Application::run(void) {
    std::cout << "Starting with: " << Options::inputFile << std::endl;

    using it = sp::istream_iterator;
    std::ifstream ifs { Options::inputFile };
    ifs.unsetf(std::ios::skipws);
    it begin { ifs };
    it end;

    auto parseOK = qi::phrase_parse(
        begin,
        end,
        parsing::BoardGrammar<it>{},
        qi::blank
    );

    std::cout << std::boolalpha << parseOK << "\n";
    auto allConsumed = (begin == end);
    std::cout << std::boolalpha << allConsumed << "\n";
    std::cout << "---\n" + std::string(begin, end) << std::endl;
}
