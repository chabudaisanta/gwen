#pragma once

// https://github.com/yosupo06/yosupo-library/blob/main/src/yosupo/container/splaytree.hpp

#include <utility>
#include <vector>

#include "gwen/types.hpp"
#include "gwen/algebra/monoid.hpp"
namespace gwen {
namespace internal {

template<acted_monoid AM>
class splay_tree_impl {
public:
    using S = AM::monoid::S;
    using F = AM::act::S;

private:
    using tree = i32;
    struct node {
        tree par;
        tree lch;
        tree rch;
        S val;
        S prod;
        F lz;
        i32 cnt;
    };

    std::vector<node> d;
};
} // namespace internal


}  // namespace gwen