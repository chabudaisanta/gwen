#pragma once

#include <vector>

#include "gwen/alge/range_affine_range_sum_monoid.hpp"
#include "gwen/ds/lazy_segment_tree.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 区間加算・区間更新・区間和をサポートするデータ構造
 * @tparam T 要素の型
 */
template <typename T> struct RangeAddUpdateSum {
    using M = range_affine_range_sum_monoid<T>;
    using Segtree = LazySegmentTree<M>;

    i32 N;        ///< 要素数
    Segtree seg;  ///< 内部の遅延評価セグメント木

    /**
     * @brief デフォルトコンストラクタ
     */
    RangeAddUpdateSum() : N(0) {}

    /**
     * @brief 長さ n の配列を初期化する
     * @param n 要素数
     */
    explicit RangeAddUpdateSum(i32 n) : N(n) {
        std::vector<typename M::S> tmp(N, {T(0), T(1)});
        seg = Segtree(tmp);
    }

    /**
     * @brief 初期配列から構築する
     * @param vec 初期配列
     */
    explicit RangeAddUpdateSum(const std::vector<T>& vec) : N(vec.size()) {
        std::vector<typename M::S> tmp(N);
        for (i32 i = 0; i < N; ++i) tmp[i] = {vec[i], T(1)};
        seg = Segtree(tmp);
    }

    /**
     * @brief 区間 [l, r) に x を加算する
     * @param l 開始インデックス
     * @param r 終了インデックス
     * @param x 加算する値
     */
    void add(i32 l, i32 r, T x) { seg.apply(l, r, {T(1), x}); }

    /**
     * @brief 要素 p に x を加算する
     * @param p インデックス
     * @param x 加算する値
     */
    void add(i32 p, T x) { seg.apply(p, p + 1, {T(1), x}); }

    /**
     * @brief 区間 [l, r) を x に更新する
     * @param l 開始インデックス
     * @param r 終了インデックス
     * @param x 更新する値
     */
    void update(i32 l, i32 r, T x) { seg.apply(l, r, {T(0), x}); }

    /**
     * @brief 要素 p を x に更新する
     * @param p インデックス
     * @param x 更新する値
     */
    void update(i32 p, T x) { seg.apply(p, p + 1, {T(0), x}); }

    /**
     * @brief 区間 [l, r) に x を乗算する
     * @param l 開始インデックス
     * @param r 終了インデックス
     * @param x 乗算する値
     */
    void multiply(i32 l, i32 r, T x) { seg.apply(l, r, {x, T(0)}); }

    /**
     * @brief 要素 p に x を乗算する
     * @param p インデックス
     * @param x 乗算する値
     */
    void multiply(i32 p, T x) { seg.apply(p, p + 1, {x, T(0)}); }

    /**
     * @brief 区間 [l, r) を a * y + b に更新する
     * @param l 開始インデックス
     * @param r 終了インデックス
     * @param a 乗数
     * @param b 加数
     */
    void affine(i32 l, i32 r, T a, T b) { seg.apply(l, r, {a, b}); }

    /**
     * @brief 要素 p を a * p + b に更新する
     * @param p インデックス
     * @param a 乗数
     * @param b 加数
     */
    void affine(i32 p, T a, T b) { seg.apply(p, p + 1, {a, b}); }

    /**
     * @brief 配列全体の総和を取得する
     * @return 全体の総和
     */
    T all_prod() { return seg.all_prod().val; }

    /**
     * @brief 区間 [l, r) の総和を取得する
     * @param l 開始インデックス
     * @param r 終了インデックス
     * @return 区間の総和
     */
    T prod(i32 l, i32 r) { return seg.prod(l, r).val; }

    /**
     * @brief 要素 p の値を取得する
     * @param p インデックス
     * @return p の値
     */
    T get(i32 p) { return seg.get(p).val; }
};

}  // namespace gwen
