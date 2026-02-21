#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "gwen/misc/node_pool.hpp"
#include "gwen/types.hpp"
namespace gwen {

template <i32 Width> struct trie_node_minimum {
    i32 par = 0;         // NILで初期化.
    i32 ch[Width] = {};  // NILで初期化.
    i32 cnt = 0;
    i32 subtree_cnt = 0;
    i32 idx = -1;  // 負のインデックスで初期化.
    trie_node_minimum() = default;
    trie_node_minimum(i32 idx_) : idx(idx_) {}
};
template <typename Node, i32 Width, i32 Offset> struct trie_base {
    using tree = i32;
    static inline node_pool<Node> d;
    static inline constexpr tree NIL = 0;

    tree root;
    explicit trie_base() : root(d.new_node(Node())) {}

    // par, ch, cnt, subtree_cnt, idx を変更すると壊れるので注意.
    static Node& get_node(tree id) { return d[id]; }

    template <typename Container> inline tree find(const Container& str) const {
        tree cur = root;
        for (auto&& x : str) {
            if (cur != NIL) {
                i32 i = i32(x) - Offset;
                assert(0 <= i && i < Width);
                cur = d[cur].ch[i];
            }
        }
        return cur;
    }
    template <typename Container> inline i32 count(const Container& str) const {
        tree tgt = find(str);
        return tgt == NIL ? 0 : d[tgt].cnt;
    }
    template <typename Container> inline bool contains(const Container& str) const { return count(str); }
    template <typename Container> inline i32 match_count(const Container& str) const {
        tree tgt = find(str);
        return tgt == NIL ? 0 : d[tgt].subtree_cnt;
    }
    template <typename Container> Node& get_node(const Container& str) {
        tree tgt = find(str);
        assert(tgt != NIL);
        return get_node(tgt);
    }

    template <typename Container> tree insert(const Container& str) {
        tree cur = root;
        for (auto&& x : str) {
            d[cur].subtree_cnt++;

            i32 i = i32(x) - Offset;
            assert(0 <= i && i < Width);
            if (d[cur].ch[i] == NIL) {
                i32 child = d.new_node(Node(i));
                d[cur].ch[i] = child;
                d[child].par = cur;
            }
            cur = d[cur].ch[i];
        }
        d[cur].subtree_cnt++;
        d[cur].cnt++;
        return cur;
    }

    // n <= 0: erase all items at str
    // 0 < n : erase n items at str
    template <typename Container> void erase(const Container& str, i32 n = 1) {
        tree cur = find(str);
        if (cur == NIL || d[cur].cnt == 0) return;

        if (n <= 0) {
            n = d[cur].cnt;
        }
        else {
            n = std::min(n, d[cur].cnt);
        }
        d[cur].cnt -= n;
        while (cur != NIL) {
            d[cur].subtree_cnt -= n;
            cur = d[cur].par;
        }
    }

    // path(str)[0] = root, path(str)[i] = node after str[0..i). size() == min(len(str), match length) + 1.
    template <typename Container> std::vector<tree> path(const Container& str) const {
        std::vector<tree> ret;
        ret.emplace_back(root);
        tree cur = root;
        for (auto&& x : str) {
            i32 i = i32(x) - Offset;
            assert(0 <= i && i < Width);
            cur = d[cur].ch[i];
            ret.emplace_back(cur);
            if (cur == NIL) break;
        }
        return ret;
    }
};

}  // namespace gwen