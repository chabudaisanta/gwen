#pragma once

#include <vector>
#include <bit>
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 64-ary tree を用いた高速な集合 (Fast Set)
 * @details 非負整数の集合を管理する。要素の追加・削除・検索・前後の要素探索をビット演算で高速に行う。
 */
class FastSet {
  public:
    /**
     * @brief [0, n) の範囲を管理する空の集合を構築する。
     * @param n_ 管理する要素の最大値の境界
     */
    explicit FastSet(i32 n_) : n(n_) {
        if (n <= 0) return;
        i32 s = n;
        while (true) {
            s = (s + 63) / 64;
            a.push_back(std::vector<u64>(s, 0));
            if (s == 1) break;
        }
    }

    /**
     * @brief 要素 x を追加する。
     * @param x 追加する要素
     */
    void insert(i32 x) {
        if (x < 0 || x >= n) return;
        for (usize i = 0; i < a.size(); ++i) {
            usize idx = static_cast<usize>(x) / 64;
            usize bit = static_cast<usize>(x) % 64;
            if ((a[i][idx] >> bit) & 1) break;
            a[i][idx] |= (u64(1) << bit);
            x /= 64;
        }
    }

    /**
     * @brief 要素 x を削除する。
     * @param x 削除する要素
     */
    void erase(i32 x) {
        if (x < 0 || x >= n || !contains(x)) return;
        for (usize i = 0; i < a.size(); ++i) {
            usize idx = static_cast<usize>(x) / 64;
            usize bit = static_cast<usize>(x) % 64;
            a[i][idx] &= ~(u64(1) << bit);
            if (a[i][idx] != 0) break;
            x /= 64;
        }
    }

    /**
     * @brief 要素 x が含まれるか判定する。
     * @param x 判定する要素
     * @return true 含まれる場合
     * @return false 含まれない場合
     */
    bool contains(i32 x) const {
        if (x < 0 || x >= n) return false;
        return (a[0][static_cast<usize>(x) / 64] >> (static_cast<usize>(x) % 64)) & 1;
    }

    /**
     * @brief x 以上の最小の要素を返す。存在しない場合は n を返す。
     * @param x 探索の始点
     * @return i32 x 以上の最小の要素。なければ n。
     */
    i32 next(i32 x) const {
        if (x < 0) x = 0;
        if (x >= n) return n;
        for (usize i = 0; i < a.size(); ++i) {
            usize idx = static_cast<usize>(x) / 64;
            usize bit = static_cast<usize>(x) % 64;
            u64 mask = a[i][idx] >> bit;
            if (mask != 0) {
                usize pos = idx * 64 + bit + static_cast<usize>(std::countr_zero(mask));
                for (usize j = i; j-- > 0; ) {
                    pos = pos * 64 + static_cast<usize>(std::countr_zero(a[j][pos]));
                }
                return static_cast<i32>(pos);
            }
            x = static_cast<i32>(idx) + 1;
        }
        return n;
    }

    /**
     * @brief x 以下の最大の要素を返す。存在しない場合は -1 を返す。
     * @param x 探索の始点
     * @return i32 x 以下の最大の要素。なければ -1。
     */
    i32 prev(i32 x) const {
        if (x < 0) return -1;
        if (x >= n) x = n - 1;
        if (n == 0) return -1;
        for (usize i = 0; i < a.size(); ++i) {
            usize idx = static_cast<usize>(x) / 64;
            usize bit = static_cast<usize>(x) % 64;
            u64 mask = a[i][idx] << (63 - bit);
            if (mask != 0) {
                usize pos = idx * 64 + bit - static_cast<usize>(std::countl_zero(mask));
                for (usize j = i; j-- > 0; ) {
                    pos = pos * 64 + 63 - static_cast<usize>(std::countl_zero(a[j][pos]));
                }
                return static_cast<i32>(pos);
            }
            if (idx == 0) break;
            x = static_cast<i32>(idx) - 1;
        }
        return -1;
    }

  private:
    i32 n;
    std::vector<std::vector<u64>> a;
};

}  // namespace gwen
