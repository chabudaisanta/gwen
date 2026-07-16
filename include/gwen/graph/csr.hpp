#pragma once

#include <span>
#include <vector>

#include "gwen/graph/edge.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief CSR (Compressed Sparse Row) 形式のグラフ表現
 * @tparam E エッジの型
 */
template <edge E> class Csr {
private:
    std::vector<i32> start_;
    std::vector<E> elist_;

public:
    Csr() = default;

    /**
     * @brief グラフ構造からCSR形式を構築する
     * @param n 頂点数
     * @param edges エッジのリスト
     * @param directed 有向グラフか無向グラフか
     */
    Csr(i32 n, const std::vector<E>& edges, bool directed = true) : start_(n + 1, 0) {
        if (directed) {
            for (const auto& e : edges) {
                start_[e.u + 1]++;
            }
            for (i32 i = 1; i <= n; ++i) {
                start_[i] += start_[i - 1];
            }
            elist_.resize(edges.size());
            auto counter = start_;
            for (const auto& e : edges) {
                elist_[counter[e.u]++] = e;
            }
        }
        else {
            for (const auto& e : edges) {
                start_[e.u + 1]++;
                start_[e.v + 1]++;
            }
            for (i32 i = 1; i <= n; ++i) {
                start_[i] += start_[i - 1];
            }
            elist_.resize(edges.size() * 2);
            auto counter = start_;
            for (const auto& e : edges) {
                elist_[counter[e.u]++] = e;
                E rev = e;
                rev.u = e.v;
                rev.v = e.u;
                elist_[counter[rev.u]++] = rev;
            }
        }
    }

    /**
     * @brief 頂点数を返す
     * @return i32 頂点数
     */
    i32 size() const { return static_cast<i32>(start_.size()) - 1; }

    /**
     * @brief 全エッジ数を返す (無向の場合は2倍)
     * @return i32 エッジ数
     */
    i32 num_edges() const { return static_cast<i32>(elist_.size()); }

    /**
     * @brief 頂点 v から出るエッジのリストを取得する
     * @param v 頂点
     * @return std::span<E> エッジのリストのビュー
     */
    std::span<E> edges(i32 v) { return std::span<E>(elist_).subspan(start_[v], start_[v + 1] - start_[v]); }

    /**
     * @brief 頂点 v から出るエッジのリストを取得する
     * @param v 頂点
     * @return std::span<const E> エッジのリストのビュー
     */
    std::span<const E> edges(i32 v) const {
        return std::span<const E>(elist_).subspan(start_[v], start_[v + 1] - start_[v]);
    }
};

}  // namespace gwen
