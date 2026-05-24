#pragma once

#include <concepts>

#include "gwen/types.hpp"

namespace gwen {

template <typename T>
concept edge_type = requires(const T& t) {
    { t.u } -> std::convertible_to<i32>;
    { t.v } -> std::convertible_to<i32>;
    typename T::weight_type;
};

template <typename T = i32> struct edge {
    using weight_type = T;
    i32 u, v;
    weight_type w;
    edge() : u(-1), v(-1), w(0) {}
    edge(i32 u_, i32 v_) : u(u_), v(v_), w(1) {}
    edge(i32 u_, i32 v_, weight_type w_) : u(u_), v(v_), w(w_) {}
};

}  // namespace gwen