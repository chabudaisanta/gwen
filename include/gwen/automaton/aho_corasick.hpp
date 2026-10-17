#pragma once

#include <array>
#include <cassert>
#include <limits>
#include <span>
#include <vector>

#include "gwen/automaton/automaton.hpp"
#include "gwen/types.hpp"

namespace gwen::automaton {

/**
 * @brief 複数パターンの末尾一致を判定する Aho-Corasick オートマトンを構築する。
 * @tparam base アルファベットのサイズ
 * @param patterns 非空のパターン列。各要素は `[0, base)` に属する。
 * @return 各状態の受理性が、現在読んだ列の末尾での一致を表す完全な Automaton
 * @details 受理状態を通過した後も、次の文字で非受理状態に遷移しうる。
 */
template <i32 base> Automaton<base> aho_corasick(std::span<const std::vector<i32>> patterns) {
    static_assert(base >= 1);
    std::vector<std::array<i32, base>> trie(1);
    trie.back().fill(-1);
    std::vector<bool> matched(1, false);

    for (const auto& pattern : patterns) {
        assert(!pattern.empty());
        i32 state = 0;
        for (i32 c : pattern) {
            assert(0 <= c && c < base);
            i32 next = trie[state][c];
            if (next == -1) {
                assert(trie.size() < static_cast<usize>(std::numeric_limits<i32>::max()));
                next = static_cast<i32>(trie.size());
                trie.emplace_back();
                trie.back().fill(-1);
                trie[state][c] = next;
                matched.push_back(false);
            }
            state = next;
        }
        matched[state] = true;
    }

    Automaton<base> a(static_cast<i32>(trie.size()));
    a.init = {0};
    for (i32 state = 0; state < a.n; ++state) {
        for (i32 c = 0; c < base; ++c) a.set_edge(state, c, trie[state][c]);
    }

    std::vector<i32> failure(a.n, 0);
    std::vector<i32> queue;
    for (i32 c = 0; c < base; ++c) {
        const i32 next = a.edge(0, c);
        if (next == -1)
            a.set_edge(0, c, 0);
        else
            queue.push_back(next);
    }
    for (usize head = 0; head < queue.size(); ++head) {
        const i32 state = queue[head];
        matched[state] = matched[state] || matched[failure[state]];
        if (matched[state]) a.accept.push_back(state);
        for (i32 c = 0; c < base; ++c) {
            const i32 next = a.edge(state, c);
            if (next == -1) {
                a.set_edge(state, c, a.edge(failure[state], c));
            }
            else {
                failure[next] = a.edge(failure[state], c);
                queue.push_back(next);
            }
        }
    }
    return a;
}

}  // namespace gwen::automaton
