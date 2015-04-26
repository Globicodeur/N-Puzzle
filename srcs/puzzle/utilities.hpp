#pragma once

#include "Puzzle.hpp"

namespace puzzle {

    // Puzzle hashing helper
    struct PuzzleHasher {

        template <uint size>
        auto operator()(const Puzzle<size> & puzzle) const {
            return boost::hash_range(
                puzzle.grid,
                puzzle.grid + sizeof(puzzle.grid)
            );
        }

    };

    // Puzzle ostream overload
    template <uint size>
    std::ostream & operator<<(std::ostream & os, const Puzzle<size> & puzzle) {
        for (uint y = 0; y < size; ++y) {
            for (uint x = 0; x < size; ++x)
                os << static_cast<uint>(puzzle.grid[y * size + x]) << " ";
            os << std::endl;
        }
        return os;
    }

}
