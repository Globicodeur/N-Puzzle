#pragma once

#include "puzzle/Puzzle.hpp"
#include "puzzle/Solution.hpp"

namespace algorithm {

    using puzzle::Puzzle;
    using puzzle::Solution;

    template <uint size>
    Solution astar(const Puzzle<size> & start, const Puzzle<size> & goal) {
        (void)start;
        (void)goal;
        return { };
    }

}
