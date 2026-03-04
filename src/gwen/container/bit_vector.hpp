#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/// 簡潔ビットベクトル。rank / select を O(1) / O(log n) で提供。
struct bit_vector {
    static constexpr i32 w = 64;

    i32 n = 0;              // ビット長
    std::vector<u64> block; // ビット列（w ビットごと）
    std::vector<i32> count; // count[i] = block[0..i-1] の 1 の個数（rank1 用）

    bit_vector() = default;

    explicit bit_vector(i32 n_) : n(n_) {
        block.resize(n / w + 1, 0);
        count.resize(block.size(), 0);
    }

    void set(i32 i) {
        assert(0 <= i && i < n);
        block[i / w] |= u64(1) << (i % w);
    }

    bool get(i32 i) const {
        assert(0 <= i && i < n);
        return (block[i / w] >> (i % w)) & 1;
    }

    /// [0, i) の 1 の個数
    i32 rank1(i32 i) const {
        assert(0 <= i && i <= n);
        if (i == 0) return 0;
        i32 r = count[i / w];
        if (i % w != 0) r += __builtin_popcountll(block[i / w] & ((u64(1) << (i % w)) - 1));
        return r;
    }

    /// [0, i) の 0 の個数
    i32 rank0(i32 i) const { return i - rank1(i); }

    /// build 後に呼ぶ（rank 用の累積を計算）
    void build() {
        for (size_t i = 1; i < block.size(); ++i)
            count[i] = count[i - 1] + __builtin_popcountll(block[i - 1]);
    }

    /// k 番目 (0-indexed) の 1 の位置。存在しなければ n
    i32 select1(i32 k) const {
        if (k < 0 || rank1(n) <= k) return n;
        i32 lo = 0, hi = n;
        while (hi - lo > 1) {
            i32 mid = lo + (hi - lo) / 2;
            if (rank1(mid) <= k) lo = mid;
            else hi = mid;
        }
        return lo;
    }

    /// k 番目 (0-indexed) の 0 の位置。存在しなければ n
    i32 select0(i32 k) const {
        if (k < 0 || rank0(n) <= k) return n;
        i32 lo = 0, hi = n;
        while (hi - lo > 1) {
            i32 mid = lo + (hi - lo) / 2;
            if (rank0(mid) <= k) lo = mid;
            else hi = mid;
        }
        return lo;
    }

    i32 size() const { return n; }
};

}  // namespace gwen
