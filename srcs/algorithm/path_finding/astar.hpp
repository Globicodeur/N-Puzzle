#pragma once

#include "Node.hpp"
#include "backtrack.hpp"

namespace algorithm {

    using puzzle::Puzzle;

    template <HClass H, bool uniform, uint size>
    auto astar(const Puzzle<size> & start, const Puzzle<size> & goal) {
        // Type aliases
        using NodeT = Node<Puzzle<size>>;
        using OpenSet = boost::heap::fibonacci_heap<NodeT>;
        using ClosedSet = std::unordered_set<NodeT, typename NodeT::Hasher>;
        using Handles = std::unordered_map<
            std::size_t,
            typename OpenSet::handle_type
        >;
        //

        const H<size> h { goal };

        std::size_t selected = 0;
        std::size_t statesInMemory = 1;

        ClosedSet closedSet;
        OpenSet openSet;
        Handles handles;

        auto handle = openSet.emplace(start, 0, h(start), nullptr);
        handles.emplace((*handle).hash, handle);

        while (!openSet.empty()) {
            auto & current = openSet.top();
            ++selected;

            if (current.data == goal)
                return backtrack(selected, statesInMemory, current);

            auto neighbors = puzzle::neighbors(current.data);
            auto newDistance = current.distance + 1;
            auto inserted = closedSet.insert(std::move(current));

            openSet.pop();
            handles.erase(current.hash);

            for (const auto & neighbor: neighbors) {
                auto newCost = h(neighbor);
                if (uniform) // should be statically optimized
                    newCost += newDistance;

                NodeT neighborNode {
                    neighbor,
                    newDistance,
                    newCost,
                    &(*inserted.first) // <iterator<NodeT>, bool>
                };

                if (closedSet.count(neighborNode))
                    continue ;

                auto handleIt = handles.find(neighborNode.hash);
                if (handleIt == handles.end()) {
                    auto handle = openSet.push(neighborNode);
                    handles.emplace(neighborNode.hash, handle);
                    ++statesInMemory;
                }
                else if (newDistance < (*handleIt->second).distance)
                    openSet.decrease(handleIt->second, neighborNode);
            }

        }
        // We should never reach here if we passed the solvability test
        throw error::PuzzleNotSolvable { };
    }

}
