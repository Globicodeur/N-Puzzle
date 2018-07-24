#pragma once

#include "puzzle/utilities.hpp"

namespace algorithm {

    template<class Data> struct Node {

        static constexpr puzzle::PuzzleHasher hasher { };

        Node(
            const Data & data,
            uint32_t distance,
            uint32_t cost,
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
        uint32_t    distance; // g
        uint32_t    cost;     // f
        const Node  *previous;
        std::size_t hash;
    };

}
