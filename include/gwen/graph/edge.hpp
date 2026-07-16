#pragma once

#include <concepts>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief エッジの要件を定義するコンセプト
 */
template <typename T>
concept edge = requires(const T& t) {
    { t.u } -> std::convertible_to<i32>;
    { t.v } -> std::convertible_to<i32>;
    typename T::weight_type;
};

/**
 * @brief 基本的なエッジ構造体
 * @tparam T 重みの型
 */
template <typename T = i32>
struct Edge {
    using weight_type = T;
    i32 u, v;
    weight_type w;

    Edge() : u(-1), v(-1), w(0) {}
    Edge(i32 u_, i32 v_) : u(u_), v(v_), w(1) {}
    Edge(i32 u_, i32 v_, weight_type w_) : u(u_), v(v_), w(w_) {}
};

} // namespace gwen
