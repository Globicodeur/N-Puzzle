#pragma once

namespace parsing {

    template <class It>
    struct BoardGrammar: qi::grammar<It, qi::blank_type> {

        BoardGrammar(): BoardGrammar::base_type(board) {
            using ph::bind;
            using ph::ref;

            comment =
                    qi::char_('#')
                >>  *(qi::char_ - qi::eol)
                >>  qi::eol
                ;

            noise = *(comment | qi::eol);

            auto validateNumber = [this](uint num) {
                if (num >= size * size)
                    return false;
                if (parsed.count(num))
                    return false;
                parsed.insert(num);
                return true;
            };

            line = qi::repeat(ref(size))[
                qi::uint_[qi::_pass = bind(validateNumber, qi::_1)]
            ];

            board =
                    noise
                >>  qi::uint_[ref(size) = qi::_1]
                >>  (comment | qi::eol)
                >>  noise
                >>  qi::repeat(ref(size))[line >> noise]
                >>  noise
                ;
        }

        qi::rule<It, qi::blank_type> board;
        qi::rule<It, qi::blank_type> line;
        qi::rule<It, qi::blank_type> comment;
        qi::rule<It, qi::blank_type> noise;

        uint size;
        std::set<uint> parsed;
    };

}
