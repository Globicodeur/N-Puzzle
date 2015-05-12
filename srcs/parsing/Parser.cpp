#include "Parser.hpp"

#include "grammar.hpp"
#include "exceptions.hpp"

namespace parsing {

    template <class It>
    static ParsedPuzzle parsePuzzle(It begin, It end) {
        const PuzzleGrammar<It> grammar;
        ParsedPuzzle puzzle;

        bool parseOk = qi::phrase_parse(begin, end, grammar, qi::blank, puzzle);
        bool allConsummed = (begin == end);

        if (!parseOk)
            throw error::ParseError { "invalid puzzle format" };
        if (!allConsummed)
            throw error::ParseError { "remaining input" };

        return puzzle;
    }

    ParsedPuzzle Parser::parse(const std::string & file) {
        ParsedPuzzle parsed;

        std::ifstream ifs { file };
        if (ifs.is_open()) {
            ifs.unsetf(std::ios::skipws); // whitespace skipping should be
                                          // handled at the grammar level
            spirit::istream_iterator begin { ifs };
            spirit::istream_iterator end;
            return parsePuzzle(begin, end);
        }
        else {
            std::cerr << "Error opening file: " << file << std::endl;
            throw std::system_error { errno, std::system_category() };
        }

        return parsed;
    }

}
