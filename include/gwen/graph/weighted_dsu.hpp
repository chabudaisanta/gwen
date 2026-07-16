#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "gwen/alge/abel.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 重み付き Union-Find
 * @tparam Abel 加群 (gwen::abel コンセプトを満たす型)
 * @details 各連結成分における要素間の相対的な重みの差分を管理します。
 */
template <abel Abel>
class WeightedDsu {
public:
    using S = typename Abel::S;

private:
    i32 n_;
    std::vector<i32> parent_or_size_;
    std::vector<S> pot_;

public:
    /**
     * @brief デフォルトコンストラクタ
     */
    WeightedDsu() : n_(0) {}

    /**
     * @brief 頂点数 n の重み付き Union-Find を構築する
     * @param n 頂点数
     */
    explicit WeightedDsu(i32 n) : n_(n), parent_or_size_(n, -1), pot_(n, Abel::e()) {}

    /**
     * @brief 頂点 a の属する連結成分の代表元を返す
     * @param a 頂点
     * @return 代表元のインデックス
     */
    i32 leader(i32 a) {
        assert(0 <= a && a < n_);
        if (parent_or_size_[a] < 0) return a;
        i32 r = leader(parent_or_size_[a]);
        pot_[a] = Abel::op(pot_[a], pot_[parent_or_size_[a]]);
        return parent_or_size_[a] = r;
    }

    /**
     * @brief 頂点 a と b の重みの差分 P(a) - P(b) を返す
     * @param a 頂点
     * @param b 頂点
     * @return 重みの差分
     */
    S diff(i32 a, i32 b) {
        assert(0 <= a && a < n_);
        assert(0 <= b && b < n_);
        return Abel::op(potential(a), Abel::inv(potential(b)));
    }

    /**
     * @brief 頂点 a と b が同じ連結成分に属するか判定する
     * @param a 頂点
     * @param b 頂点
     * @return 同じ連結成分なら true
     */
    bool same(i32 a, i32 b) {
        assert(0 <= a && a < n_);
        assert(0 <= b && b < n_);
        return leader(a) == leader(b);
    }

    /**
     * @brief 頂点 a の代表元との重みの差分 P(a) - P(leader(a)) を返す
     * @param a 頂点
     * @return 代表元との重み差分
     */
    S potential(i32 a) {
        leader(a);
        return pot_[a];
    }

    /**
     * @brief P(a) - P(b) = w となるように頂点 a と b をマージする
     * @param a 頂点
     * @param b 頂点
     * @param w 重みの差分
     * @return 成功時は新しい代表元のインデックス。既に同じ成分にあり矛盾する場合は -1。
     */
    i32 merge(i32 a, i32 b, S w) {
        assert(0 <= a && a < n_);
        assert(0 <= b && b < n_);
        w = Abel::op(w, Abel::op(potential(b), Abel::inv(potential(a))));
        a = leader(a);
        b = leader(b);
        if (a == b) {
            return (w == Abel::e()) ? a : -1;
        }
        if (-parent_or_size_[a] < -parent_or_size_[b]) {
            std::swap(a, b);
            w = Abel::inv(w);
        }
        parent_or_size_[a] += parent_or_size_[b];
        parent_or_size_[b] = static_cast<i32>(a);
        pot_[b] = Abel::inv(w); // P(b) - P(a) = -w = inv(w)
        return a;
    }

    /**
     * @brief 頂点 a が属する連結成分のサイズを返す
     * @param a 頂点
     * @return 連結成分のサイズ
     */
    i32 size(i32 a) {
        assert(0 <= a && a < n_);
        return -parent_or_size_[leader(a)];
    }

    /**
     * @brief 全連結成分の頂点リストを返す
     * @return 連結成分ごとの頂点リスト
     */
    std::vector<std::vector<i32>> groups() {
        std::vector<i32> leader_buf(n_);
        std::vector<i32> group_size(n_, 0);
        for (i32 i = 0; i < n_; ++i) {
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]]++;
        }
        std::vector<std::vector<i32>> result(n_);
        for (i32 i = 0; i < n_; ++i) {
            result[i].reserve(group_size[i]);
        }
        for (i32 i = 0; i < n_; ++i) {
            result[leader_buf[i]].push_back(i);
        }
        result.erase(std::remove_if(result.begin(), result.end(),
                                    [](const std::vector<i32>& v) { return v.empty(); }),
                     result.end());
        return result;
    }
};

} // namespace gwen
