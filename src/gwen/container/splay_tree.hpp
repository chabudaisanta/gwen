#pragma once

// 参考
// https://tjkendev.github.io/procon-library/cpp/binary_search_tree/splay-tree.html
// https://github.com/yosupo06/yosupo-library/blob/main/src/yosupo/container/splaytree.hpp

#include <vector>

#include "gwen/algebra/monoid.hpp"
namespace gwen {

template <acted_monoid M>
class splay_tree {
public:
    using S = typename M::S;
    using F = typename M::F;

    struct node {
        int l, r;
        int len;
        S s, prod;
        F f;
    };

    std::vector<node> nodes;
    M m;

public:
    splay_tree(const M& m_) : m(m_) {}

private:

};

}  // namespace gwen