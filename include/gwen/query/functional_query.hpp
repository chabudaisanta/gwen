#pragma once

#include <bit>
#include <tuple>
#include <utility>
#include <vector>

#include "gwen/types.hpp"
namespace gwen {

template <typename Monoid> struct functional_query {
    using S = typename Monoid::S;

    i32 N;
    i32 lg;
    std::vector<std::vector<i32>> pos;
    std::vector<std::vector<S>> acc;

    // f(from, to)
    explicit functional_query(const std::vector<i32>& P, i64 k_max, auto&& f)
        : N(P.size()), lg(std::bit_width(u64(k_max))) {
        acc.resize(lg, std::vector<S>(N, Monoid::e()));
        pos.resize(lg, std::vector<i32>(N, -1));
        for (i32 i = 0; i < N; ++i) {
            pos[0][i] = P[i];
            acc[0][i] = f(i, P[i]);
        }
        for (i32 l = 1; l < lg; ++l) {
            for (i32 i = 0; i < N; ++i) {
                i32 j = pos[l - 1][i];
                pos[l][i] = pos[l - 1][j];
                acc[l][i] = Monoid::op(acc[l - 1][i], acc[l - 1][j]);
            }
        }
    }

    std::pair<i32, S> query(i32 p, i64 k, S initial_value = Monoid::e()) {
        i32 cur = p;
        S x = initial_value;
        for (i32 l = 0; l < lg; ++l) {
            if (k & (1ll << l)) {
                x = Monoid::op(x, acc[l][cur]);
                cur = pos[l][cur];
            }
        }
        return {cur, x};
    }

    // f(x) = trueである限り進み続ける
    std::tuple<i32, i64, S> max_step(i32 p, auto&& f, S initial_value = Monoid::e()) {
        i32 cur = p;
        i64 step = 0;
        S x = initial_value;
        for (i32 l = lg - 1; l >= 0; --l) {
            S nx = Monoid::op(x, acc[l][cur]);
            if (f(nx)) {
                x = nx;
                step |= 1ll << l;
                cur = pos[l][cur];
            }
        }
        return {cur, step, x};
    }
};

}  // namespace gwen