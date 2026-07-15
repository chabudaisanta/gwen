#pragma once
#include <concepts>

namespace gwen {

template <typename T1, typename T2>
inline bool chmax(T1& a, T2 b) { return (a < b ? a = b, true : false); }

template <typename T1, typename T2>
inline bool chmin(T1& a, T2 b) { return (a > b ? a = b, true : false); }

template <std::integral T>
inline bool isIn(T x, T l, T r) { return (l <= x) && (x < r); }

template <std::integral T>
inline bool isOut(T x, T l, T r) { return (x < l) || (r <= x); }

template <std::integral T>
inline bool isOverlap(T l0, T r0, T l1, T r1) { return !(r1 <= l0 || r0 <= l1); }

} // namespace gwen
