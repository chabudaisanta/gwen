#pragma once

#include <iostream>
#include <utility>
#include <concepts>
#include <algorithm>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <queue>
#include <stack>
#include <bit>
#include <bitset>
#include <cassert>
#include <cctype>
#include <locale>
#include <chrono>
#include <cmath>
#include <climits>
#include <compare>
#include <complex>
#include <cstdint>
#include <numeric>
#include <iterator>
#include <memory>
#include <numbers>
#include <random>
#include <ranges>
#include <string>
#include <tuple>
#include <type_traits>
#include <iomanip>

#include "gwen/types.hpp"
using gwen::i32;
using gwen::i64;
using gwen::i128;
using gwen::u32;
using gwen::u64;
using gwen::u128;
using ll = long long;
using ull = unsigned long long;

using std::cin;
using std::cout;
using std::vector;

#define rep(i,l,r)      for(int i = (int)(l); i < (int)(r); ++i)
#define rp(i,n)         for(int i = 0; i < (int)(n); ++i)
#define rrep(i,l,r)     for(int i = (int)(r) - 1; i >= 0; --i)
#define all_(a)         a.begin(), a.end()
#define rall_(a)        a.rbegin(), a.rend()

#ifdef LOCAL
#define BAR std::cerr << "----------------------------------------------\n"
#define S_BAR std::cerr << "------------------\n"
template <typename... TArgs>
inline void _debug_vars_printer_helper(std::ostream &os, bool &is_first, TArgs &&...args_pack) {
    auto single_arg_printer = [&](const auto &val) {
        if (!is_first) { os << ", "; }
        os << std::boolalpha << val;
        is_first = false;
    };
    (single_arg_printer(std::forward<TArgs>(args_pack)), ...);
}
#define DEBUGS(...)                                                                             \
    do {                                                                                        \
        std::string _debug_vars_macro_arg_str = #__VA_ARGS__;                                   \
        if (!_debug_vars_macro_arg_str.empty()) {                                               \
            std::cerr << "[" << _debug_vars_macro_arg_str << "]: ";                             \
            bool _debug_vars_macro_is_first_arg = true;                                         \
            _debug_vars_printer_helper(std::cerr, _debug_vars_macro_is_first_arg, __VA_ARGS__); \
            std::cerr << std::endl;                                                             \
        }                                                                                       \
    } while (0)
#else
#define BAR             void(0)
#define S_BAR           void(0)
#define DEBUGS(...)     void(0)
#endif

constexpr std::pair<int,int> mv[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {0, 0}};
constexpr int dw[] = {0, 0, -1, 1, -1, -1, 1, 1, 0};
constexpr int dh[] = {-1, 1, 0, 0, -1, 1, -1, 1, 0};
constexpr int mod998 = 998244353, mod107 = 1000000007, mod109 = 1000000009, mod31 = 2147483647;
constexpr ll mod61 = (1LL << 61) - 1;
constexpr int iINF = (1 << 30) + 1;
constexpr ll liINF = (1LL << 60) + 1;
constexpr char EL = '\n', SPA = ' ';

template <typename T1, typename T2> inline bool chmax(T1 &a, T2 b) { return (a < b ? a = b, true : false); }
template <typename T1, typename T2> inline bool chmin(T1 &a, T2 b) { return (a > b ? a = b, true : false); }

template <std::integral T> inline bool isIn(T x, T l, T r) { return (l <= x) && (x < r); }
template <std::integral T> inline bool isOut(T x, T l, T r) { return (x < l) || (r <= x); }

template <typename T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    for (auto it = vec.begin(); it != vec.end(); it++) os << *it << (it == prev(vec.end()) ? "" : " ");
    return os;
}
template <typename T> std::istream &operator>>(std::istream &is, std::vector<T> &vec) {
    for (T &e : vec) is >> e;
    return is;
}
void yon(bool b) { std::cout << (b ? "Yes\n" : "No\n"); }

template <typename T>
std::vector<int> idxsort(const std::vector<T> &vec, bool rev = false) {
    std::vector<int> ret(vec.size());
    std::iota(ret.begin(), ret.end(), 0);
    sort(ret.begin(), ret.end(), [&vec, &rev](int a, int b) { return (rev ? vec[a] > vec[b] : vec[a] < vec[b]); });
    return ret;
}
template <std::integral T> T ceil_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : d + ((x > 0) ^ (y < 0));
}
template <std::integral T> T floor_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : d - ((x < 0) ^ (y < 0));
}
template <std::integral T> T out_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : ((x > 0) == (y > 0)) ? d + 1 : d - 1;
}

template <typename T> constexpr T sq(T x) { return x * x; }
template <std::integral T1, std::integral T2> constexpr T1 getBit(T1 bit, T2 i) { return bit & (static_cast<T1>(1) << i); }
template <std::integral T1, std::integral T2> constexpr T1 setBit(T1 bit, T2 i) { return bit | (static_cast<T1>(1) << i); }
template <std::integral T1, std::integral T2> constexpr T1 clearBit(T1 bit, T2 i) { return bit & ~(static_cast<T1>(1) << i); }
template <std::integral T1, std::integral T2> constexpr T1 toggleBit(T1 bit, T2 i) { return bit ^ (static_cast<T1>(1) << i); }

template <typename Iterator> auto runlength(Iterator begin, Iterator end) {
    using ValueType = typename std::iterator_traits<Iterator>::value_type;
    using CountType = int;
    std::vector<std::pair<ValueType, CountType>> ret;

    for (auto it = begin; it != end; ++it) {
        if(ret.empty() || ret.back().first != *it) ret.emplace_back(*it, 0);
        ret.back().second++;
    }
    return ret;
}

#ifdef LOCAL
#define _GLIBCXX_DEBUG
#endif
#if __has_include(<atcoder/all>)
#include <atcoder/all>
using mint998 = atcoder::modint998244353;
using mint107 = atcoder::modint1000000007;
#endif
#if __has_include(<gmpxx.h>)
#include <gmpxx.h>
using gmp_int = mpz_class;
#endif
#if __has_include(<Eigen/Dense>)
#include <Eigen/Dense>
#endif