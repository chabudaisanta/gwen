#pragma once

#include <concepts>
#include <type_traits>

#include "gwen/alge/monoid.hpp"

namespace gwen {

/**
 * @brief 作用付きモノイド（Acted Monoid）の要件を定義するコンセプト
 *
 * 遅延評価セグメント木などで、区間取得・区間作用を行うために必要な演算が
 * 定義されていることを要求する。
 */
template <typename M>
concept acted_monoid = monoid<M> && requires(typename M::F f, typename M::F g, typename M::S x) {
    // 作用素 F
    typename M::F;

    // 作用: F(S) -> S
    { M::mapping(f, x) } -> std::same_as<typename M::S>;

    // 作用素の合成: F(F(x)) -> F
    { M::composition(f, g) } -> std::same_as<typename M::F>;

    // 作用素の単位元
    { M::id() } -> std::same_as<typename M::F>;
};

}  // namespace gwen
