#pragma once

#include "heuristics/ManhattanDistance.hpp"
#include "heuristics/LinearConflict.hpp"
#include "heuristics/MisplacedTiles.hpp"
#include "heuristics/MisplacedRowsAndColumns.hpp"
#include "heuristics/Gaschnig.hpp"

namespace algorithm {

    namespace heuristics {

        template <HClass H> struct Wrapper {

            template <uint s>
            using heuristic = H<s>;

        };

        template <HClass H, const char * n>
        struct Trait: Wrapper<H> {

            static constexpr const char * name = n;

        };

        // /!\ Clang bug: Class template specialisation does not work with
        // template template parameters. Here we are forced to instantiate
        // concrete Heuristics to get a unique Id
        template <class T> struct HeuristicId;

        #define REGISTER_ID(H, id) \
        template <> struct HeuristicId<H<1>> { \
            using type = boost::mpl::size_t<id>; \
        };
        REGISTER_ID(ManhattanDistance,       0);
        REGISTER_ID(LinearConflict,          1);
        REGISTER_ID(MisplacedTiles,          2);
        REGISTER_ID(MisplacedRowsAndColumns, 3);
        #undef REGISTER_ID

        template <class WrappedH> struct WrappedId {

            using type = typename HeuristicId<
                typename WrappedH::template heuristic<1>
            >::type;

        };

        constexpr char manhattanName[]      = "manhattan";
        constexpr char linearName[]         = "linear";
        constexpr char misplacedName[]      = "misplaced";
        constexpr char misplacedRCName[]    = "misplacedRC";

        using Heuristics = std::tuple<
            Trait<ManhattanDistance,       manhattanName>,
            Trait<LinearConflict,          linearName>,
            Trait<MisplacedTiles,          misplacedName>,
            Trait<MisplacedRowsAndColumns, misplacedRCName>
        >;

        template <std::size_t i>
        using TraitAt = std::tuple_element_t<i, Heuristics>;

        constexpr auto HEURISTICS_COUNT = std::tuple_size<Heuristics>::value;

    }

}
