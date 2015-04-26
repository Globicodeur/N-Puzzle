#pragma once

namespace puzzle {

    template <uint size>
    class Puzzle {

        using GridUnit = unsigned char; // Allows up to 255-puzzle (16x16) on
                                        // common platforms.
                                        // This should be enough.
        // Guards for excessive sizes
        static_assert(
            size * size <= (1 << sizeof(GridUnit) * CHAR_BIT),
            "Puzzle size is too large"
        );

        static constexpr auto gridSize = size * size;

    public:
        GridUnit    grid[gridSize];

        Puzzle(void)            = default;
        Puzzle(const Puzzle &)  = default;
        Puzzle(Puzzle &&)       = default;

        Puzzle & operator=(Puzzle &&)               = default;
        Puzzle & operator=(const Puzzle & other)    = delete;

        bool operator==(const Puzzle & other) const {
            return !std::memcmp(grid, other.grid, sizeof(grid));
        }

        auto indexOf(GridUnit val) const {
            return std::distance(grid, std::find(grid, grid + gridSize, val));
        }

        // void computeCost(const Puzzle &) {
        //     cost = 0;
        //     // GG
        //     // for (uint i = 0; i < size_ * size_; ++i) {
        //     //     if (data_[i] == goal.data_[i])
        //     //         cost--;
        //     // }
        //     //
        //     // Manhattan distance

        //     static const uint IDXS[] = {
        //         12, 0, 1, 2, 3, 4, 9, 14, 19, 24, 23, 22, 21,
        //         20, 15, 10, 5, 6, 7, 8, 13, 18, 17, 16, 11
        //     };
        //     for (uint i = 0; i < size * size; ++i) {
        //         uint goalIdx = IDXS[static_cast<short>(grid[i])];
        //         uint goalX = goalIdx % size;
        //         uint goalY = goalIdx / size;
        //         uint x = i % size;
        //         uint y = i / size;
        //         int dx = x - goalX;
        //         int dy = y - goalY;
        //         cost += dx * dx + dy * dy;
        //     }
        // }
    };

}
