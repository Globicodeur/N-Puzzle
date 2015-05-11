#pragma once

#include "puzzle/Puzzle.hpp"
#include "puzzle/Solution.hpp"
#include "puzzle/utilities.hpp"

namespace algorithm {

    using puzzle::Puzzle;
    using puzzle::Solution;

    template<class Data> struct Node {

        static constexpr puzzle::PuzzleHasher hasher { };

        Node(
            const Data & data,
            uint distance,
            uint cost,
            const Node * previous
        ):
            data        (data),
            distance    { distance },
            cost        { cost },
            previous    { previous },
            hash        { hasher(data) }
        { }

        bool operator<(const Node & other) const {
            return cost > other.cost;
        }

        bool operator==(const Node & other) const {
            return data == other.data;
        }

        struct Hasher {

            auto operator()(const Node & node) const {
                return node.hash;
            }

        };

        Data        data;
        uint        distance; // g
        uint        cost;     // f
        const Node  *previous;
        std::size_t hash;
    };

    template <uint size>
    auto backTrack(std::size_t iterations, std::size_t visitedCount,
                   const Node<Puzzle<size>> & lastNode) {
        Solution<size> solution { iterations, visitedCount };
        auto nodePtr = &lastNode;

        do {
            solution.push_back(nodePtr->data);
            nodePtr = nodePtr->previous;
        } while (nodePtr);

        std::reverse(solution.begin(), solution.end());
        return solution;
    }

    template <HClass H, uint size>
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
                return backTrack(selected, statesInMemory, current);

            auto neighbors = puzzle::neighbors(current.data);
            auto newDistance = current.distance + 1;
            auto inserted = closedSet.insert(std::move(current));

            openSet.pop();
            handles.erase(current.hash);

            for (const auto & neighbor: neighbors) {
                NodeT neighborNode {
                    neighbor,
                    newDistance,
                    newDistance + h(neighbor),
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

    template <HClass H, uint size>
    auto idastar(const Puzzle<size> & start, const Puzzle<size> & goal) {
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
        std::size_t statesInMemory = 0;
        std::size_t maxStatesInMemory = 0;

        uint nextTreshold = h(start);
        uint treshold = std::numeric_limits<uint>::max();

        while (treshold != nextTreshold) {
            treshold = nextTreshold;
            nextTreshold = std::numeric_limits<uint>::max();

            maxStatesInMemory = std::max(maxStatesInMemory, statesInMemory);
            statesInMemory = 1;

            ClosedSet closedSet;
            OpenSet openSet;
            Handles handles;

            auto handle = openSet.emplace(start, 0, h(start), nullptr);
            handles.emplace((*handle).hash, handle);

            while (!openSet.empty()) {
                auto & current = openSet.top();
                ++selected;

                if (current.data == goal) {
                    maxStatesInMemory = std::max(maxStatesInMemory, statesInMemory);
                    return backTrack(selected, maxStatesInMemory, current);
                }

                handles.erase(current.hash);

                auto neighbors = puzzle::neighbors(current.data);
                auto newDistance = current.distance + 1;
                auto inserted = closedSet.insert(std::move(current));

                openSet.pop();
                for (const auto & neighbor: neighbors) {
                    NodeT neighborNode {
                        neighbor,
                        newDistance,
                        newDistance + h(neighbor),
                        &(*inserted.first) // <iterator<NodeT>, bool>
                    };

                    if (closedSet.count(neighborNode))
                        continue ;

                    if (neighborNode.cost > treshold) {
                        nextTreshold = std::min(nextTreshold, neighborNode.cost);
                        continue ;
                    }

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
        }
        // We should never reach here if we passed the solvability test
        throw error::PuzzleNotSolvable { };
    }

}
