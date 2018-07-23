#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <HClass... Heuristics>
        struct Composition {

            template <uint size>
            class Composer {

                using Puzzle = puzzle::Puzzle<size>;

            public:

                Composer(const Puzzle & goal):
                    heuristics { Heuristics<size> { goal } ... }
                { }

                uint operator()(const Puzzle & puzzle) const {
                    return call<sizeof...(Heuristics) - 1>(puzzle);
                }

            private:

                template <uint idx>
                auto call(const Puzzle & puzzle) const {
                    auto heuristic = std::get<idx>(heuristics);
                    auto heuristic_value = heuristic(puzzle);

                    if constexpr (idx == 0)
                        return heuristic_value;
                    else
                        return heuristic_value + call<idx - 1>(puzzle);
                }

                std::tuple<Heuristics<size>...> heuristics;

            };

        };

    }

}
