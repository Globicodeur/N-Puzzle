#pragma once

#include "Puzzle.hpp"

template <class T>
    auto pretty_name()
    {
        return abi::__cxa_demangle(
            typeid(T).name(),
            nullptr,
            nullptr,
            nullptr
        );
    }

namespace parsing {

    template <class It>
    struct PuzzleGrammar: qi::grammar<It, Puzzle(), qi::blank_type> {

        PuzzleGrammar(): PuzzleGrammar::base_type(puzzle) {
            using ph::bind;
            using ph::ref;

            comment =
                    qi::char_('#')
                >>  *(qi::char_ - qi::eol)
                >>  qi::eol
                ;

            noise = *(comment | qi::eol);

            auto validateNumber = [this](auto & ret, uint num) {
                if (num >= size * size)
                    return false;
                if (parsed.count(num))
                    return false;
                parsed.insert(num);
                ret.push_back(num);
                return true;
            };

            line = qi::repeat(ref(size))[
                qi::uint_[qi::_pass = bind(validateNumber, qi::_val, qi::_1)]
            ];

            auto fillPuzzle = [this](Puzzle & puzzle, const auto & data) {
                puzzle = Puzzle { size };
                for (uint y = 0; y < size; ++y) {
                    for (uint x = 0; x < size; ++x)
                        puzzle[y][x] = data[y][x];
                }
            };

            puzzle =
                    noise
                >>  qi::uint_[ref(size) = qi::_1]
                >>  (comment | qi::eol)
                >>  noise
                >>  (qi::repeat(ref(size))[line >> noise])[bind(fillPuzzle, qi::_val, qi::_1)]
                >>  noise
                ;
        }

        qi::rule<It, Puzzle(), qi::blank_type> puzzle;
        qi::rule<It, std::vector<uint>(), qi::blank_type> line;
        qi::rule<It, qi::blank_type> comment;
        qi::rule<It, qi::blank_type> noise;

        uint size;
        std::set<uint> parsed;
    };

}
