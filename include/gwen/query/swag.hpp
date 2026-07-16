#pragma once

#include <cassert>
#include <deque>
#include <numeric>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/algo/counting_sort.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief Sliding Window Aggregation (SWAG)
 * @details オフラインの区間クエリを処理する SWAG です。
 * モノイドの性質（結合律）を用いて、左端・右端が単調に移動するクエリ群を $O(1)$ amortized で処理します。
 *
 * @tparam Monoid モノイドを表す型 (`gwen::monoid` コンセプトを満たすこと)
 */
template <monoid Monoid> class SlidingWindowAggregation {
public:
    using S = typename Monoid::S;

private:
    i32 n;
    std::vector<i32> l_vec, r_vec;

public:
    /**
     * @brief デフォルトコンストラクタ
     */
    SlidingWindowAggregation() : n(0) {}

    /**
     * @brief コンストラクタ
     * @param n_ 対象となる配列の要素数 $N$
     */
    explicit SlidingWindowAggregation(i32 n_) : n(n_) {}

    /**
     * @brief クエリを追加する
     * @details l の昇順 -> r の昇順にソートしたときに、L[i] <= L[i+1] かつ R[i] <= R[i+1] が成立することが前提。
     * @param l 区間の左端 (0-indexed, 包含)
     * @param r 区間の右端 (0-indexed, 非包含)
     */
    void add_query(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= n);
        l_vec.emplace_back(l);
        r_vec.emplace_back(r);
    }

    /**
     * @brief クエリを一括で処理する
     * @param data クエリ対象の元データ配列
     * @param sorted すでにクエリが適切な順番にソートされている場合は true を指定する
     * @return std::vector<S> 各クエリに対する答えの配列
     */
    std::vector<S> solve(const std::vector<S>& data, bool sorted = false) {
        const i32 q = static_cast<i32>(l_vec.size());
        std::vector<i32> query(q);

        if (sorted) {
            std::iota(query.begin(), query.end(), 0);
        }
        else {
            std::vector<i32> pr = counting_sort(r_vec, n);
            std::vector<i32> ls(q);
            for (i32 i = 0; i < q; ++i) {
                ls[i] = l_vec[pr[i]];
            }
            std::vector<i32> pl = counting_sort(ls, n);
            for (i32 i = 0; i < q; ++i) {
                query[i] = pr[pl[i]];
            }
        }

        std::vector<S> ret(q), tail;
        S head = Monoid::e();
        i32 l = 0, r = 0;

        for (i32 idx : query) {
            i32 nl = l_vec[idx], nr = r_vec[idx];
            assert(l <= nl && r <= nr);

            for (; r < nr; ++r) {
                head = Monoid::op(head, data[r]);
            }
            for (; l < nl; ++l) {
                if (tail.empty()) {
                    // [l, r) の中身をすべて tail に移す
                    S acc = Monoid::e();
                    for (i32 i = r - 1; i >= l; --i) {
                        acc = Monoid::op(data[i], acc);
                        tail.emplace_back(acc);
                    }
                    head = Monoid::e();
                }
                tail.pop_back();
            }
            ret[idx] = Monoid::op((tail.empty() ? Monoid::e() : tail.back()), head);
        }
        return ret;
    }
};

/**
 * @brief Foldable Deque (SWAG Deque)
 * @details 両端キュー (Deque) として要素の追加・削除を行いながら、
 * 現在のキュー全体のモノイド演算の結果を $O(1)$ amortized で取得できるデータ構造です。
 *
 * @tparam Monoid モノイドを表す型 (`gwen::monoid` コンセプトを満たすこと)
 */
template <monoid Monoid> class SwagDeque {
public:
    using S = typename Monoid::S;

private:
    std::vector<S> f, b;
    std::deque<S> data;

    // dir = false: f >= b
    // dir = true : f <= b
    void balance(bool dir) {
        i32 n = static_cast<i32>(data.size());
        i32 m = dir ? n / 2 : (n + 1) / 2;
        f.resize(m);
        b.resize(n - m);

        S acc = Monoid::e();
        for (i32 i = m - 1; i >= 0; --i) {
            acc = Monoid::op(data[i], acc);
            f[m - 1 - i] = acc;
        }
        acc = Monoid::e();
        for (i32 i = m; i < n; ++i) {
            acc = Monoid::op(acc, data[i]);
            b[i - m] = acc;
        }
    }

public:
    /**
     * @brief デフォルトコンストラクタ
     */
    SwagDeque() = default;

    /**
     * @brief 現在のキュー内の全要素を畳み込んだ結果を返す
     * @return S 計算結果
     */
    S fold() const {
        if (data.empty()) return Monoid::e();
        if (f.empty()) return b.back();
        if (b.empty()) return f.back();
        return Monoid::op(f.back(), b.back());
    }

    /**
     * @brief キューの先頭に要素を追加する
     * @param x 追加する要素
     */
    void push_front(S x) {
        data.emplace_front(x);
        if (f.empty()) {
            f.emplace_back(x);
        }
        else {
            f.emplace_back(Monoid::op(x, f.back()));
        }
    }

    /**
     * @brief キューの末尾に要素を追加する
     * @param x 追加する要素
     */
    void push_back(S x) {
        data.emplace_back(x);
        if (b.empty()) {
            b.emplace_back(x);
        }
        else {
            b.emplace_back(Monoid::op(b.back(), x));
        }
    }

    /**
     * @brief キューの末尾の要素を削除する
     */
    void pop_back() {
        assert(!data.empty());
        if (b.empty()) balance(true);
        b.pop_back();
        data.pop_back();
    }

    /**
     * @brief キューの先頭の要素を削除する
     */
    void pop_front() {
        assert(!data.empty());
        if (f.empty()) balance(false);
        f.pop_back();
        data.pop_front();
    }
};

}  // namespace gwen
