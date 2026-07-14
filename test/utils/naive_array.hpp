#pragma once

#include <vector>
#include <algorithm>
#include <numeric>

namespace gwen::test {

/**
 * @brief セグメント木やBITなどのデータ構造のテスト用の愚直解クラス
 * 
 * 計算量は意図的に O(N) となっており、ランダムテスト時に
 * 正確な答え（Expected）を計算する目的で使用します。
 */
template <typename T>
struct NaiveArray {
    std::vector<T> data;

    NaiveArray() = default;
    explicit NaiveArray(int n, T default_value = T{}) : data(n, default_value) {}
    explicit NaiveArray(const std::vector<T>& v) : data(v) {}

    // 要素数
    int size() const { return data.size(); }

    // 1点取得・更新
    T get(int p) const { return data[p]; }
    void set(int p, T x) { data[p] = x; }
    void add(int p, T x) { data[p] += x; }

    // 区間更新系: [l, r)
    void range_add(int l, int r, T x) {
        for (int i = l; i < r; ++i) data[i] += x;
    }
    
    void range_update(int l, int r, T x) {
        for (int i = l; i < r; ++i) data[i] = x;
    }

    // 区間取得系: [l, r)
    T sum(int l, int r, T init = T{}) const {
        T res = init;
        for (int i = l; i < r; ++i) res += data[i];
        return res;
    }

    T min(int l, int r, T init) const {
        T res = init;
        for (int i = l; i < r; ++i) res = std::min(res, data[i]);
        return res;
    }

    T max(int l, int r, T init) const {
        T res = init;
        for (int i = l; i < r; ++i) res = std::max(res, data[i]);
        return res;
    }

    // 任意の二項演算による区間取得
    template <typename Op>
    T fold(int l, int r, Op op, T e) const {
        T res = e;
        for (int i = l; i < r; ++i) res = op(res, data[i]);
        return res;
    }
};

} // namespace gwen::test
