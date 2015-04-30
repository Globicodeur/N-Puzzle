#pragma once

namespace algorithm {

    static constexpr auto MAX_PUZZLE_SIZE = 16u;

    namespace error {

        struct PuzzleSizeTooLarge: public std::exception {

            PuzzleSizeTooLarge(std::size_t size) {
                std::ostringstream oss;
                oss << "Solver error: the puzzle size ("
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

        struct EmptyPuzzle: public std::exception {

            virtual const char * what() const noexcept {
                return "Solver error: it does not make sense to solve a 0-puzzle";
            }

        };

        struct PuzzleNotSolvable: public std::exception {

            virtual const char * what() const noexcept {
                return "Solver error: the puzzle is not solvable";
            }

        };

    }

}
