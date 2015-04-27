// STL
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <queue>

// Boost
#include <boost/program_options.hpp>

#include <boost/functional/hash.hpp>

#include <boost/heap/binomial_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/heap_concepts.hpp>
#include <boost/heap/heap_merge.hpp>
#include <boost/heap/pairing_heap.hpp>
#include <boost/heap/policies.hpp>
#include <boost/heap/priority_queue.hpp>
#include <boost/heap/skew_heap.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

// Aliases
namespace po = boost::program_options;

namespace spirit = boost::spirit;
namespace qi = boost::spirit::qi;
namespace ph = boost::phoenix;
