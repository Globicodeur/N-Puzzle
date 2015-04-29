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
        static const int width = std::floor(std::log10(size * size - 1)) + 1;

        for (uint y = 0; y < size; ++y) {
            os << '|';
            for (uint x = 0; x < size; ++x)
                os  << std::setw(width) << std::left
                    << static_cast<uint>(puzzle.grid[y * size + x])
                    << " |";
            os << std::endl;
        }
        return os;
    }

    // Build a 'snail' puzzle like the ones in the subject
    template <uint size>
    auto makeSnail(void) {
        struct Position { int x, y; };
        static const Position DELTAS[] = {
            { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 }
        };

        Puzzle<size> puzzle;
        Position pos { -1, 0 };
        uint xMoves = size;
        uint yMoves = size - 1;
        uint value = 1;
        uint deltaIdx = 0;
        uint moved = 0;

        while (xMoves || yMoves) {
            Position delta = DELTAS[deltaIdx % 4];
            pos.x += delta.x;
            pos.y += delta.y;
            puzzle.grid[pos.y * size + pos.x] = (value++) % (size * size);
            ++moved;
            if (delta.x && moved == xMoves) { deltaIdx++; moved = 0; --xMoves; }
            if (delta.y && moved == yMoves) { deltaIdx++; moved = 0; --yMoves; }
        }
        return puzzle;
    }

}
