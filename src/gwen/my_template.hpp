#pragma once
#ifdef LOCAL
#define _GLIBCXX_DEBUG
#endif
#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <cctype>
#include <chrono>
#include <climits>
#include <cmath>
#include <compare>
#include <concepts>
#include <deque>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <locale>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <ranges>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "gwen/dump.hpp"
#include "gwen/types.hpp"
using gwen::i128;
using gwen::i32;
using gwen::i64;
using gwen::u128;
using gwen::u32;
using gwen::u64;
using ll = long long;
using ull = unsigned long long;

#define rep(i, l, r)  for (int i = (int)(l); i < (int)(r); ++i)
#define rp(i, n)      for (int i = 0; i < (int)(n); ++i)
#define rrep(i, l, r) for (int i = (int)(r) - 1; i >= (int)(l); --i)
#define all(a)        a.begin(), a.end()
#define rall(a)       a.rbegin(), a.rend()

#ifdef LOCAL
#define BAR   std::cerr << "----------------------------------------------\n"
#define S_BAR std::cerr << "------------------\n"
#else
#define BAR   void(0)
#define S_BAR void(0)
#endif

constexpr std::pair<int, int> mv[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {0, 0}};
constexpr int dw[] = {0, 0, -1, 1, -1, -1, 1, 1, 0};
constexpr int dh[] = {-1, 1, 0, 0, -1, 1, -1, 1, 0};
constexpr int mod998 = 998244353, mod107 = 1000000007, mod109 = 1000000009,
              mod31 = 2147483647;
constexpr ll mod61 = (1LL << 61) - 1;
constexpr int iINF = (1 << 30) + 1;
constexpr ll liINF = (1LL << 60) + 1;
constexpr char EL = '\n', SPA = ' ';

std::pair<int, int> mv_to(int hi, int wi, int dir) {
    assert(0 <= dir && dir < 9);
    return std::make_pair(hi + dh[dir], wi + dw[dir]);
}

template <typename T1, typename T2>
inline bool chmax(T1& a, T2 b) {
    return (a < b ? a = b, true : false);
}
template <typename T1, typename T2>
inline bool chmin(T1& a, T2 b) {
    return (a > b ? a = b, true : false);
}

template <std::integral T>
inline bool isIn(T x, T l, T r) {
    return (l <= x) && (x < r);
}
template <std::integral T>
inline bool isOut(T x, T l, T r) {
    return (x < l) || (r <= x);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    for (auto it = vec.begin(); it != vec.end(); it++)
        os << *it << (it == prev(vec.end()) ? "" : " ");
    return os;
}
template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& vec) {
    for (T& e : vec) is >> e;
    return is;
}
void yon(bool b) { std::cout << (b ? "Yes\n" : "No\n"); }

template <typename T>
std::vector<int> idxsort(const std::vector<T>& vec, bool rev = false) {
    std::vector<int> ret(vec.size());
    std::iota(ret.begin(), ret.end(), 0);
    sort(ret.begin(), ret.end(), [&vec, &rev](int a, int b) {
        return (rev ? vec[a] > vec[b] : vec[a] < vec[b]);
    });
    return ret;
}
template <std::integral T>
T ceil_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : d + ((x > 0) ^ (y < 0));
}
template <std::integral T>
T floor_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : d - ((x < 0) ^ (y < 0));
}
template <std::integral T>
T out_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : ((x > 0) == (y > 0)) ? d + 1
                                                 : d - 1;
}
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
constexpr T sq(T x) { return x * x; }
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

template <typename Iterator>
auto runlength(Iterator begin, Iterator end) {
    using ValueType = typename std::iterator_traits<Iterator>::value_type;
    using CountType = int;
    std::vector<std::pair<ValueType, CountType>> ret;

    for (auto it = begin; it != end; ++it) {
        if (ret.empty() || ret.back().first != *it) ret.emplace_back(*it, 0);
        ret.back().second++;
    }
    return ret;
}

#if __has_include(<atcoder/all>)
#include <atcoder/all>
using mint998 = atcoder::modint998244353;
using mint107 = atcoder::modint1000000007;
#endif
#if __has_include(<gmpxx.h>)
#include <gmpxx.h>
using gmp_int = mpz_class;
#endif
/*
#if __has_include(<Eigen/Dense>)
#include <Eigen/Dense>
#endif
*/