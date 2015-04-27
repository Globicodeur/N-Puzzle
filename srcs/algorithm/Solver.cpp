#include "Solver.hpp"

namespace algorithm {

    static constexpr auto MAX_PUZZLE_SIZE = 16u;

    struct PuzzleSizeTooLarge: public std::exception {

        PuzzleSizeTooLarge(std::size_t size) {
            std::ostringstream oss;
            oss << "Solver error: The puzzle size ("
                << size << ") is too large (maximum allowed: "
                << MAX_PUZZLE_SIZE << ")";
            what_ = oss.str();
        }

        virtual const char * what() const noexcept {
            return what_.c_str();
        }

    private:
        std::string what_;
    };

    template <uint i>
    static auto solve(const parsing::ParsedPuzzle &) {
        return Solution { };
    }

    // Runtime value unrolling
    // -> iterating over the puzzle size (determined at runtime) to apply a
    // solving function with a static size (i.e. known at compile time)
    // This should allow some badass optimizations
    // (BTW, fuck clang 3.6 for still not supporting non type template parameter
    // packs)
    template <uint i>
    static std::enable_if_t<i == MAX_PUZZLE_SIZE, Solution>
    findAndApplyStaticSolver(const parsing::ParsedPuzzle & parsed) {
        if (parsed.size() != i)
            throw PuzzleSizeTooLarge { parsed.size() };
        return solve<i>(parsed);
    }

    template <uint i>
    static std::enable_if_t<i < MAX_PUZZLE_SIZE, Solution>
    findAndApplyStaticSolver(const parsing::ParsedPuzzle & parsed) {
        if (parsed.size() == i)
            return solve<i>(parsed);
        return findAndApplyStaticSolver<i + 1>(parsed);
    }

    Solution Solver::solve(const parsing::ParsedPuzzle & parsed) {
        return findAndApplyStaticSolver<0>(parsed);
    }

}
