#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <HClass... Heuristics>
        struct Composition {

            template <uint size>
            class Composer: public Heuristics<size>... {

                using Puzzle = puzzle::Puzzle<size>;

            public:

                Composer(const Puzzle & goal):
                    Heuristics<size> { goal } ...
                { }

                uint operator()(const Puzzle & puzzle) const {
                    return call<Heuristics...>(puzzle);
                }

            private:

                template <HClass H1, HClass... Hn>
                std::enable_if_t<sizeof...(Hn) == 0, uint>
                call(const Puzzle & puzzle) const {
                    return H1<size>::operator()(puzzle);
                }

                template <HClass H1, HClass... Hn>
                std::enable_if_t<sizeof...(Hn) != 0, uint>
                call(const Puzzle & puzzle) const {
                    return call<H1>(puzzle) + call<Hn...>(puzzle);
                }

            };

        };

    }

}
