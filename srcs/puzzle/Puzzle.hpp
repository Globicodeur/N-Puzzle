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

        Puzzle & operator=(const Puzzle & other)    = default;
        Puzzle & operator=(Puzzle &&)               = default;

        bool operator==(const Puzzle & other) const {
            return !std::memcmp(grid, other.grid, sizeof(grid));
        }

        auto indexOf(GridUnit val) const {
            return std::distance(grid, std::find(grid, grid + gridSize, val));
        }

    };

}
