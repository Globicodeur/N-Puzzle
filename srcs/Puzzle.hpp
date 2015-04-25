#pragma once

class Puzzle {

public:
    uint     size_;
    short   *data_;
    int     cost;

    Puzzle(uint size):
        size_ { size },
        data_ { new short[size * size] },
        cost { 0 }
    { }

    Puzzle(): data_ { nullptr } {

    }

    Puzzle(const Puzzle & other):
        size_ { other.size_ },
        data_ { new short[size_ * size_] },
        cost { other.cost } {
        std::memcpy(data_, other.data_, size_ * size_ * sizeof(short));
    }

    Puzzle & operator=(const Puzzle & other) {
        if (this != &other) {
            if (data_)
                delete [] data_;
            size_ = other.size_;
            cost = other.cost;
            data_ = new short[size_ * size_];
            std::memcpy(data_, other.data_, size_ * size_ * sizeof(short));
        }
        return *this;
    }

    short * operator[](int i) const {
        return data_ + i * size_;
    }

    bool operator==(const Puzzle & other) const {
        return !std::memcmp(data_, other.data_, size_ * size_ * sizeof(short));
    }

    uint idx(uint val) const {
        return std::distance(data_, std::find(data_, data_ + size_ * size_, val));
    }

    void computeCost(const Puzzle & goal) {
        cost = 0;
        // GG
        // for (uint i = 0; i < size_ * size_; ++i) {
        //     if (data_[i] == goal.data_[i])
        //         cost--;
        // }
        //
        // Manhattan distance
        for (uint i = 0; i < size_ * size_; ++i) {
            uint goalIdx = goal.idx(data_[i]);
            uint goalX = goalIdx % size_;
            uint goalY = goalIdx / size_;
            uint x = i % size_;
            uint y = i / size_;
            int dx = x - goalX;
            int dy = y - goalY;
            cost += dx * dx + dy * dy;
        }
    }

};

std::ostream & operator<<(std::ostream & os, const Puzzle & p) {
    for (uint y = 0; y < p.size_; ++y) {
        for (uint x = 0; x < p.size_; ++x)
            os << p[y][x] << " ";
        os << std::endl;
    }
    return os;
}
