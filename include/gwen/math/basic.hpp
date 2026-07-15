#pragma once
#include <concepts>
#include <bit>

namespace gwen {

// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p3605r0.pdf
template <std::unsigned_integral T>
constexpr T isqrt(const T n) noexcept {
    if (n <= T{1}) return n;
    T i_current{0}, i_next{T(T{1} << ((std::bit_width(T(n - 1)) + 1) >> 1))};
    do {
        i_current = i_next;
        i_next = T((i_current + n / i_current) >> 1);
    } while (i_next < i_current);
    return i_current;
}

template <typename T>
constexpr T sq(T x) {
    return x * x;
}

template <std::integral T>
constexpr T choose2(T x) {
    return x * (x - 1) / 2;
}

} // namespace gwen
