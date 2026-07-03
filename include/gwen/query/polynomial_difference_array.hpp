#pragma once

#include <vector>
#include <cassert>
#include <iterator>
#include "gwen/types.hpp"

namespace gwen {

template <typename Ring>
struct PolynomialDifferenceArray {
    using S = typename Ring::S;

    i32 n, k;
    std::vector<std::vector<S>> A;

    // n: 配列のサイズ, k: 最大の次数
    PolynomialDifferenceArray(i32 n, i32 k) : n(n), k(k), A(k + 1, std::vector<S>(n, Ring::e())) {}

    // 区間 [l, r) に多項式 f(i) = sum(poly[d] * i^d) を加算する
    // poly は std::vector<S>, std::array<S, N> などのイテレート可能なコンテナを受け取る
    template <typename Container>
    void add_poly(i32 l, i32 r, const Container& poly) {
        assert((i32)std::size(poly) <= k + 1);
        
        if (l < 0) l = 0;
        if (l >= r || l >= n) return;

        i32 d = 0;
        for (const auto& c : poly) {
            if (d > k) break;
            A[d][l] = Ring::op(A[d][l], c);
            if (r < n) A[d][r] = Ring::op(A[d][r], Ring::inv(c));
            d++;
        }
    }

    // 区間 [l, r) に単一の項 c * i^d を加算する
    void add_term(i32 l, i32 r, S c, i32 d) {
        if (d > k) return;
        
        if (l < 0) l = 0;
        if (l >= r || l >= n) return;

        A[d][l] = Ring::op(A[d][l], c);
        if (r < n) A[d][r] = Ring::op(A[d][r], Ring::inv(c));
    }

    // すべてのクエリを処理し、最終的な配列を構築して返す
    std::vector<S> build() {
        // 各次数ごとに累積和をとる
        for (i32 d = 0; d <= k; ++d) {
            for (i32 i = 1; i < n; ++i) {
                A[d][i] = Ring::op(A[d][i], A[d][i - 1]);
            }
        }

        // 各座標 i に対して sum(A[d][i] * i^d) を計算する
        std::vector<S> ans(n, Ring::e());
        for (i32 i = 0; i < n; ++i) {
            S current_i_pow = Ring::e_mul(); // i^0 = 1
            S s_i = S((i64)i); 
            
            for (i32 d = 0; d <= k; ++d) {
                ans[i] = Ring::op(ans[i], Ring::mul(A[d][i], current_i_pow));
                current_i_pow = Ring::mul(current_i_pow, s_i);
            }
        }
        return ans;
    }
};

}  // namespace gwen
