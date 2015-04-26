#pragma once

#include "forwards.hpp"

namespace parsing {

    template <class It>
    struct PuzzleGrammar: grammar<It, ParsedPuzzle(), blank_type> {

        PuzzleGrammar(): PuzzleGrammar::base_type(puzzle) {
            using ph::bind;
            using ph::push_back;
            using ph::ref;

            comment =
                    '#'
                >>  *(char_ - eol)
                >>  eol
            ;

            // Useful rule for ignoring crap
            noise = *(comment | eol);

            auto validateNumber = [this](Row & storage, uint number) {
                // Not in range
                if (number >= size * size)
                    return false;
                // Already parsed
                if (parsed.count(number))
                    return false;
                // Ok -> mark and store it
                parsed.insert(number);
                storage.push_back(number);
                return true;
            };

            row =
                repeat(ref(size))[
                    uint_[
                        // Using custom validation
                        _pass = bind(validateNumber, _val, _1)
                    ]
                ]
            ;

            puzzle =
                    noise
                    // Parsing the puzzle size and storing it in `size`
                >>  uint_[ref(size) = _1] >> (comment | eol)
                >>  noise
                    // Parsing `size` rows
                >>  (repeat(ref(size))[row >> noise])[_val = _1]
                >>  noise
            ;
        }

    private:
        using Row                               = ParsedPuzzle::value_type;

        rule<It, blank_type>                    comment;
        rule<It, blank_type>                    noise;
        rule<It, Row(), blank_type>             row;
        rule<It, ParsedPuzzle(), blank_type>    puzzle;

        uint                                    size;
        std::unordered_set<uint>                parsed;
    };

}
