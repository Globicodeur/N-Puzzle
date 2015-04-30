#pragma once

#include "puzzle/Puzzle.hpp"

namespace algorithm {

    namespace heuristics {

        template <template <uint> class... Heuristics>
        struct Composition {

            template <uint size>
            struct Composer: public Heuristics<size>... {

                Composer(const puzzle::Puzzle<size> & goal):
                    Heuristics<size> { goal } ...
                { }

                uint operator()(const puzzle::Puzzle<size> & puzzle) const {
                    return call<Heuristics...>(puzzle);
                }

            private:

                template <template <uint> class H1, template <uint> class... Hn>
                std::enable_if_t<sizeof...(Hn) != 0, uint>
                call(const puzzle::Puzzle<size> & puzzle) const {
                    return call<H1>(puzzle) + call<Hn...>(puzzle);
                }

                template <template <uint> class H1, template <uint> class... Hn>
                std::enable_if_t<sizeof...(Hn) == 0, uint>
                call(const puzzle::Puzzle<size> & puzzle) const {
                    return H1<size>::operator()(puzzle);
                }

            };

        };

    }

}
