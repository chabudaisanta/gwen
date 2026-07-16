#pragma once

#include <cassert>
#include <concepts>
#include <ranges>
#include <span>
#include <vector>

#include "gwen/graph/csr.hpp"
#include "gwen/graph/edge.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief グラフの要件を定義するコンセプト
 */
template <typename T>
concept graph = requires(const T& t, i32 u) {
    typename T::edge_type;
    typename T::weight_type;
    { t.size() } -> std::convertible_to<i32>;
    { t.edges(u) } -> std::ranges::range;
    requires std::convertible_to<std::ranges::range_reference_t<decltype(t.edges(u))>, const typename T::edge_type&>;
    { t.adjacent(u) } -> std::ranges::range;
    requires std::convertible_to<std::ranges::range_reference_t<decltype(t.adjacent(u))>, i32>;
};

/**
 * @brief グラフの基本実装クラス
 * @tparam EdgeType エッジの型
 * @tparam IsDirected 有向グラフか無向グラフか
 */
template <edge EdgeType, bool IsDirected = true> class GraphBase {
public:
    using edge_type = EdgeType;
    using weight_type = typename EdgeType::weight_type;

private:
    i32 n_;
    bool built_;
    std::vector<EdgeType> edge_buf_;
    Csr<EdgeType> csr_;

public:
    GraphBase() : GraphBase(0) {}
    explicit GraphBase(i32 n) : n_(n), built_(false) {}
    GraphBase(i32 n, const std::vector<EdgeType>& edges)
        : n_(n), built_(true), edge_buf_(edges), csr_(n, edges, IsDirected) {}

    /**
     * @brief グラフにエッジを追加する
     * @param e 追加するエッジ
     */
    void add_edge(const EdgeType& e) {
        assert(!built_);
        edge_buf_.push_back(e);
    }
    void add_edge(i32 u, i32 v) {
        assert(0 <= u && u < n_);
        assert(0 <= v && u < n_);
        add_edge(EdgeType(u, v));
    }
    void add_edge(i32 u, i32 v, weight_type w) {
        assert(0 <= u && u < n_);
        assert(0 <= v && v < n_);
        add_edge(EdgeType(u, v, w));
    }

    /**
     * @brief 内部データ構造(CSR)を構築する。クエリ応答の前に一度だけ呼ぶ。
     */
    void build() {
        assert(!built_);
        csr_ = Csr<EdgeType>(n_, edge_buf_, IsDirected);
        built_ = true;
    }

    /**
     * @brief 頂点数を返す
     * @return i32 頂点数
     */
    i32 size() const { return n_; }

    /**
     * @brief 全エッジを取得する
     * @return const std::vector<EdgeType>& エッジのリスト
     */
    const std::vector<EdgeType>& all_edges() const { return edge_buf_; }

    /**
     * @brief 頂点 u から出るエッジのビューを取得する
     * @param u 頂点
     * @return 頂点 u から出るエッジのビュー
     */
    std::span<const EdgeType> edges(i32 u) const {
        assert(built_);
        return csr_.edges(u);
    }

    /**
     * @brief 頂点 u に隣接する頂点のビューを取得する
     * @param u 頂点
     * @return 隣接する頂点 (v) のビュー
     */
    auto adjacent(i32 u) const {
        assert(built_);
        return csr_.edges(u) | std::views::transform([](const EdgeType& e) -> i32 { return e.v; });
    }

    /**
     * @brief 頂点 u に隣接する頂点のビューを取得する (adjacent のエイリアス)
     * @param u 頂点
     * @return 隣接する頂点 (v) のビュー
     */
    auto operator[](i32 u) const { return adjacent(u); }
};

template <typename T> using DirectedGraph = GraphBase<Edge<T>, true>;
template <typename T> using NonDirectedGraph = GraphBase<Edge<T>, false>;

}  // namespace gwen
