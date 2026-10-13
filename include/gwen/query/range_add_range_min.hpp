#pragma once

#include <vector>

#include "gwen/alge/range_add_range_min_monoid.hpp"
#include "gwen/ds/lazy_segment_tree.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 区間加算・区間最小値をサポートするデータ構造
 * @tparam T 符号付き整数型
 * @details 初期値・更新後の値は単位元と異なり、要素値と作用の合成でオーバーフローしないこと。
 */
template <std::signed_integral T> class RangeAddRangeMin {
    using M = range_add_range_min_monoid<T>;
    using Segtree = LazySegmentTree<M>;

    Segtree seg;

public:
    /** @brief 空のデータ構造を作る */
    RangeAddRangeMin() = default;

    /**
     * @brief 長さ n の零初期化配列を作る
     * @param n 要素数
     */
    explicit RangeAddRangeMin(i32 n) : seg(std::vector<T>(n, T(0))) {}

    /**
     * @brief 初期配列から作る
     * @param values 初期値
     */
    explicit RangeAddRangeMin(const std::vector<T>& values) : seg(values) {}

    /**
     * @brief 区間 [l, r) に x を加算する
     * @param l 開始位置
     * @param r 終了位置
     * @param x 加算値
     */
    void add(i32 l, i32 r, T x) { seg.apply(l, r, x); }

    /**
     * @brief 要素 p に x を加算する
     * @param p 位置
     * @param x 加算値
     */
    void add(i32 p, T x) { seg.apply(p, x); }

    /**
     * @brief 区間 [l, r) の最小値を返す
     * @param l 開始位置
     * @param r 終了位置
     * @return 区間最小値。空区間では単位元を返す
     */
    T prod(i32 l, i32 r) { return seg.prod(l, r); }

    /** @brief 全要素の最小値を返す */
    T all_prod() const { return seg.all_prod(); }

    /**
     * @brief 要素 p の値を返す
     * @param p 位置
     * @return 要素値
     */
    T get(i32 p) { return seg.get(p); }
};

}  // namespace gwen
