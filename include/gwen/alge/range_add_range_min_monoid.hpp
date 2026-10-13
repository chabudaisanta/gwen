#pragma once

#include <concepts>
#include <limits>

#include "gwen/alge/acted_monoid.hpp"

namespace gwen {

/**
 * @brief 区間加算・区間最小値を管理する作用付きモノイド
 * @tparam T 符号付き整数型
 * @details 番兵値を実データや更新後の値として使わず、値と遅延作用の加算でオーバーフローしないこと。
 */
template <std::signed_integral T> struct range_add_range_min_monoid {
    /** @brief 区間最小値の型 */
    using S = T;
    /** @brief 区間加算作用の型 */
    using F = T;

    /** @brief 2つの最小値を結合する */
    static constexpr S op(S a, S b) { return a < b ? a : b; }

    /** @brief 空区間を表す単位元を返す */
    static constexpr S e() { return std::numeric_limits<T>::max(); }

    /** @brief 区間内の各要素に x を加算する */
    static constexpr S mapping(F x, S a) { return a == e() ? a : a + x; }

    /** @brief 2つの区間加算作用を合成する */
    static constexpr F composition(F f, F g) { return f + g; }

    /** @brief 加算しない作用を返す */
    static constexpr F id() { return 0; }
};

}  // namespace gwen
