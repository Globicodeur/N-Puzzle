#pragma once

struct Options {

    using seed_t = std::mt19937::result_type;

    static std::string              initialFile;
    static std::string              goalFile;
    static std::vector<std::string> heuristics;
    static std::string              searchStrategy;
    static std::string              astarVariant;
    static bool                     randomGoal;
    static PuzzleSize               generationSize;
    static bool                     showMoves;
    static seed_t                   randomSeed;

    static void parseFromCommandLine(int argc, char **argv);
};
