#pragma once

namespace parsing {

    namespace error {

        struct ParseError: public std::exception {

            ParseError(const std::string & what):
                what_("Parse error: " + what)
            { }

            virtual const char * what() const noexcept {
                return what_.c_str();
            }

        private:
            std::string what_;
        };

    }

}
