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
                    auto add_heuristic_cost = [&](auto cost, auto heuristic) {
                        return cost + heuristic(puzzle);
                    };

                    return hana::fold(heuristics, 0, add_heuristic_cost);
                }

            private:

                hana::tuple<Heuristics<size>...> heuristics;

            };

        };

    }

}
