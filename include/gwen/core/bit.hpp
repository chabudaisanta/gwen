#pragma once
#include <concepts>

namespace gwen {

template <std::integral T1, std::integral T2>
constexpr T1 getBit(T1 bit, T2 i) {
    return bit & (static_cast<T1>(1) << i);
}

template <std::integral T1, std::integral T2>
constexpr T1 setBit(T1 bit, T2 i) {
    return bit | (static_cast<T1>(1) << i);
}

template <std::integral T1, std::integral T2>
constexpr T1 clearBit(T1 bit, T2 i) {
    return bit & ~(static_cast<T1>(1) << i);
}

template <std::integral T1, std::integral T2>
constexpr T1 toggleBit(T1 bit, T2 i) {
    return bit ^ (static_cast<T1>(1) << i);
}

} // namespace gwen
