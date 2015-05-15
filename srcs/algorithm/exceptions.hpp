#pragma once

namespace algorithm {

    static constexpr auto MAX_PUZZLE_SIZE = 12u;

    namespace error {

        struct SolverError: public std::exception {

            SolverError(const std::string & message):
                what_ { "Solver error: " + message }
            { }

            virtual const char * what() const noexcept {
                return what_.c_str();
            }

        private:

            std::string what_;

        };

        struct PuzzleSizeTooLarge: public SolverError {

            static auto message(std::size_t size) {
                std::ostringstream oss;

                oss << "Solver error: the puzzle size ("
                    << size << ") is too large (maximum allowed: "
                    << MAX_PUZZLE_SIZE << ")";

                return oss.str();
            }

            PuzzleSizeTooLarge(std::size_t size):
                SolverError { message(size) }
            { }

        };

        struct ZeroGeneration: public SolverError {

            ZeroGeneration():
                SolverError { "it does not make sense to generate a 0-puzzle" }
            { }

        };

        struct EmptyPuzzle: public SolverError {

            EmptyPuzzle():
                SolverError { "it does not make sense to solve a 0-puzzle" }
            { }

        };

        struct PuzzleNotSolvable: public SolverError {

            PuzzleNotSolvable():
                 SolverError { "the puzzle is not solvable" }
            { }

        };

        struct GoalSizeMismatch: public SolverError {

            static auto message(std::size_t goalSize, std::size_t initialSize) {
                std::ostringstream oss;

                oss << "Solver error: the goal size ("
                    << goalSize << ") does not match the initial state size ("
                    << initialSize << ")";

                return oss.str();
            }

            GoalSizeMismatch(std::size_t goalSize, std::size_t initialSize):
                SolverError { message(goalSize, initialSize) }
            { }

        };

    }

}
