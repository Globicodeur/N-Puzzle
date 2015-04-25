#include "Application.hpp"

#include "options/Options.hpp"

#include "parsing/grammar.hpp"

#include "Puzzle.hpp"

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);
}

struct LessPuzzleComparator {

    bool operator()(const Puzzle & lhs, const Puzzle & rhs) const {
        // less -> true -> fin
        // less -> false -> debut
        return lhs.cost > rhs.cost;
    }

};

struct PuzzleHasher {

    auto operator()(const Puzzle & puzzle) const {
        return boost::hash_range(puzzle.data_, puzzle.data_ + puzzle.size_ * puzzle.size_);
    }

};

static auto findNeighbors(const Puzzle & puzzle) {
    std::vector<Puzzle> neighbors;

    //
    uint x, y;
    bool break2 = false;
    for (y = 0; y < puzzle.size_; ++y) {
        for (x = 0; x < puzzle.size_; ++x) {
            if (puzzle[y][x] == 0) {
                break2 = true;
                break ;
            }
        }
        if (break2)
            break;
    }

    if (x > 0) {
        Puzzle c { puzzle };
        std::swap(c[y][x], c[y][x - 1]);
        neighbors.push_back(c);
    }
    if (x < puzzle.size_ - 1) {
        Puzzle c { puzzle };
        std::swap(c[y][x], c[y][x + 1]);
        neighbors.push_back(c);
    }
    if (y > 0) {
        Puzzle c { puzzle };
        std::swap(c[y][x], c[y - 1][x]);
        neighbors.push_back(c);
    }
    if (y < puzzle.size_ - 1) {
        Puzzle c { puzzle };
        std::swap(c[y][x], c[y + 1][x]);
        neighbors.push_back(c);
    }

    return neighbors;
}

using ClosedSet = std::unordered_map<Puzzle, Puzzle, PuzzleHasher>;

static void backTrack(Puzzle first, Puzzle last, const ClosedSet & closedSet) {
    std::cout << last << std::endl;
    int i = 0;
    while (true) {
        auto it = closedSet.find(last);
        if (it != closedSet.end()) {
            std::cout << it->second << std::endl;
            last = it->second;
            ++i;
            if (last == first)
                break ;
        }
        else
            break ;
    }
    std::cout << i << std::endl;
}

static auto findPath(const Puzzle & from, const Puzzle & to) {
    using OpenSet = std::priority_queue<
        Puzzle,
        std::vector<Puzzle>,
        LessPuzzleComparator
    >;

    OpenSet openSet;
    ClosedSet closedSet;

    openSet.push(from);

    while (!openSet.empty()) {
        Puzzle current = openSet.top();
        openSet.pop();

        if (current == to) {
            std::cout << "GG\n";
            backTrack(from, current, closedSet);
            return true;
        }

        auto neighbors = findNeighbors(current);
        for (auto & neighbor: neighbors) {
            if (closedSet.count(neighbor))
                continue ;
            neighbor.computeCost(to);
            closedSet.insert({neighbor, current});
            openSet.push(neighbor);
        }
    }
    // not found
    return false;
}

void Application::run(void) {
    std::cout << "Starting with: " << Options::inputFile << std::endl;

    using it = sp::istream_iterator;
    std::ifstream ifs { Options::inputFile };
    ifs.unsetf(std::ios::skipws);
    it begin { ifs };
    it end;

    Puzzle solution { 6 };

    short sol[] = {
        1, 2 ,3 ,4 ,5 ,6,
        20,21,22,23,24,7,
        19,32,33,34,25,8,
        18,31,0 ,35,26,9,
        17,30,29,28,27,10,
        16,15,14,13,12,11,
    };
    std::memcpy(solution.data_, sol, solution.size_ * solution.size_ * sizeof(short));

    auto solve = [&solution](const Puzzle & puzzle) {
        assert(puzzle.size_ == solution.size_);
        std::cout << puzzle << std::endl;
        std::cout << findPath(puzzle, solution) << std::endl;
    };

    auto parseOK = qi::phrase_parse(
        begin,
        end,
        parsing::PuzzleGrammar<it>{}[solve],
        qi::blank
    );

    std::cout << std::boolalpha << parseOK << "\n";
    auto allConsumed = (begin == end);
    std::cout << std::boolalpha << allConsumed << "\n";
    std::cout << "---\n" + std::string(begin, end) << std::endl;

    // Puzzle initial { 3 };
    // Puzzle solution { 3 };

    // initial[0][0] = 1; initial[0][1] = 4; initial[0][2] = 7;
    // initial[1][0] = 2; initial[1][1] = 5; initial[1][2] = 8;
    // initial[2][0] = 3; initial[2][1] = 6; initial[2][2] = 0;

    // // 147806325
    // // 2+4+4+3+1

    // solution[0][0] = 1; solution[0][1] = 2; solution[0][2] = 3;
    // solution[1][0] = 8; solution[1][1] = 0; solution[1][2] = 4;
    // solution[2][0] = 7; solution[2][1] = 6; solution[2][2] = 5;

    Puzzle initial { 4 };
    // Puzzle solution { 4 };

    initial[0][0] = 4; initial[0][1] = 15; initial[0][2] = 13; initial[0][3] = 9;
    initial[1][0] = 12; initial[1][1] = 5; initial[1][2] = 0; initial[1][3] = 2;
    initial[2][0] = 1; initial[2][1] = 8; initial[2][2] = 10; initial[2][3] = 14;
    initial[3][0] = 6; initial[3][1] = 3; initial[3][2] = 7; initial[3][3] = 11;

    // 1 8 9 15 14 13 0 12 5 4 3 2 7 10 11 6
    // 0 6 6 11 10 9    8  3 2 1 0 1 1  1  0
}
