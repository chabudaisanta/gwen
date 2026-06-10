#pragma once

#include <cassert>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

template <typename Node> struct node_pool {
    std::vector<Node> pool;
    explicit node_pool() { pool.emplace_back(Node()); }
    i32 new_node(Node&& node) {
        pool.emplace_back(std::move(node));
        return pool.size() - 1;
    }
    Node& operator[](i32 id) { return pool[id]; }
    const Node& operator[](i32 id) const { return pool[id]; }
};
}  // namespace gwen