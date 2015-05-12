#pragma once

#include "heuristics/ManhattanDistance.hpp"
#include "heuristics/LinearConflict.hpp"
#include "heuristics/MisplacedTiles.hpp"
#include "heuristics/MisplacedRowsAndColumns.hpp"
#include "heuristics/Gaschnig.hpp"

namespace algorithm {

    namespace heuristics {

        template <const char * n, HClass H>
        struct NamedHeuristic {
            template <uint size> using type = H<size>;
            static constexpr const char * name = n;
        };

        constexpr char linearName[]         = "linear";
        constexpr char manhattanName[]      = "manhattan";
        constexpr char misplacedName[]      = "misplaced";
        constexpr char misplacedRCName[]    = "misplacedRC";

        using Heuristics = std::tuple<
            NamedHeuristic<manhattanName,   ManhattanDistance>,
            NamedHeuristic<linearName,      LinearConflict>,
            NamedHeuristic<misplacedName,   MisplacedTiles>,
            NamedHeuristic<misplacedRCName, MisplacedRowsAndColumns>
        >;

        template <std::size_t i>
        using HeuristicAt = typename std::tuple_element<i, Heuristics>::type;

        constexpr auto HEURISTICS_COUNT = std::tuple_size<Heuristics>::value;

    }

}
