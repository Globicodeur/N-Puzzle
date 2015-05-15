#pragma once

#include "Puzzle.hpp"

#include "parsing/forwards.hpp"

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

    // Builds a puzzle of known size from nested vectors of values
    template <uint size>
    auto buildStaticPuzzle(const parsing::ParsedPuzzle & parsed) {
        puzzle::Puzzle<size> puzzle;

        for (uint y = 0; y < size; ++y)
            for (uint x = 0; x < size; ++x)
                puzzle.grid[y * size + x] = parsed.at(y).at(x);

        return puzzle;
    }

    // Builds a 'snail' puzzle like the ones in the subject
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

    // Finds immediate neighbors for a given puzzle
    template <uint size>
    auto neighbors(const Puzzle<size> & puzzle) {
        std::vector<Puzzle<size>> neighbors;
        neighbors.reserve(4);
        auto idx = puzzle.indexOf(0);

        auto addNeighbor = [&](int di) {
            Puzzle<size> copy(puzzle);
            std::swap(copy.grid[idx], copy.grid[idx + di]);
            neighbors.push_back(copy);
        };

        if (idx % size > 0)         addNeighbor(-1);
        if (idx % size < size - 1)  addNeighbor(1);
        if (idx / size > 0)         addNeighbor(-size);
        if (idx / size < size - 1)  addNeighbor(size);

        return neighbors;
    }

    // Computes the number of inversions for a given puzzle
    template <uint size>
    auto inversions(const Puzzle<size> & puzzle) {
        uint count = 0;

        for (uint i = 0; i < size * size - 1; ++i) {
            if (puzzle.grid[i] == 0)
                continue ;
            for (uint j = i + 1; j < size * size; ++j) {
                if (puzzle.grid[j] == 0)
                    continue ;
                count += (puzzle.grid[j] < puzzle.grid[i]);
            }
        }

        return count;
    }

    // Returns true if `start` belongs to the same permutation group as `goal`
    template <uint size>
    bool isSolvable(const Puzzle<size> & start, const Puzzle<size> & goal) {
        auto startInversions = inversions(start);
        auto goalInversions = inversions(goal);

        if (size % 2 == 0) {
            startInversions += start.indexOf(0) / size;
            goalInversions += goal.indexOf(0) / size;
        }

        return startInversions % 2 == goalInversions % 2;
    }

    // Returns a random puzzle
    template <uint size>
    auto generate() {
        static std::random_device seed;

        Puzzle<size> puzzle;

        std::iota(
            puzzle.grid,
            puzzle.grid + size * size,
            0
        );

        std::shuffle(
            puzzle.grid,
            puzzle.grid + size * size,
            seed
        );

        return puzzle;
    }

    // Returns a random puzzle that is solvable for the given puzzle
    template <uint size>
    auto generateMatch(const Puzzle<size> & match) {
        Puzzle<size> puzzle;

        do {
            puzzle = generate<size>();
        } while(!isSolvable(puzzle, match));

        return puzzle;
    }
}
