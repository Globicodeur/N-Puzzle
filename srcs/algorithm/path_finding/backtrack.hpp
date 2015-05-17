#pragma once

#include "Node.hpp"

#include "puzzle/Solution.hpp"

namespace algorithm {

    template <uint size>
    auto backtrack(std::size_t iterations, std::size_t visitedCount,
                   const Node<puzzle::Puzzle<size>> & lastNode) {
        puzzle::Solution<size> solution { iterations, visitedCount };
        auto nodePtr = &lastNode;

        do {
            solution.push_back(nodePtr->data);
            nodePtr = nodePtr->previous;
        } while (nodePtr);

        std::reverse(solution.begin(), solution.end());
        return solution;
    }

}
