#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <utility>
#include <vector>

#include "gwen/graph/graph.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 無向グラフの lowlink 値、関節点、橋、頂点二重連結成分を求める
 * @tparam EdgeType 辺の型
 * @details
 * 頂点二重連結成分は辺番号のリストとして返します。橋は単独の成分、自己ループも
 * 1 辺からなる成分として扱い、孤立頂点は成分に含めません。
 */
template <edge EdgeType> class Lowlink {
private:
    struct AdjacentEdge {
        i32 to;
        usize id;
    };

    struct Frame {
        i32 vertex;
        i32 parent;
        usize parent_edge;
        usize next_edge;
        i32 child_count;
    };

    std::vector<i32> ord_;
    std::vector<i32> low_;
    std::vector<i32> articulation_points_;
    std::vector<usize> bridges_;
    std::vector<std::vector<usize>> vertex_biconnected_components_;

public:
    /**
     * @brief 無向グラフを解析する
     * @param g 構築済みの無向グラフ
     */
    explicit Lowlink(const GraphBase<EdgeType, false>& g) : ord_(g.size(), -1), low_(g.size(), -1) {
        const i32 n = g.size();
        const auto& edges = g.all_edges();
        std::vector<std::vector<AdjacentEdge>> adjacency(n);
        for (usize id = 0; id < edges.size(); ++id) {
            const auto& e = edges[id];
            adjacency[e.u].push_back({e.v, id});
            adjacency[e.v].push_back({e.u, id});
        }

        constexpr usize no_edge = std::numeric_limits<usize>::max();
        std::vector<usize> edge_stack;
        std::vector<bool> is_articulation(n, false);
        std::vector<bool> self_loop_added(edges.size(), false);
        std::vector<Frame> dfs;
        i32 timer = 0;

        for (i32 root = 0; root < n; ++root) {
            if (ord_[root] != -1) continue;
            ord_[root] = low_[root] = timer++;
            dfs.push_back({root, -1, no_edge, 0, 0});

            while (!dfs.empty()) {
                Frame& frame = dfs.back();
                const i32 u = frame.vertex;
                if (frame.next_edge < adjacency[u].size()) {
                    const AdjacentEdge e = adjacency[u][frame.next_edge++];
                    if (e.to == u) {
                        if (!self_loop_added[e.id]) {
                            self_loop_added[e.id] = true;
                            vertex_biconnected_components_.push_back({e.id});
                        }
                        continue;
                    }
                    if (e.id == frame.parent_edge) continue;

                    if (ord_[e.to] == -1) {
                        ++frame.child_count;
                        edge_stack.push_back(e.id);
                        ord_[e.to] = low_[e.to] = timer++;
                        dfs.push_back({e.to, u, e.id, 0, 0});
                    }
                    else if (ord_[e.to] < ord_[u]) {
                        edge_stack.push_back(e.id);
                        low_[u] = std::min(low_[u], ord_[e.to]);
                    }
                    continue;
                }

                const Frame finished = frame;
                dfs.pop_back();
                if (finished.parent == -1) {
                    if (finished.child_count > 1) is_articulation[u] = true;
                    continue;
                }

                const i32 parent = finished.parent;
                low_[parent] = std::min(low_[parent], low_[u]);
                if (low_[u] > ord_[parent]) bridges_.push_back(finished.parent_edge);
                if (low_[u] >= ord_[parent]) {
                    if (!dfs.empty() && dfs.back().parent != -1) is_articulation[parent] = true;
                    std::vector<usize> component;
                    while (!edge_stack.empty()) {
                        const usize id = edge_stack.back();
                        edge_stack.pop_back();
                        component.push_back(id);
                        if (id == finished.parent_edge) break;
                    }
                    vertex_biconnected_components_.push_back(std::move(component));
                }
            }
        }

        for (i32 v = 0; v < n; ++v) {
            if (is_articulation[v]) articulation_points_.push_back(v);
        }
    }

    /**
     * @brief 各頂点の DFS 発見順を返す
     * @return 頂点ごとの発見順。未訪問値は存在しない
     */
    const std::vector<i32>& ord() const { return ord_; }

    /**
     * @brief 各頂点の lowlink 値を返す
     * @return 頂点ごとの lowlink 値
     */
    const std::vector<i32>& low() const { return low_; }

    /**
     * @brief 関節点を頂点番号の昇順で返す
     * @return 関節点の頂点番号
     */
    const std::vector<i32>& articulation_points() const { return articulation_points_; }

    /**
     * @brief 橋を返す
     * @return 橋の 0-based 入力辺番号
     */
    const std::vector<usize>& bridges() const { return bridges_; }

    /**
     * @brief 頂点二重連結成分を辺番号のリストとして返す
     * @return 各成分に属する辺の 0-based 入力辺番号
     */
    const std::vector<std::vector<usize>>& vertex_biconnected_components() const {
        return vertex_biconnected_components_;
    }
};

}  // namespace gwen
