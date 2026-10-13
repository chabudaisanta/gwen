#pragma once

#include <vector>

#include "gwen/alge/range_add_range_sum_monoid.hpp"
#include "gwen/ds/lazy_segment_tree.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 区間加算・区間和をサポートするデータ構造
 * @tparam T 半環
 */
template <semiring T> class RangeAddRangeSum {
    using M = range_add_range_sum_monoid<T>;
    using Segtree = LazySegmentTree<M>;

    Segtree seg;

public:
    /** @brief 空のデータ構造を作る */
    RangeAddRangeSum() = default;

    /**
     * @brief 長さ n の零初期化配列を作る
     * @param n 要素数
     */
    explicit RangeAddRangeSum(i32 n) {
        std::vector<typename M::S> initial(n, M::unit(T(0)));
        seg = Segtree(initial);
    }

    /**
     * @brief 初期配列から作る
     * @param values 初期値
     */
    explicit RangeAddRangeSum(const std::vector<T>& values) {
        std::vector<typename M::S> initial;
        initial.reserve(values.size());
        for (T value : values) initial.push_back(M::unit(value));
        seg = Segtree(initial);
    }

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
     * @brief 区間 [l, r) の和を返す
     * @param l 開始位置
     * @param r 終了位置
     * @return 区間和
     */
    T prod(i32 l, i32 r) { return seg.prod(l, r).sum; }

    /** @brief 全要素の和を返す */
    T all_prod() const { return seg.all_prod().sum; }

    /**
     * @brief 要素 p の値を返す
     * @param p 位置
     * @return 要素値
     */
    T get(i32 p) { return seg.get(p).sum; }
};

}  // namespace gwen
