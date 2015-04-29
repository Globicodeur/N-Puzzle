#include "Solver.hpp"

#include "astar.hpp"

#include "exceptions.hpp"

#include "puzzle/Puzzle.hpp"
#include "puzzle/utilities.hpp"

namespace algorithm {

    using puzzle::Solution;
    using puzzle::Puzzle;

    using parsing::ParsedPuzzle;

    template <uint size>
    static auto buildStaticPuzzle(const ParsedPuzzle & parsed) {
        Puzzle<size> puzzle;

        for (uint y = 0; y < size; ++y)
            for (uint x = 0; x < size; ++x)
                puzzle.grid[y * size + x] = parsed.at(y).at(x);

        return puzzle;
    }

    template <uint size>
    static auto buildGoal() {
        struct Position { int x, y; };
        static const Position DELTAS[] = {
            { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 }
        };
        Puzzle<size> puzzle;

        Position pos { -1, 0 };
        uint dx = size, dy = size - 1;
        uint value = 1;
        uint i = 0;
        uint moved = 0;

        while (dx || dy) {
            auto delta = DELTAS[i % 4];
            pos.x += delta.x;
            pos.y += delta.y;
            puzzle.grid[pos.y * size + pos.x] = (value++) % (size * size);
            ++moved;
            if (delta.x && moved == dx)  { i++; moved = 0; --dx; }
            if (delta.y && moved == dy) { i++; moved = 0; --dy; }
        }
        return puzzle;
    }

    template <uint size>
    static auto inversions(const Puzzle<size> & puzzle) {
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

    template <uint size>
    static bool isSolvable(const Puzzle<size> & start, const Puzzle<size> & goal) {
        auto startInversions = inversions(start);
        auto goalInversions = inversions(goal);
        if (size % 2 == 0) {
            startInversions += start.indexOf(0) / size;
            goalInversions += goal.indexOf(0) / size;
        }
        return startInversions % 2 == goalInversions % 2;
    }

    template <uint size>
    static auto solve(const ParsedPuzzle & parsed) {
        auto start = buildStaticPuzzle<size>(parsed);
        std::cout << start << std::endl;

        auto goal = buildGoal<size>();
        std::cout << goal << std::endl;

        if (!isSolvable(start, goal))
            throw error::PuzzleNotSolvable { };

        return astar(start, goal);
    }

    // Runtime value unrolling
    // -> iterating over the puzzle size (determined at runtime) to apply a
    // solving function with a static size (i.e. known at compile time)
    // This should allow some badass optimizations
    // (BTW, fuck clang 3.6 for still not supporting non type template parameter
    // packs)
    template <uint size>
    static std::enable_if_t<size == MAX_PUZZLE_SIZE, Solution>
    findAndApplyStaticSolver(const ParsedPuzzle & parsed) {
        if (parsed.size() != size)
            throw error::PuzzleSizeTooLarge { parsed.size() };
        return solve<size>(parsed);
    }

    template <uint size>
    static std::enable_if_t<size < MAX_PUZZLE_SIZE, Solution>
    findAndApplyStaticSolver(const ParsedPuzzle & parsed) {
        if (parsed.size() == size)
            return solve<size>(parsed);
        return findAndApplyStaticSolver<size + 1>(parsed);
    }

    Solution Solver::solve(const ParsedPuzzle & parsed) {
        if (parsed.size() == 0)
            throw error::EmptyPuzzle { };
        return findAndApplyStaticSolver<1>(parsed);
    }

}
