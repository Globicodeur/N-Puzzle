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

        Data data;
        uint distance;  // g
        uint cost;      // f
        const Data *previous;
    };

    struct NodeHasher {

        template <class Data>
        auto operator()(const Node<Data> & node) const {
            static const puzzle::PuzzleHasher hasher { };
            return hasher(node.data);
        }

    };

    template <uint size>
    static auto findNeighbors(const Puzzle<size> & puzzle) {
        std::vector<Puzzle<size>> neighbors;
        neighbors.reserve(4);

        uint x, y;
        auto idx = puzzle.indexOf(0);
        x = idx % size;
        y = idx / size;

        if (x > 0) {
            Puzzle<size> c(puzzle);
            std::swap(c.grid[idx], c.grid[y * size + x - 1]);
            neighbors.push_back(c);
        }
        if (x < size - 1) {
            Puzzle<size> c(puzzle);
            std::swap(c.grid[idx], c.grid[y * size + x + 1]);
            neighbors.push_back(c);
        }
        if (y > 0) {
            Puzzle<size> c(puzzle);
            std::swap(c.grid[idx], c.grid[(y - 1) * size + x]);
            neighbors.push_back(c);
        }
        if (y < size - 1) {
            Puzzle<size> c(puzzle);
            std::swap(c.grid[idx], c.grid[(y + 1) * size + x]);
            neighbors.push_back(c);
        }

        return neighbors;
    }

    template <uint size>
    auto dumbHeuristic(const Puzzle<size> & start, const Puzzle<size> &) {
        // static const uint IDXS[] = {
        //     12, 0, 1, 2, 3, 4, 9, 14, 19, 24, 23, 22, 21,
        //     20, 15, 10, 5, 6, 7, 8, 13, 18, 17, 16, 11
        // };
        static const uint IDXS[] = {
            9, 0, 1, 2, 3, 7, 11, 15, 14, 13, 12, 8, 4, 5, 6, 10
        };
        uint cost = 0;
        for (uint i = 0; i < size * size; ++i) {
            auto goalIdx = IDXS[start.grid[i]];
            int goalX = goalIdx % size;
            int goalY = goalIdx / size;
            int x = i % size;
            int y = i / size;
            int dx = x - goalX;
            int dy = y - goalY;
            cost += (dx * dx) + (dy * dy);
        }
        return cost;
    }

    template <uint size>
    Solution astar(const Puzzle<size> & start, const Puzzle<size> & goal) {
        static const NodeHasher nodeHasher { };

        using NodeT = Node<Puzzle<size>>;
        using ClosedSet = std::unordered_map<Puzzle<size>, const Puzzle<size> *, puzzle::PuzzleHasher>;
        using OpenSet = boost::heap::binomial_heap<NodeT>;
        using Handles = std::unordered_map<size_t, typename OpenSet::handle_type>;

        ClosedSet closedSet;
        OpenSet openSet;
        Handles handles;

        auto handle = openSet.emplace(start, 0, dumbHeuristic(start, goal), nullptr);
        handles.emplace(nodeHasher(*handle), handle);

        while (!openSet.empty()) {
            auto current = openSet.top();
            if (current.data == goal) {
                std::cout << "GG\n";
                // TODO: reconstruct path (i.e. build Solution)
                return { };
            }

            openSet.pop();
            auto inserted = closedSet.emplace(current.data, current.previous);
            for (const auto & neighbor: findNeighbors(current.data)) {
                if (closedSet.count(neighbor))
                    continue ;
                auto possibleNewDist = current.distance + 1;
                NodeT neighborNode { neighbor, possibleNewDist, possibleNewDist + dumbHeuristic(neighbor, goal), &inserted.first->first };

                auto hash = nodeHasher(neighborNode);
                auto handleIt = handles.find(hash);
                if (handleIt == handles.end() || possibleNewDist < (*handleIt->second).distance) {
                    if (handleIt == handles.end()) {
                        auto handle = openSet.push(std::move(neighborNode));
                        handles.emplace(hash, handle);
                    }
                    else {
                        (*handleIt->second).distance = possibleNewDist;
                        openSet.decrease(handleIt->second, neighborNode);
                    }
                }
            }

        }

        return { };
    }

}
