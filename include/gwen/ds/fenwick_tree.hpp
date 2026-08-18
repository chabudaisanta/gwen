#pragma once

#include <cassert>
#include <format>
#include <string>
#include <vector>

#include "gwen/alge/abel.hpp"
#include "gwen/dump.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief Fenwick Tree (Binary Indexed Tree)
 *
 * アーベル群（結合律、単位元、逆元、交換律を満たす代数構造）を要素に持つ長さ N の配列に対し、
 * 一点更新と区間取得を O(log N) で行うデータ構造です。
 *
 * @tparam Abel アーベル群を表す型（gwen::abel コンセプトを満たすこと）
 */
template <abel Abel> struct FenwickTree {
    using S = typename Abel::S;

    i32 N;
    std::vector<S> data;

    /**
     * @brief デフォルトコンストラクタ。空の木を生成する。
     */
    FenwickTree() : N(0) {}

    /**
     * @brief 長さ N_ の配列で初期化する
     * @param N_ 配列の長さ
     */
    explicit FenwickTree(i32 N_) : N(N_), data(N_ + 1, Abel::e()) {}

    /**
     * @brief 与えられた配列で初期化する
     * @param vec 初期値の配列
     */
    explicit FenwickTree(const std::vector<S>& vec) : N(vec.size()), data(N + 1) {
        for (i32 i = 1; i <= N; ++i) data[i] = vec[i - 1];
        for (i32 i = 1; i <= N; ++i) {
            i32 j = i + (i & -i);
            if (j <= N) data[j] = Abel::op(data[i], data[j]);
        }
    }

    /**
     * @brief 座標 p に値 x を加算する
     * @param p 対象のインデックス (0 <= p < N)
     * @param x 加算する値
     */
    void add(i32 p, S x) {
        assert(0 <= p && p < N);
        p++;
        while (p <= N) {
            data[p] = Abel::op(data[p], x);
            p += p & -p;
        }
    }

    /**
     * @brief 区間 [0, r) の和を求める
     * @param r 区間の右端 (0 <= r <= N)
     * @return S 区間 [0, r) の和
     */
    S sum(i32 r) const {
        assert(0 <= r && r <= N);
        S ret = Abel::e();
        while (r > 0) {
            ret = Abel::op(ret, data[r]);
            r -= r & -r;
        }
        return ret;
    }

    /**
     * @brief 区間 [l, r) の和を求める
     * @param l 区間の左端 (0 <= l <= r)
     * @param r 区間の右端 (l <= r <= N)
     * @return S 区間 [l, r) の和
     */
    S sum(i32 l, i32 r) const {
        assert(0 <= l && l <= r && r <= N);
        return Abel::op(Abel::inv(sum(l)), sum(r));
    }

    /**
     * @brief f(sum(0, r)) = true となる最大の r を返す
     *
     * @tparam F 判定関数の型 (S -> bool)
     * @param f 単調性を持つ判定関数 (f(sum(0, x)) がある境界まで true、以降 false となること)
     * @return i32 条件を満たす最大の r (0 <= r <= N)
     */
    template <class F> i32 max_right(F&& f) const { return max_right_internal(Abel::e(), std::forward<F>(f)); }

    /**
     * @brief f(sum(l, r)) = true となる最大の r を返す
     *
     * @tparam F 判定関数の型 (S -> bool)
     * @param l 区間の左端 (0 <= l <= N)
     * @param f 単調性を持つ判定関数 (f(sum(l, x)) がある境界まで true、以降 false となること)
     * @return i32 条件を満たす最大の r (0 <= r <= N)。
     */
    template <class F> i32 max_right(i32 l, F&& f) const {
        return max_right_internal(Abel::inv(sum(l)), std::forward<F>(f));
    }

    /**
     * @brief 現在の配列要素を std::vector で返す
     * @return std::vector<S> 現在の配列
     */
    std::vector<S> to_vec() const {
        std::vector<S> res(N, Abel::e());
        for (i32 i = 1; i <= N; ++i) res[i - 1] = data[i];
        for (i32 i = N; i > 0; --i) {
            i32 j = i + (i & -i);
            if (j <= N) res[j - 1] = Abel::op(Abel::inv(res[i - 1]), res[j - 1]);
        }
        return res;
    }

    /**
     * @brief デバッグ用の文字列表現を返す
     * @return std::string 木の状態の文字列表現
     */
    std::string dump() const {
        return std::format("FenwickTree{{\n  N = {},\n  data = {},\n  data(restored) = {},\n}}", N,
                           internal::format_range(data), internal::format_range(to_vec()));
    }

private:
    template <class F> i32 max_right_internal(S initial, F&& f) const {
        S s = initial;
        i32 x = 0;

        i32 len = 1;
        while (len <= N) len <<= 1;
        len >>= 1;

        for (; len > 0; len >>= 1) {
            if (x + len <= N) {
                S nxt_s = Abel::op(s, data[x + len]);
                if (f(nxt_s)) {
                    x += len;
                    s = nxt_s;
                }
            }
        }
        return x;
    }
};

}  // namespace gwen
