// STL
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <random>

#include <iostream>
#include <iomanip>
#include <fstream>

// Boost
#include <boost/program_options.hpp>

#include <boost/functional/hash.hpp>

#include <boost/heap/fibonacci_heap.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <boost/mpl/set.hpp>

#include <boost/hana.hpp>

// Aliases
namespace po = boost::program_options;

namespace phoenix = boost::phoenix;
namespace spirit = boost::spirit;
namespace qi = boost::spirit::qi;

namespace hana = boost::hana;

// Macros
#define Constant static const auto
#define HClass template <uint> class
