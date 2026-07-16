#pragma once

#include "gwen/alge/ring.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 区間アフィン変換・区間和を管理する作用付きモノイド
 * @tparam T 要素の型（加法と乗法が定義されている必要がある）
 */
template <ring T>
struct range_affine_range_sum_monoid {
    /**
     * @brief モノイドの要素
     */
    struct S {
        T val;  ///< 区間の要素の和
        T len;  ///< 区間の長さ
    };

    /**
     * @brief 要素同士の二項演算
     */
    static constexpr S op(S a, S b) { return {a.val + b.val, a.len + b.len}; }

    /**
     * @brief 要素の単位元
     */
    static constexpr S e() { return {T(0), T(0)}; }

    /**
     * @brief 1要素からの初期化用ヘルパー
     * @param x 初期値
     * @return S 長さ1の要素
     */
    static constexpr S unit(T x) { return {x, T(1)}; }

    /**
     * @brief 作用素（アフィン変換 f(x) = ax + b）
     */
    struct F {
        T a;  ///< 係数 a
        T b;  ///< 定数項 b
    };

    /**
     * @brief 作用素を要素に適用する
     * @param f 作用素
     * @param x 要素
     * @return S 適用後の要素
     */
    static constexpr S mapping(F f, S x) { return {f.a * x.val + f.b * x.len, x.len}; }

    /**
     * @brief 作用素の合成 f(g(x)) = f.a * (g.a * x + g.b) + f.b = (f.a * g.a) * x + (f.a * g.b + f.b)
     * @param f 後から適用する作用素
     * @param g 先に適用する作用素
     * @return F 合成された作用素
     */
    static constexpr F composition(F f, F g) { return {f.a * g.a, f.a * g.b + f.b}; }

    /**
     * @brief 作用素の恒等写像 (id(x) = 1*x + 0)
     * @return F 恒等作用素
     */
    static constexpr F id() { return {T(1), T(0)}; }
};

} // namespace gwen
