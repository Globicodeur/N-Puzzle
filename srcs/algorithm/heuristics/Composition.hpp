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
                std::enable_if_t<idx == 0, uint>
                call(const Puzzle & puzzle) const {
                    return std::get<idx>(heuristics)(puzzle);
                }

                template <uint idx>
                std::enable_if_t<idx != 0, uint>
                call(const Puzzle & puzzle) const {
                    return std::get<idx>(heuristics)(puzzle) +
                           call<idx - 1>(puzzle);
                }

                std::tuple<Heuristics<size>...> heuristics;

            };

        };

    }

}
