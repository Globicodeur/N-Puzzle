#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <HClass... Heuristics>
        struct Composition {

            template <PuzzleSize size>
            class Composer {

                using Puzzle = puzzle::Puzzle<size>;

            public:

                Composer(const Puzzle & goal):
                    heuristics { Heuristics<size> { goal } ... }
                { }

                auto operator()(const Puzzle & puzzle) const {
                    auto sum_heuristics = [&](auto... heuristics) {
                        return (heuristics(puzzle) + ...);
                    };

                    return std::apply(sum_heuristics, heuristics);
                }

            private:

                std::tuple<Heuristics<size>...> heuristics;

            };

        };

    }

}
