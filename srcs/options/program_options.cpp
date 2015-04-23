#include "program_options.hpp"

namespace options {

    static po::options_description getUsage(void) {
        po::options_description usage { "Available options" };

        usage.add_options()
            ("help,h", "Show this help message")
        ;

        return usage;
    }

    void parseFromCommandLine(int argc, char **argv) {
        po::variables_map options;
        po::command_line_parser parser { argc, argv };

        auto usage = getUsage();
        try {
            po::store(parser.options(usage).run(), options);
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

}
