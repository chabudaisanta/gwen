#pragma once

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

#include "gwen/utils/node_pool.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief Trie木のノード
 * @tparam Width 文字の種類数
 */
template <i32 Width>
struct TrieNodeMinimum {
    i32 par = 0;         // NILで初期化
    i32 ch[Width] = {};  // NILで初期化
    i32 cnt = 0;         // このノードで終わる文字列の数
    i32 subtree_cnt = 0; // このノードを部分木に持つ文字列の数 (prefix count)
    i32 idx = -1;        // 文字のインデックス
    
    TrieNodeMinimum() = default;
    explicit TrieNodeMinimum(i32 idx_) : idx(idx_) {}
};

/**
 * @brief Trie木 (プレフィックス木) のベースクラス
 *
 * @tparam Node ノードの型
 * @tparam Width 文字の種類数
 * @tparam Offset 文字のオフセット (例: 'a')
 */
template <typename Node, i32 Width, i32 Offset>
class TrieBase {
public:
    /** @brief Trie木のノードID型 */
    using tree = i32;
    /** @brief ノードの実態を管理するメモリプール */
    static inline NodePool<Node> d;
    /** @brief 存在しないノードを示す番兵 */
    static inline constexpr tree NIL = 0;

private:
    tree root;

public:
    /**
     * @brief デフォルトコンストラクタ
     */
    explicit TrieBase() : root(d.new_node(Node())) {}

    /**
     * @brief IDからノードの参照を取得する
     * @param id ノードのID
     */
    static Node& get_node(tree id) { return d[id]; }

    /**
     * @brief 文字列に完全一致するノードIDを検索する
     * @param str 検索する文字列 (std::string や std::vector など)
     * @return tree ノードのID。見つからない場合は NIL
     */
    template <std::ranges::range Container>
    inline tree find(const Container& str) const {
        tree cur = root;
        for (auto&& x : str) {
            if (cur != NIL) {
                i32 i = static_cast<i32>(x) - Offset;
                assert(0 <= i && i < Width);
                cur = d[cur].ch[i];
            }
        }
        return cur;
    }

    /**
     * @brief 文字列がTrie木にいくつ登録されているかを返す
     * @param str 検索する文字列
     */
    template <std::ranges::range Container>
    inline i32 count(const Container& str) const {
        tree tgt = find(str);
        return tgt == NIL ? 0 : d[tgt].cnt;
    }

    /**
     * @brief 文字列がTrie木に含まれているかを判定する
     * @param str 検索する文字列
     */
    template <std::ranges::range Container>
    inline bool contains(const Container& str) const {
        return count(str) > 0;
    }

    /**
     * @brief 文字列をプレフィックスとして持つ単語の数を返す
     * @param str プレフィックス
     */
    template <std::ranges::range Container>
    inline i32 match_count(const Container& str) const {
        tree tgt = find(str);
        return tgt == NIL ? 0 : d[tgt].subtree_cnt;
    }

    /**
     * @brief 文字列に対応するノードの参照を取得する
     * @param str 検索する文字列 (存在することが保証されていること)
     */
    template <std::ranges::range Container>
    Node& get_node(const Container& str) {
        tree tgt = find(str);
        assert(tgt != NIL);
        return get_node(tgt);
    }

    /**
     * @brief 文字列をTrie木に追加する
     * @param str 追加する文字列
     * @return tree 終端ノードのID
     */
    template <std::ranges::range Container>
    tree insert(const Container& str) {
        tree cur = root;
        for (auto&& x : str) {
            d[cur].subtree_cnt++;

            i32 i = static_cast<i32>(x) - Offset;
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

    /**
     * @brief 文字列をTrie木から削除する
     * @param str 削除する文字列
     * @param n 削除する個数 (n <= 0 の場合はすべて削除)
     */
    template <std::ranges::range Container>
    void erase(const Container& str, i32 n = 1) {
        tree cur = find(str);
        if (cur == NIL || d[cur].cnt == 0) return;

        if (n <= 0) {
            n = d[cur].cnt;
        } else {
            n = std::min(n, d[cur].cnt);
        }
        
        d[cur].cnt -= n;
        while (cur != NIL) {
            d[cur].subtree_cnt -= n;
            cur = d[cur].par;
        }
    }

    /**
     * @brief 文字列を探索した際の経路(ノードIDの列)を返す
     * @details path(str)[0] は root、path(str)[i] は str[0..i) の後のノード。
     * @param str 探索する文字列
     */
    template <std::ranges::range Container>
    std::vector<tree> path(const Container& str) const {
        std::vector<tree> ret;
        ret.emplace_back(root);
        tree cur = root;
        for (auto&& x : str) {
            i32 i = static_cast<i32>(x) - Offset;
            assert(0 <= i && i < Width);
            cur = d[cur].ch[i];
            ret.emplace_back(cur);
            if (cur == NIL) break;
        }
        return ret;
    }
};

/** @brief 英小文字 ('a'-'z') 用の Trie木 */
using LowerAlphaTrie = TrieBase<TrieNodeMinimum<26>, 26, 'a'>;

/** @brief 英大文字 ('A'-'Z') 用の Trie木 */
using UpperAlphaTrie = TrieBase<TrieNodeMinimum<26>, 26, 'A'>;

/** @brief アルファベット全般 (記号等も含む可能性があるため Offset='A', Width=52) 用の Trie木 */
using AlphaTrie = TrieBase<TrieNodeMinimum<52>, 52, 'A'>;

}  // namespace gwen
