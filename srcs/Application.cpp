#include "Application.hpp"

#include "options/Options.hpp"
#include "parsing/Parser.hpp"
#include "algorithm/Solver.hpp"

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);
}

void Application::run(void) {
    std::cout << "Starting with: " << Options::inputFile << std::endl;

    parsing::Parser parser;
    algorithm::Solver solver;

    auto parsed = parser.parse(Options::inputFile);
    auto solution = solver.solve(parsed);

    std::cout << solution << std::endl;
    // using it = sp::istream_iterator;
    // std::ifstream ifs { Options::inputFile };
    // ifs.unsetf(std::ios::skipws);
    // it begin { ifs };
    // it end;

    // Puzzle<5> solution;

    // char sol[] = {
    //     1, 2 ,3 ,4 ,5,
    //     16,17,18,19,6,
    //     15,24,0, 20,7,
    //     14,23,22,21,8,
    //     13,12,11,10,9,
    // };
    // std::memcpy(solution.data, sol, 25);

    // auto solve = [&solution](const auto & puzzle) {
    //     std::cout << puzzle.size() << std::endl;
    //     // assert(puzzle.size_ == solution.size_);
    //     // std::cout << puzzle << std::endl;
    //     // std::cout << findPath(puzzle, solution) << std::endl;
    // };

    // auto parseOK = qi::phrase_parse(
    //     begin,
    //     end,
    //     parsing::PuzzleGrammar<it>{}[solve],
    //     qi::blank
    // );

    // std::cout << std::boolalpha << parseOK << "\n";
    // auto allConsumed = (begin == end);
    // std::cout << std::boolalpha << allConsumed << "\n";
    // std::cout << "---\n" + std::string(begin, end) << std::endl;
}


// static auto findNeighbors(const Puzzle<5> & puzzle) {
//     // Puzzle<0> p;
//     std::vector<Puzzle<5>> neighbors;

//     //
//     uint x, y;
//     auto idx = puzzle.indexOf(0);
//     x = idx % 5;
//     y = idx / 5;

//     if (x > 0) {
//         Puzzle<5> c(puzzle);
//         std::swap(c.data[idx], c.data[y * 5 + x - 1]);
//         neighbors.push_back(c);
//     }
//     if (x < 5 - 1) {
//         Puzzle<5> c(puzzle);
//         std::swap(c.data[idx], c.data[y * 5 + x + 1]);
//         neighbors.push_back(c);
//     }
//     if (y > 0) {
//         Puzzle<5> c(puzzle);
//         std::swap(c.data[idx], c.data[(y - 1) * 5 + x]);
//         neighbors.push_back(c);
//     }
//     if (y < 5 - 1) {
//         Puzzle<5> c(puzzle);
//         std::swap(c.data[idx], c.data[(y + 1) * 5 + x]);
//         neighbors.push_back(c);
//     }

//     return neighbors;
// }

// using ClosedSet = std::unordered_map<Puzzle<5>, Puzzle<5>, PuzzleHasher>;

// static void backTrack(Puzzle<5> first, Puzzle<5> last, ClosedSet & closedSet) {
//     std::cout << last << std::endl;
//     int i = 0;
//     std::reference_wrapper<Puzzle<5>> p { last };
//     while (true) {
//         auto it = closedSet.find(p);
//         if (it != closedSet.end()) {
//             std::cout << it->second << std::endl;
//             p = it->second;
//             ++i;
//             if (p.get() == first)
//                 break ;
//         }
//         else
//             break ;
//     }
//     std::cout << i << std::endl;
// }

// auto findPath(const Puzzle<5> & from, const Puzzle<5> & to) {
//     using OpenSet = std::priority_queue<
//         Puzzle<5>,
//         std::vector<Puzzle<5>>,
//         LessPuzzleComparator
//     >;

//     OpenSet openSet;
//     ClosedSet closedSet;

//     openSet.emplace(from);

//     while (!openSet.empty()) {
//         Puzzle<5> current = openSet.top();
//         openSet.pop();

//         if (current == to) {
//             std::cout << "GG\n";
//             backTrack(from, current, closedSet);
//             return true;
//         }

//         auto neighbors = findNeighbors(current);
//         for (auto & neighbor: neighbors) {
//             if (closedSet.count(neighbor))
//                 continue ;
//             neighbor.computeCost(to);
//             closedSet.insert({neighbor, current});
//             openSet.emplace(neighbor);
//         }
//     }
//     // not found
//     return false;
// }
