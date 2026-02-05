#pragma once

#include <vector>
#include <cassert>
#include <ranges>
#include <utility>
#include "gwen/types.hpp"

// https://codeforces.com/blog/entry/135239
namespace gwen {

struct xor_tree_scan_info {
    std::vector<i32> par, ord;
};
template<typename Edge, bool NeedOrd>
xor_tree_scan_info xor_tree_scan(const std::vector<Edge>& E, i32 root) {
    const i32 N = E.size() + 1;
    assert(0 <= root && root < N);

    std::vector<i32> D(N), X(N);
    for(const auto& e : E) {
        D[e.u]++;
        D[e.v]++;
        X[e.u] ^= e.v;
        X[e.v] ^= e.u;
    }
    D[root] = 0;

    std::vector<i32> xor_order;
    if constexpr (NeedOrd) xor_order.reserve(N - 1);

    for(i32 i = 0; i < N; ++i) {
        i32 node = i;
        while(D[node] == 1) {
            if constexpr (NeedOrd) xor_order.emplace_back(node);
            i32 par = X[node];
            X[par] ^= node;
            D[par]--;
            D[node] = 0;
            node = par;
        }
    }
    
    X[root] = -1;
    return { X, xor_order };
}

std::vector<i32> get_dfs_order(const xor_tree_scan_info& info) {
    const auto& [par, ord] = info;
    assert((par.empty() && ord.empty()) || ord.size() == par.size() - 1);
    const i32 N = par.size();

    std::vector<i32> s(N, 1); // subtree_size
    for(i32 node : ord) s[par[node]] += s[node];

    // s[par]: parのdfs範囲の右端
    // parのdfs範囲から、nodeのdfs範囲を切り出して分割していく.
    for(i32 node : ord | std::views::reverse) {
        // s[node], s[par] = s[par], s[par] - s[node]
        i32 p = par[node];
        s[node] = std::exchange(s[p], s[p] - s[node]);
    }

    std::vector<i32> dfs_ord(N);
    for(i32 i = 0; i < N; ++i) dfs_ord[s[i] - 1] = i;
    return dfs_ord;
}

}