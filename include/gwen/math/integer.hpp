#pragma once
#include <concepts>
#include <cassert>

namespace gwen {

template <std::integral T>
constexpr T ceil_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : d + ((x > 0) ^ (y < 0));
}

template <std::integral T>
constexpr T floor_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : d - ((x < 0) ^ (y < 0));
}

template <std::integral T>
constexpr T out_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : ((x > 0) == (y > 0)) ? d + 1 : d - 1;
}

} // namespace gwen
