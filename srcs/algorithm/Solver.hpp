#pragma once

#include "parsing/forwards.hpp"
#include "puzzle/Solution.hpp"

namespace algorithm {

    struct Solver {

        puzzle::Solution solve(const parsing::ParsedPuzzle &);

    };

}
