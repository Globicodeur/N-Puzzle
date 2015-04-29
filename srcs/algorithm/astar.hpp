#pragma once

#include "puzzle/Puzzle.hpp"
#include "puzzle/Solution.hpp"
#include "puzzle/utilities.hpp"

namespace algorithm {

    using puzzle::Puzzle;
    using puzzle::Solution;

    template<class Data> struct Node {

        Node(const Data & data, uint distance, uint cost, const Data * previous):
            data        (data),
            distance    { distance },
            cost        { cost },
            previous    { previous }
        { }

        bool operator<(const Node & other) const {
            return cost > other.cost;
        }

        bool operator==(const Node & other) const {
            return data == other.data;
        }

        Data        data;
        uint        distance; // g
        uint        cost;     // f
        const Data  *previous;
    };

    template <template <uint> class H, uint size>
    Solution astar(const Puzzle<size> & start, const Puzzle<size> & goal) {
        using NodeT = Node<Puzzle<size>>;
        using OpenSet = boost::heap::binomial_heap<NodeT>;
        using ClosedSet = std::unordered_map<
            Puzzle<size>,
            const Puzzle<size> *,
            puzzle::PuzzleHasher
        >;
        using Handles = std::unordered_map<
            size_t,
            typename OpenSet::handle_type
        >;

        static const puzzle::PuzzleHasher puzzleHasher { };
        const H<size> h { goal };

        ClosedSet closedSet;
        OpenSet openSet;
        Handles handles;

        auto handle = openSet.emplace(start, 0, h(start), nullptr);
        handles.emplace(puzzleHasher(start), handle);

        while (!openSet.empty()) {
            auto current = openSet.top();
            if (current.data == goal) {
                // TODO: reconstruct path (i.e. build Solution)
                return { };
            }

            openSet.pop();
            auto inserted = closedSet.emplace(current.data, current.previous);
            for (const auto & neighbor: puzzle::neighbors(current.data)) {
                if (closedSet.count(neighbor))
                    continue ;

                auto newDistance = current.distance + 1;
                NodeT neighborNode {
                    neighbor,
                    newDistance,
                    newDistance + h(neighbor),
                    &inserted.first->first // <it<Puzzle, Puzzle *>, bool>
                };

                auto hash = puzzleHasher(neighbor);
                auto handleIt = handles.find(hash);
                if (handleIt == handles.end()) {
                    auto handle = openSet.push(neighborNode);
                    handles.emplace(hash, handle);
                }
                else if (newDistance < (*handleIt->second).distance) {
                    (*handleIt->second).distance = newDistance;
                    openSet.decrease(handleIt->second, neighborNode);
                }
            }

        }
        // We should never reach here if we passed the solvability test
        return { };
    }

}
