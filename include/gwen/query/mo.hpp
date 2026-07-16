#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <concepts>
#include <numeric>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {
namespace internal {

// https://take44444.github.io/Algorithm-Book/range/mo/main.html
inline i64 hilbert_order(i64 x, i64 y, i64 n) {
    assert(std::has_single_bit(static_cast<u64>(n)));
    i64 rx, ry, d = 0;
    for (u64 s = n >> 1; s; s >>= 1) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += static_cast<i64>(s * s) * ((rx * 3) ^ ry);
        if (ry) continue;
        if (rx) {
            x = n - 1 - x;
            y = n - 1 - y;
        }
        std::swap(x, y);
    }
    return d;
}

}  // namespace internal

/**
 * @brief Mo's algorithm (Mo法)
 * @details オフラインの区間クエリを平方分割とヒルベルト曲線を用いて $O((N+Q)\sqrt{N})$ で処理する。
 */
class Mo {
private:
    i32 q = 0;
    i32 n, bc;
    std::vector<i32> L, R;
    std::vector<i64> ord;

public:
    /**
     * @brief コンストラクタ
     * @param n_ 対象となる配列の要素数 $N$
     */
    explicit Mo(i32 n_) : n(n_), bc(std::bit_ceil(static_cast<u32>(n))) {}

    /**
     * @brief クエリを追加する
     * @param l 区間の左端 (0-indexed, 包含)
     * @param r 区間の右端 (0-indexed, 非包含)
     */
    void add_query(i32 l, i32 r) {
        L.emplace_back(l);
        R.emplace_back(r);
        ord.emplace_back(internal::hilbert_order(l, r, bc));
        q++;
    }

    /**
     * @brief 左右で異なる追加・削除処理を行うクエリを一括処理する
     * @tparam IL 左端を縮小 (increment l) する関数の型
     * @tparam DL 左端を拡大 (decrement l) する関数の型
     * @tparam IR 右端を拡大 (increment r) する関数の型
     * @tparam DR 右端を縮小 (decrement r) する関数の型
     * @tparam GR クエリの答えを取得する関数の型
     * @param increment_l l をインクリメントする際に呼ばれる関数 `f(l, r)`
     * @param decrement_l l をデクリメントする際に呼ばれる関数 `f(l, r)`
     * @param increment_r r をインクリメントする際に呼ばれる関数 `f(l, r)`
     * @param decrement_r r をデクリメントする際に呼ばれる関数 `f(l, r)`
     * @param get_res クエリIDを受け取り、現在の区間の計算結果を返す関数 `f(id)`
     * @return std::vector<decltype(get_res(0))> 各クエリの答えの配列
     */
    template <
        std::invocable<i32, i32> IL,
        std::invocable<i32, i32> DL,
        std::invocable<i32, i32> IR,
        std::invocable<i32, i32> DR,
        std::invocable<i32> GR>
    auto solve(IL increment_l, DL decrement_l, IR increment_r, DR decrement_r, GR get_res) {
        std::vector<i32> query(q);
        std::iota(query.begin(), query.end(), 0);
        std::sort(query.begin(), query.end(), [&](i32 a, i32 b) { return ord[a] < ord[b]; });

        using S = decltype(get_res(0));
        std::vector<S> ret(q);
        i32 l = 0, r = 0;
        for (i32 idx : query) {
            i32 nl = L[idx], nr = R[idx];

            // 先に拡張
            while (nl < l) decrement_l(--l, r);
            while (r < nr) increment_r(l, r++);
            // 後から縮小
            while (l < nl) increment_l(l++, r);
            while (nr < r) decrement_r(l, --r);

            ret[idx] = get_res(idx);
        }
        return ret;
    }

    /**
     * @brief 左右で共通の追加・削除処理を行うクエリを一括処理する
     * @tparam Add 要素を追加する関数の型
     * @tparam Del 要素を削除する関数の型
     * @tparam Calc クエリの答えを取得する関数の型
     * @param add 追加する要素のインデックスを受け取る関数 `f(idx)`
     * @param del 削除する要素のインデックスを受け取る関数 `f(idx)`
     * @param calc クエリIDを受け取り、現在の区間の計算結果を返す関数 `f(id)`
     * @return std::vector<decltype(calc(0))> 各クエリの答えの配列
     */
    template <
        std::invocable<i32> Add,
        std::invocable<i32> Del,
        std::invocable<i32> Calc>
    auto solve(Add add, Del del, Calc calc) {
        auto increment_l = [&](i32 l, i32 /*r*/) { del(l); };
        auto decrement_l = [&](i32 l, i32 /*r*/) { add(l); };
        auto increment_r = [&](i32 /*l*/, i32 r) { add(r); };
        auto decrement_r = [&](i32 /*l*/, i32 r) { del(r); };
        return solve(increment_l, decrement_l, increment_r, decrement_r, calc);
    }
};

}  // namespace gwen
