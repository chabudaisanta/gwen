#pragma once
#include <vector>
#include <cassert>
#include "gwen/types.hpp"

namespace gwen {

template <typename T>
std::vector<T> prefix_sum(const std::vector<T>& vec) {
    i32 N = vec.size();
    std::vector<T> PS(N + 1);
    for (i32 i = 0; i < N; ++i) PS[i + 1] = PS[i] + vec[i];
    return PS;
}

template <typename T>
struct prefix_sum_2d {
    std::vector<std::vector<T>> dat;
    i32 H, W;

    explicit prefix_sum_2d(const std::vector<std::vector<T>>& vec)
        : H(vec.size()), W(H > 0 ? vec[0].size() : 0) {
        dat.resize(H + 1, std::vector<T>(W + 1, T{}));
        for (i32 i = 0; i < H; ++i)
            for (i32 j = 0; j < W; ++j) dat[i + 1][j + 1] = vec[i][j];
        for (i32 i = 0; i < H; ++i)
            for (i32 j = 0; j < W; ++j) dat[i + 1][j + 1] += dat[i][j + 1];
        for (i32 i = 0; i < H; ++i)
            for (i32 j = 0; j < W; ++j) dat[i + 1][j + 1] += dat[i + 1][j];
    }

    T get(i32 l, i32 r, i32 u, i32 d) const {
        assert(0 <= l && l <= r && r <= W);
        assert(0 <= u && u <= d && d <= H);
        return dat[d][r] - dat[d][l] - dat[u][r] + dat[u][l];
    }
};

} // namespace gwen
