#pragma once

#include <algorithm>
#include <concepts>
#include <vector>

#include "gwen/graph/graph.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 木の直径と中心を表す結果
 * @tparam Weight 辺の重みの型
 */
template <typename Weight> struct TreeDiameterResult {
    /** @brief 直径の重み付き長さ */
    Weight distance{};
    /** @brief 直径の一方の端点 */
    i32 endpoint_u = -1;
    /** @brief 直径のもう一方の端点 */
    i32 endpoint_v = -1;
    /** @brief endpoint_u から endpoint_v への直径パス */
    std::vector<i32> path;
    /**
     * @brief 選択した直径パス上にある中心の表現
     * @details 中心が頂点上ならその頂点1つ、正重み辺の内部ならその辺の両端を格納します。
     *          0重み辺上で中心と同距離になる複数頂点がある場合は代表となる頂点を1つ格納します。
     */
    std::vector<i32> centers;
};

/**
 * @brief 非負整数重み付き木の直径、直径パス、および中心を求める
 * @tparam Edge 辺の型
 * @param G 非負整数重みの無向木
 * @return 直径の長さ、端点、パス、および中心を表す結果
 * @details 木を2回走査して直径を求めます。同率の端点が複数ある場合、返す端点は任意です。
 * @note G は頂点数1以上の連結な木であり、全辺重みは非負、パス上の重み和は重み型に収まる必要があります。
 * @note centers は全ての中心頂点を列挙せず、選択した直径パス上の中心を表します。
 * @complexity O(N) 時間、O(N) 追加領域
 */
template <edge Edge>
    requires(std::integral<typename Edge::weight_type> && !std::same_as<typename Edge::weight_type, bool>)
auto tree_diameter(const GraphBase<Edge, false>& G) {
    using Weight = typename Edge::weight_type;

    struct WalkResult {
        std::vector<Weight> distance;
        std::vector<i32> parent;
        i32 farthest;
    };

    const usize n = static_cast<usize>(G.size());
    auto walk = [&](i32 start) {
        WalkResult result{std::vector<Weight>(n), std::vector<i32>(n, -1), start};
        std::vector<i32> stack{start};
        result.parent[static_cast<usize>(start)] = start;

        while (!stack.empty()) {
            const i32 v = stack.back();
            stack.pop_back();
            const usize v_index = static_cast<usize>(v);
            if (result.distance[v_index] > result.distance[static_cast<usize>(result.farthest)]) result.farthest = v;

            for (const Edge& e : G.edges(v)) {
                const usize next_index = static_cast<usize>(e.v);
                if (e.v == result.parent[v_index]) continue;
                result.parent[next_index] = v;
                result.distance[next_index] = result.distance[v_index] + e.w;
                stack.push_back(e.v);
            }
        }
        return result;
    };

    const WalkResult first = walk(0);
    const i32 endpoint_u = first.farthest;
    WalkResult second = walk(endpoint_u);
    const i32 endpoint_v = second.farthest;

    TreeDiameterResult<Weight> result;
    result.distance = second.distance[static_cast<usize>(endpoint_v)];
    result.endpoint_u = endpoint_u;
    result.endpoint_v = endpoint_v;
    for (i32 v = endpoint_v;; v = second.parent[static_cast<usize>(v)]) {
        result.path.push_back(v);
        if (v == endpoint_u) break;
    }
    std::ranges::reverse(result.path);

    const Weight half = result.distance / 2;
    const bool odd = result.distance % 2 != 0;
    for (usize i = 0; i < result.path.size(); ++i) {
        const i32 v = result.path[i];
        const Weight prefix = second.distance[static_cast<usize>(v)];
        if (!odd && prefix == half) {
            result.centers.push_back(v);
            break;
        }

        if (i + 1 < result.path.size()) {
            const i32 next = result.path[i + 1];
            const Weight next_prefix = second.distance[static_cast<usize>(next)];
            const bool crosses_center = odd ? prefix <= half && next_prefix > half : prefix < half && next_prefix > half;
            if (crosses_center) {
                result.centers = {v, next};
                break;
            }
        }
    }

    return result;
}

}  // namespace gwen
