#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 簡潔ビットベクトル
 * @details rank / select を O(1) / O(log n) で提供するデータ構造
 */
struct BitVector {
    static constexpr i32 w = 64;

    i32 n = 0;               ///< ビット長
    std::vector<u64> block;  ///< ビット列（w ビットごと）
    std::vector<i32> count;  ///< count[i] = block[0..i-1] の 1 の個数（rank1 用）

    /**
     * @brief デフォルトコンストラクタ
     */
    BitVector() = default;

    /**
     * @brief コンストラクタ
     * @param n_ ビット長
     */
    explicit BitVector(i32 n_) : n(n_) {
        block.resize(n / w + 1, 0);
        count.resize(block.size(), 0);
    }

    /**
     * @brief i 番目のビットを 1 に設定する
     * @param i 設定するビットのインデックス
     */
    void set(i32 i) {
        assert(0 <= i && i < n);
        block[i / w] |= u64(1) << (i % w);
    }

    /**
     * @brief i 番目のビットを取得する
     * @param i 取得するビットのインデックス
     * @return i 番目のビットが 1 なら true、0 なら false
     */
    bool get(i32 i) const {
        assert(0 <= i && i < n);
        return (block[i / w] >> (i % w)) & 1;
    }

    /**
     * @brief [0, i) の 1 の個数を取得する
     * @param i 終了インデックス
     * @return [0, i) の 1 の個数
     */
    i32 rank1(i32 i) const {
        assert(0 <= i && i <= n);
        if (i == 0) return 0;
        i32 r = count[i / w];
        if (i % w != 0) r += std::popcount(block[i / w] & ((u64(1) << (i % w)) - 1));
        return r;
    }

    /**
     * @brief [0, i) の 0 の個数を取得する
     * @param i 終了インデックス
     * @return [0, i) の 0 の個数
     */
    i32 rank0(i32 i) const { return i - rank1(i); }

    /**
     * @brief 内部データ(rank 用の累積)を構築する
     * @details 全ての set 操作が完了した後に呼ぶ必要がある
     */
    void build() {
        for (usize i = 1; i < block.size(); ++i) {
            count[i] = count[i - 1] + std::popcount(block[i - 1]);
        }
    }

    /**
     * @brief k 番目 (0-indexed) の 1 の位置を取得する
     * @param k 探したい 1 のインデックス (0-indexed)
     * @return k 番目の 1 の位置。存在しなければ n
     */
    i32 select1(i32 k) const {
        if (k < 0 || rank1(n) <= k) return n;
        i32 lo = 0, hi = n;
        while (hi - lo > 1) {
            i32 mid = lo + (hi - lo) / 2;
            if (rank1(mid) <= k)
                lo = mid;
            else
                hi = mid;
        }
        return lo;
    }

    /**
     * @brief k 番目 (0-indexed) の 0 の位置を取得する
     * @param k 探したい 0 のインデックス (0-indexed)
     * @return k 番目の 0 の位置。存在しなければ n
     */
    i32 select0(i32 k) const {
        if (k < 0 || rank0(n) <= k) return n;
        i32 lo = 0, hi = n;
        while (hi - lo > 1) {
            i32 mid = lo + (hi - lo) / 2;
            if (rank0(mid) <= k)
                lo = mid;
            else
                hi = mid;
        }
        return lo;
    }

    /**
     * @brief ビット長を返す
     * @return ビット長
     */
    i32 size() const { return n; }
};

}  // namespace gwen
