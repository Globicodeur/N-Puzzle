#pragma once

struct Options {

    static std::string              initialFile;
    static std::string              goalFile;
    static std::vector<std::string> heuristics;
    static std::string              searchStrategy;
    static std::string              astarVariant;

    static void parseFromCommandLine(int argc, char **argv);
};
