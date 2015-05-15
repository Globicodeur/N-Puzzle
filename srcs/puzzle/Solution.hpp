#pragma once

#include "Puzzle.hpp"

#include "options/Options.hpp"

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
                for (const auto & puzzle: solution)
                    os << puzzle << "\n";
            }
            os  << "Number of states selected:          " << solution.timeComplexity << "\n"
                << "Maximum number of states in memory: " << solution.spaceComplexity << "\n"
                << "Number of moves required:           " << solution.size() - 1 << "\n";
            return os;
        }
    };

}
