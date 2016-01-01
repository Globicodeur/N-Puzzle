#pragma once

#include "heuristics/ManhattanDistance.hpp"
#include "heuristics/LinearConflict.hpp"
#include "heuristics/MisplacedTiles.hpp"
#include "heuristics/MisplacedRowsAndColumns.hpp"
#include "heuristics/Gaschnig.hpp"

#include "heuristics/Composition.hpp"

namespace algorithm {

    namespace heuristics {

        template <HClass H, const char * n, size_t id>
        struct Trait: boost::mpl::size_t<id> {

            template <uint s>
            using heuristic = H<s>;

            static constexpr const char * name = n;

        };

        constexpr char manhattanName[]   = "manhattan";
        constexpr char linearName[]      = "linear";
        constexpr char misplacedName[]   = "misplaced";
        constexpr char misplacedRCName[] = "misplacedRC";
        constexpr char gaschnigName[]    = "gaschnig";

        using Heuristics = std::tuple<
            Trait<ManhattanDistance,       manhattanName,   0>,
            Trait<LinearConflict,          linearName,      1>,
            Trait<MisplacedTiles,          misplacedName,   2>,
            Trait<MisplacedRowsAndColumns, misplacedRCName, 3>,
            Trait<Gaschnig,                gaschnigName,    4>
        >;

        constexpr auto HEURISTICS_COUNT = std::tuple_size<Heuristics>::value;

        std::unordered_map<std::string, std::string> prettyNames {
            { manhattanName,    "Manhattan distance" },
            { linearName,       "Linear conflict" },
            { misplacedName,    "Misplaced tiles" },
            { misplacedRCName,  "Misplaced rows and columns" },
            { gaschnigName,     "Gaschnig" },
        };

    }

}
