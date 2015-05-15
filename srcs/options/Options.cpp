#include "Options.hpp"

std::string                 Options::initialFile    { };
std::string                 Options::goalFile       { };
std::vector<std::string>    Options::heuristics     { "manhattan" };
std::string                 Options::searchStrategy { "uniform" };
std::string                 Options::astarVariant   { "astar" };
bool                        Options::randomGoal     { false };
uint                        Options::generationSize { 3 };
bool                        Options::showMoves      { false };

static auto getUsage(void) {
    po::options_description usage { "Available options" };

    usage.add_options()
        ("help,h",      "Show this help message")
        ("initial,i",   po::value(&Options::initialFile),
                        "Input file for the initial state")
        ("goal,g",      po::value(&Options::goalFile),
                        "Input file for the goal state")
        ("heuristics",  po::value(&Options::heuristics)
                            ->multitoken(),
                        "Heuristics used for solving the puzzle")
        ("strategy",    po::value(&Options::searchStrategy),
                        "Search strategy (uniform or greedy)")
        ("variant,v",   po::value(&Options::astarVariant),
                        "A* variant (astar or idastar)")
        ("random-goal", po::bool_switch(&Options::randomGoal),
                        "Generates a goal state other than a snail")
        ("size,s",      po::value(&Options::generationSize),
                        "Size of randomly generated states")
        ("show-moves",  po::bool_switch(&Options::showMoves),
                        "Display the states resulting from the path finding")
    ;

    return usage;
}

static auto getPositional(void) {
    po::positional_options_description usage;

    usage.add("initial", 1);
    usage.add("goal", 1);

    return usage;
}

void Options::parseFromCommandLine(int argc, char **argv) {
    po::variables_map options;
    po::command_line_parser parser { argc, argv };

    auto usage = getUsage();
    auto positional = getPositional();
    try {
        po::store(
            parser.options(usage)
                  .positional(positional)
                  .run(),
            options
        );
        if (options.count("help")) {
            std::cerr << usage << std::endl;
            exit(0);
        }
        po::notify(options);
    }
    catch (const po::error & exception) {
        std::cerr << exception.what() << std::endl << std::endl;
        std::cerr << usage << std::endl;
        exit(1);
    }
}
