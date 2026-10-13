#pragma once

#include "gwen/alge/semiring.hpp"

namespace gwen {

/**
 * @brief 区間加算・区間和を管理する作用付きモノイド
 * @tparam T 加算と乗算を持つ半環
 */
template <semiring T> struct range_add_range_sum_monoid {
    /** @brief 区間和と区間長 */
    struct S {
        T sum;  ///< 区間の要素の和
        T len;  ///< 区間の長さ
    };

    /** @brief 区間加算作用型 */
    using F = T;

    /** @brief 2つの区間情報を結合する */
    static constexpr S op(S a, S b) { return {a.sum + b.sum, a.len + b.len}; }

    /** @brief 空区間を返す */
    static constexpr S e() { return {T(0), T(0)}; }

    /** @brief 値 x を持つ長さ1の区間を作る */
    static constexpr S unit(T x) { return {x, T(1)}; }

    /** @brief 区間内の各要素に x を加算する */
    static constexpr S mapping(F x, S a) { return {a.sum + x * a.len, a.len}; }

    /** @brief 2つの区間加算作用を合成する */
    static constexpr F composition(F f, F g) { return f + g; }

    /** @brief 加算しない作用を返す */
    static constexpr F id() { return T(0); }
};

}  // namespace gwen
