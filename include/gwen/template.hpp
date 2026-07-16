#pragma once
#ifdef LOCAL
#define _GLIBCXX_DEBUG
#else
#ifndef NDEBUG
#define NDEBUG
#endif
#endif

// STL includes
#include <bits/stdc++.h>

// gwen includes
#include "gwen/types.hpp"
#include "gwen/dump.hpp"

// core
#include "gwen/core/macro.hpp"
#include "gwen/core/constants.hpp"
#include "gwen/core/grid.hpp"
#include "gwen/core/utils.hpp"
// #include "gwen/core/bit.hpp"

// math
#include "gwen/math/integer.hpp"
#include "gwen/math/basic.hpp"

// io
#include "gwen/io/vector.hpp"
#include "gwen/io/yesno.hpp"

// algorithm & ds
#include "gwen/algo/idxsort.hpp"
#include "gwen/algo/runlength.hpp"
#include "gwen/ds/prefix_sum.hpp"

// global using
using gwen::i128;
using gwen::i32;
using gwen::i64;
using gwen::u128;
using gwen::u32;
using gwen::u64;
using ll = long long;
using ull = unsigned long long;
using namespace gwen::literals;

/**
 * @brief コンテナの全要素の累積和を返す
 * @tparam Container 要素型が `T` のコンテナ型
 * @param vec 対象のコンテナ
 * @return 全要素の値の累積和
 */
template <typename Container> auto SUM(const Container& vec) {
    using T = typename Container::value_type;
    return std::accumulate(vec.begin(), vec.end(), T{});
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
