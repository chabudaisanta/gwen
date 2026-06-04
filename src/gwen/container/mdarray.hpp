#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include "gwen/detail/mdspan.hpp"
#include "gwen/types.hpp"

namespace gwen {

namespace mdarray_detail {

template <i32 Rank> constexpr i64 volume(const std::array<i32, Rank>& shape) {
    i64 n = 1;
    for (i32 d : shape) n *= d;
    return n;
}

}  // namespace mdarray_detail

// Owning multidimensional array backed by vector<T>, indexed via std::mdspan.
template <typename T, i32 Rank>
    requires(Rank >= 1)
struct mdarray {
    std::array<i32, Rank> shape{};
    std::vector<T> data;
    detail::mdspan_type<T, Rank> span{};

    mdarray() = default;

    explicit mdarray(std::array<i32, Rank> shape_, T fill = T{})
        : shape(shape_), data(mdarray_detail::volume<Rank>(shape_), fill) {
        span = detail::mdspan_type<T, Rank>(data.data(), detail::make_extents<Rank>(shape));
    }

    template <typename... Is>
        requires(sizeof...(Is) == Rank)
    explicit mdarray(Is... dims) : mdarray(std::array<i32, Rank>{i32(dims)...}) {}

    i64 size() const { return i64(data.size()); }
    T* raw_data() { return data.data(); }
    const T* raw_data() const { return data.data(); }

    detail::mdspan_type<T, Rank>& view() { return span; }
    const detail::mdspan_type<T, Rank>& view() const { return span; }

    template <typename... Is>
        requires(sizeof...(Is) == Rank)
    T& operator()(Is... indices) {
        return span(i32(indices)...);
    }
    template <typename... Is>
        requires(sizeof...(Is) == Rank)
    const T& operator()(Is... indices) const {
        return span(i32(indices)...);
    }

    void fill(const T& x) { std::fill(data.begin(), data.end(), x); }
};

template <typename T, i32 Rank> std::istream& operator>>(std::istream& is, mdarray<T, Rank>& a) {
    for (T& e : a.data) is >> e;
    return is;
}

}  // namespace gwen
