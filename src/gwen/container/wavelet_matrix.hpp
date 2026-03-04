#pragma once

#include <algorithm>
#include <vector>

#include "gwen/container/bit_vector.hpp"
#include "gwen/types.hpp"

namespace gwen {

/// Wavelet Matrix。非負整数列の range rank / range quantile 等を O(bits) で提供。
template <typename T>
struct wavelet_matrix {
    i32 n = 0;
    i32 lg = 0;  // ビット幅（0 から lg-1 がレベル）
    std::vector<T> a;
    std::vector<bit_vector> bv;

    wavelet_matrix() = default;

    explicit wavelet_matrix(const std::vector<T>& vec) : n(static_cast<i32>(vec.size())), a(vec) {
        build();
    }

    void build() {
        if (n == 0) {
            lg = 0;
            return;
        }
        T max_val = *std::max_element(a.begin(), a.end());
        max_val = std::max(max_val, T(1));
        lg = 0;
        while (T(1) << lg <= max_val) lg++;
        if (lg == 0) lg = 1;

        bv.assign(lg, bit_vector(n));
        std::vector<T> cur = a, nxt(static_cast<size_t>(n));

        for (i32 h = lg - 1; h >= 0; --h) {
            for (i32 i = 0; i < n; ++i)
                if ((cur[i] >> h) & 1) bv[h].set(i);
            bv[h].build();
            i32 zeros = bv[h].rank0(n);
            i32 pos0 = 0, pos1 = zeros;
            for (i32 i = 0; i < n; ++i) {
                if (bv[h].get(i))
                    nxt[static_cast<size_t>(pos1++)] = cur[i];
                else
                    nxt[static_cast<size_t>(pos0++)] = cur[i];
            }
            cur.swap(nxt);
        }
    }

    /// a[i] を返す
    T access(i32 i) const {
        if (i < 0 || i >= n) return T(-1);
        T ret = 0;
        for (i32 h = lg - 1; h >= 0; --h) {
            i32 f = bv[h].get(i) ? 1 : 0;
            ret |= f ? (T(1) << h) : 0;
            i = f ? bv[h].rank1(i) + bv[h].rank0(n) : bv[h].rank0(i);
        }
        return ret;
    }

    /// [l, r) で k 番目 (0-indexed) に小さい値
    T kth_smallest(i32 l, i32 r, i32 k) const {
        T res = 0;
        for (i32 h = lg - 1; h >= 0; --h) {
            i32 l0 = bv[h].rank0(l), r0 = bv[h].rank0(r);
            i32 zeros = bv[h].rank0(n);
            if (k < r0 - l0) {
                l = l0;
                r = r0;
            } else {
                k -= r0 - l0;
                res |= T(1) << h;
                l = l + zeros - l0;
                r = r + zeros - r0;
            }
        }
        return res;
    }

    /// [l, r) で k 番目 (0-indexed) に大きい値
    T kth_largest(i32 l, i32 r, i32 k) const {
        return kth_smallest(l, r, (r - l) - 1 - k);
    }

    /// [l, r) のうち値が upper 未満の個数
    i32 range_freq(i32 l, i32 r, T upper) const {
        if (upper >= (T(1) << lg)) return r - l;
        i32 ret = 0;
        for (i32 h = lg - 1; h >= 0; --h) {
            i32 f = (upper >> h) & 1;
            i32 l0 = bv[h].rank0(l), r0 = bv[h].rank0(r);
            i32 zeros = bv[h].rank0(n);
            if (f) {
                ret += r0 - l0;
                l = l + zeros - l0;
                r = r + zeros - r0;
            } else {
                l = l0;
                r = r0;
            }
        }
        return ret;
    }

    /// [l, r) のうち値が [lower, upper) に含まれる個数
    i32 range_freq(i32 l, i32 r, T lower, T upper) const {
        return range_freq(l, r, upper) - range_freq(l, r, lower);
    }

    /// [l, r) で upper 未満の最大値。なければ -1（T が符号付きなら -1、要検討）
    T prev_value(i32 l, i32 r, T upper) const {
        i32 cnt = range_freq(l, r, upper);
        return cnt == 0 ? T(-1) : kth_smallest(l, r, cnt - 1);
    }

    /// [l, r) で lower 以上の最小値。なければ -1
    T next_value(i32 l, i32 r, T lower) const {
        i32 cnt = range_freq(l, r, lower);
        return cnt == r - l ? T(-1) : kth_smallest(l, r, cnt);
    }

    i32 size() const { return n; }
};

}  // namespace gwen
