#pragma once

#include "Puzzle.hpp"

#include "options/Options.hpp"

#include "tools/ansi.hpp"

namespace puzzle {

    template <uint size>
    struct Solution: public std::vector<Puzzle<size>> {

        Solution(std::size_t timeComplexity, std::size_t spaceComplexity):
            timeComplexity  { timeComplexity },
            spaceComplexity { spaceComplexity }
        { }

        std::size_t timeComplexity;
        std::size_t spaceComplexity;

        friend std::ostream & operator<<(std::ostream & os, const Solution & solution) {
            if (Options::showMoves) {
                os << "Moves:\n\n";
                for (const auto & puzzle: solution)
                    os << puzzle << "\n";
                os << "==============\n\n";
            }
            os  << "Number of states selected:          "
                << ansi::BOLD << ansi::GREEN << solution.timeComplexity
                << ansi::RESET << "\n"
                << "Maximum number of states in memory: "
                << ansi::BOLD << ansi::GREEN << solution.spaceComplexity
                << ansi::RESET << "\n"
                << "Number of moves required:           "
                << ansi::BOLD << ansi::GREEN << solution.size() - 1
                << ansi::RESET << "\n";
            return os;
        }
    };

}
