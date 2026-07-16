#pragma once

#include <algorithm>
#include <concepts>
#include <optional>
#include <vector>

#include "gwen/ds/bit_vector.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief ウェーブレット行列 (Wavelet Matrix)
 * @details 非負整数列の range rank / range quantile 等を O(log(max A)) で提供するデータ構造
 * @tparam T 要素の型（整数型）
 */
template <std::integral T> struct WaveletMatrix {
    i32 n = 0;                    ///< 配列の長さ
    i32 lg = 0;                   ///< ビット幅（0 から lg-1 がレベル）
    std::vector<T> a;             ///< 元の配列
    std::vector<BitVector> bv;    ///< 各レベルのビットベクトル

    /**
     * @brief デフォルトコンストラクタ
     */
    WaveletMatrix() = default;

    /**
     * @brief コンストラクタ
     * @param vec 初期配列（非負整数列）
     */
    explicit WaveletMatrix(const std::vector<T>& vec) : n(static_cast<i32>(vec.size())), a(vec) { build(); }

    /**
     * @brief 内部データ構造を構築する
     */
    void build() {
        if (n == 0) {
            lg = 0;
            return;
        }
        T max_val = *std::max_element(a.begin(), a.end());
        max_val = std::max(max_val, T(1));
        lg = 0;
        while ((T(1) << lg) <= max_val) lg++;
        if (lg == 0) lg = 1;

        bv.assign(lg, BitVector(n));
        std::vector<T> cur = a, nxt(static_cast<usize>(n));

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

    /**
     * @brief a[i] の値を取得する
     * @param i インデックス
     * @return a[i] の値（範囲外の場合は -1 に相当する値）
     */
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

    /**
     * @brief 区間 [l, r) で k 番目 (0-indexed) に小さい値を取得する
     * @param l 区間の開始インデックス
     * @param r 区間の終了インデックス
     * @param k k番目 (0-indexed)
     * @return k番目に小さい値
     */
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

    /**
     * @brief 区間 [l, r) で k 番目 (0-indexed) に大きい値を取得する
     * @param l 区間の開始インデックス
     * @param r 区間の終了インデックス
     * @param k k番目 (0-indexed)
     * @return k番目に大きい値
     */
    T kth_largest(i32 l, i32 r, i32 k) const { return kth_smallest(l, r, (r - l) - 1 - k); }

    /**
     * @brief 区間 [l, r) のうち値が upper 未満の個数を取得する
     * @param l 区間の開始インデックス
     * @param r 区間の終了インデックス
     * @param upper 上限値（含まない）
     * @return 条件を満たす要素の個数
     */
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

    /**
     * @brief 区間 [l, r) のうち値が [lower, upper) に含まれる個数を取得する
     * @param l 区間の開始インデックス
     * @param r 区間の終了インデックス
     * @param lower 下限値（含む）
     * @param upper 上限値（含まない）
     * @return 条件を満たす要素の個数
     */
    i32 range_freq(i32 l, i32 r, T lower, T upper) const { return range_freq(l, r, upper) - range_freq(l, r, lower); }

    /**
     * @brief 区間 [l, r) で upper 未満の最大値を取得する
     * @param l 区間の開始インデックス
     * @param r 区間の終了インデックス
     * @param upper 上限値
     * @return 条件を満たす最大値（存在しない場合は std::nullopt）
     */
    std::optional<T> prev_value(i32 l, i32 r, T upper) const {
        i32 cnt = range_freq(l, r, upper);
        if (cnt == 0) return std::nullopt;
        return kth_smallest(l, r, cnt - 1);
    }

    /**
     * @brief 区間 [l, r) で lower 以上の最小値を取得する
     * @param l 区間の開始インデックス
     * @param r 区間の終了インデックス
     * @param lower 下限値
     * @return 条件を満たす最小値（存在しない場合は std::nullopt）
     */
    std::optional<T> next_value(i32 l, i32 r, T lower) const {
        i32 cnt = range_freq(l, r, lower);
        if (cnt == r - l) return std::nullopt;
        return kth_smallest(l, r, cnt);
    }

    /**
     * @brief 配列の長さを返す
     * @return 配列の長さ
     */
    i32 size() const { return n; }
};

}  // namespace gwen
