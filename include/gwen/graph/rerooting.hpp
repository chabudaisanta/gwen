#pragma once

#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/graph/graph.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 無向木の各頂点を根とした DP の結果を求める。
 * @details 各頂点の隣接辺を `g.edges(v)` の順に結合する。`lift` に渡す辺は、値を受け取る頂点から送信元へ向く。
 *          コールバックが定数時間なら計算量は O(N) 時間・空間。
 * @tparam E 辺の型
 * @tparam T DP の値型
 * @tparam Merge 隣接寄与を結合する関数の型
 * @tparam AddVertex 頂点を加える関数の型
 * @tparam Lift 辺を越えた寄与に変換する関数の型
 * @param g 構築済みの無向木
 * @param identity `merge` の単位元
 * @param merge 結合律を満たす関数 `T(T, T)`
 * @param add_vertex 隣接寄与の集約に頂点を加える関数 `T(T, i32)`
 * @param lift 隣接頂点の完成値を辺越しの寄与に変換する関数 `T(T, const E&)`
 * @return 各頂点を根とした DP の値。空グラフなら空配列を返す。
 * @pre `g` は構築済みで、空でない場合は連結かつ閉路のない無向グラフである。
 * @pre `identity` は `merge` の両側単位元である。
 */
template <edge E, std::copyable T, typename Merge, typename AddVertex, typename Lift>
    requires std::invocable<Merge&, const T&, const T&> &&
             std::convertible_to<std::invoke_result_t<Merge&, const T&, const T&>, T> &&
             std::invocable<AddVertex&, const T&, i32> &&
             std::convertible_to<std::invoke_result_t<AddVertex&, const T&, i32>, T> &&
             std::invocable<Lift&, const T&, const E&> &&
             std::convertible_to<std::invoke_result_t<Lift&, const T&, const E&>, T>
std::vector<T> rerooting(const GraphBase<E, false>& g, T identity, Merge merge, AddVertex add_vertex, Lift lift) {
    const i32 n = g.size();
    if (n == 0) return {};

    std::vector<i32> parent(n, -1);
    std::vector<i32> order{0};
    order.reserve(n);
    for (usize i = 0; i < order.size(); ++i) {
        const i32 v = order[i];
        for (const E& e : g.edges(v)) {
            if (e.v == parent[v]) continue;
            parent[e.v] = v;
            order.push_back(e.v);
        }
    }

    std::vector<T> down(n, identity);
    for (usize i = order.size(); i > 0; --i) {
        const i32 v = order[i - 1];
        T merged = identity;
        for (const E& e : g.edges(v)) {
            if (e.v != parent[v]) merged = std::invoke(merge, merged, std::invoke(lift, down[e.v], e));
        }
        down[v] = std::invoke(add_vertex, merged, v);
    }

    std::vector<T> from_parent(n, identity);
    std::vector<T> answer(n, identity);
    std::vector<T> contribution;
    std::vector<T> prefix;
    for (i32 v : order) {
        const auto edges = g.edges(v);
        const usize degree = edges.size();
        contribution.assign(degree, identity);
        prefix.assign(degree + 1, identity);
        for (usize i = 0; i < degree; ++i) {
            const E& e = edges[i];
            const T& source = e.v == parent[v] ? from_parent[v] : down[e.v];
            contribution[i] = std::invoke(lift, source, e);
            prefix[i + 1] = std::invoke(merge, prefix[i], contribution[i]);
        }
        answer[v] = std::invoke(add_vertex, prefix[degree], v);

        T suffix = identity;
        for (usize i = degree; i > 0; --i) {
            const E& e = edges[i - 1];
            if (e.v != parent[v]) {
                from_parent[e.v] = std::invoke(add_vertex, std::invoke(merge, prefix[i - 1], suffix), v);
            }
            suffix = std::invoke(merge, contribution[i - 1], suffix);
        }
    }
    return answer;
}

/**
 * @brief モノイドを指定して無向木の各頂点を根とした DP の結果を求める。
 * @details `M::e()` を単位元、`M::op` を隣接寄与の結合に使用する。隣接辺の順序と `lift`
 * の辺方向は上のオーバーロードと同じ。
 * @tparam M 隣接寄与を結合するモノイド
 * @tparam E 辺の型
 * @tparam AddVertex 頂点を加える関数の型
 * @tparam Lift 辺を越えた寄与に変換する関数の型
 * @param g 構築済みの無向木
 * @param add_vertex 隣接寄与の集約に頂点を加える関数 `M::S(const M::S&, i32)`
 * @param lift 隣接頂点の完成値を辺越しの寄与に変換する関数 `M::S(const M::S&, const E&)`
 * @return 各頂点を根とした DP の値。空グラフなら空配列を返す。
 * @pre `g` は構築済みで、空でない場合は連結かつ閉路のない無向グラフである。
 */
template <monoid M, edge E, typename AddVertex, typename Lift>
    requires std::copyable<typename M::S> && std::invocable<AddVertex&, const typename M::S&, i32> &&
             std::convertible_to<std::invoke_result_t<AddVertex&, const typename M::S&, i32>, typename M::S> &&
             std::invocable<Lift&, const typename M::S&, const E&> &&
             std::convertible_to<std::invoke_result_t<Lift&, const typename M::S&, const E&>, typename M::S>
std::vector<typename M::S> rerooting(const GraphBase<E, false>& g, AddVertex add_vertex, Lift lift) {
    using S = typename M::S;
    return rerooting(
        g, static_cast<S>(M::e()), [](S a, S b) { return M::op(std::move(a), std::move(b)); }, add_vertex, lift);
}

}  // namespace gwen
