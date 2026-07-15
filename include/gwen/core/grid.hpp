#pragma once
#include <utility>
#include <cassert>

namespace gwen {

constexpr std::pair<int, int> mv[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {0, 0}};
constexpr int dw[] = {0, 0, -1, 1, -1, -1, 1, 1, 0};
constexpr int dh[] = {-1, 1, 0, 0, -1, 1, -1, 1, 0};

inline std::pair<int, int> mv_to(int hi, int wi, int dir) {
    assert(0 <= dir && dir < 9);
    return std::make_pair(hi + dh[dir], wi + dw[dir]);
}

} // namespace gwen
