#pragma once

#include "Puzzle.hpp"

#include "parsing/forwards.hpp"

namespace puzzle {

    // Puzzle hashing helper
    struct PuzzleHasher {

        template <PuzzleSize size>
        auto operator()(const Puzzle<size> & puzzle) const {
            return boost::hash_range(
                puzzle.grid,
                puzzle.grid + sizeof(puzzle.grid)
            );
        }

    };

    // Puzzle ostream overload
    template <PuzzleSize size>
    std::ostream & operator<<(std::ostream & os, const Puzzle<size> & puzzle) {
        static const int width = std::floor(std::log10(size * size - 1)) + 1;

        for (PuzzleSize y = 0; y < size; ++y) {
            os << '|';
            for (PuzzleSize x = 0; x < size; ++x)
                os  << std::setw(width) << std::left
                    << static_cast<uint32_t>(puzzle.grid[y * size + x])
                    << " |";
            os << std::endl;
        }
        return os;
    }

    // Builds a puzzle of known size from nested vectors of values
    template <PuzzleSize size>
    auto buildStaticPuzzle(const parsing::ParsedPuzzle & parsed) {
        puzzle::Puzzle<size> puzzle;

        for (PuzzleSize y = 0; y < size; ++y)
            for (PuzzleSize x = 0; x < size; ++x)
                puzzle.grid[y * size + x] = parsed.at(y).at(x);

        return puzzle;
    }

    // Builds a 'snail' puzzle like the ones in the subject
    template <PuzzleSize size>
    auto makeSnail(void) {
        struct Position { int x, y; };
        static const Position DELTAS[] = {
            { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 }
        };

        Puzzle<size> puzzle;
        Position pos { -1, 0 };
        PuzzleSize xMoves = size;
        PuzzleSize yMoves = size - 1;
        uint32_t value = 1;
        uint32_t deltaIdx = 0;
        uint32_t moved = 0;

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
    template <PuzzleSize size>
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
    template <PuzzleSize size>
    auto inversions(const Puzzle<size> & puzzle) {
        PuzzleSize count = 0;

        for (PuzzleSize i = 0; i < size * size - 1; ++i) {
            if (puzzle.grid[i] == 0)
                continue ;
            for (PuzzleSize j = i + 1; j < size * size; ++j) {
                if (puzzle.grid[j] == 0)
                    continue ;
                count += (puzzle.grid[j] < puzzle.grid[i]);
            }
        }

        return count;
    }

    // Returns true if `start` belongs to the same permutation group as `goal`
    template <PuzzleSize size>
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
    template <PuzzleSize size>
    auto generate() {
        static std::mt19937 engine { Options::randomSeed };

        Puzzle<size> puzzle;

        std::iota(
            puzzle.grid,
            puzzle.grid + size * size,
            0
        );

        std::shuffle(
            puzzle.grid,
            puzzle.grid + size * size,
            engine
        );

        return puzzle;
    }

    // Returns a random puzzle that is solvable for the given puzzle
    template <PuzzleSize size>
    auto generateMatch(const Puzzle<size> & match) {
        Puzzle<size> puzzle;

        do {
            puzzle = generate<size>();
        } while(!isSolvable(puzzle, match));

        return puzzle;
    }
}
