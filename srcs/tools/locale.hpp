#pragma once

#include <locale>

namespace tools {

    struct GroupNumbers: std::numpunct<char> {

        virtual char do_thousands_sep()   const { return ' ';   }
        virtual std::string do_grouping() const { return "\03"; }

    };

    static std::locale GroupNumbersLocale {
        std::locale { }, new GroupNumbers
    };

};
