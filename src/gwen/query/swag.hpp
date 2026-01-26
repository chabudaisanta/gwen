#pragma once

#include <vector>
#include <cassert>
#include <numeric>
#include "gwen/types.hpp"
#include "gwen/misc/counting_sort.hpp"
#include "gwen/dump.hpp"
namespace gwen {

template<typename Monoid>
struct sliding_window_aggregation {
    using S = typename Monoid::S;
    i32 N;
    std::vector<S> data;
    std::vector<i32> L, R;

    sliding_window_aggregation() : N(0) {}
    explicit sliding_window_aggregation(const std::vector<S>& vec) : N(vec.size()), data(vec) {}

    // l の昇順 -> r の昇順にソートしたときに、L[i] <= L[i+1] かつ R[i] <= R[i+1] が成立していなければならない
    void add_query(i32 l, i32 r) {
        assert(0 <= l && l <= r && r <= N);
        L.emplace_back(l);
        R.emplace_back(r);
    }

    std::vector<S> solve(bool sorted = false) {
        const i32 Q = L.size();
        std::vector<i32> query(Q);
        if(sorted) {
            std::iota(query.begin(), query.end(), 0);
        }
        else {
            std::vector<i32> PR = counting_sort(R, N);
            std::vector<i32> Ls(Q);
            for(i32 i = 0; i < Q; ++i) Ls[i] = L[PR[i]];
            std::vector<i32> PL = counting_sort(Ls, N);
            for(i32 i = 0; i < Q; ++i) query[i] = PR[PL[i]];
        }
        for(i32 idx : query) DUMP(L[idx], R[idx]);

        std::vector<S> ret(Q), tail;
        S head = Monoid::e();
        i32 l = 0, r = 0;
        for(i32 idx : query) {
            i32 nl = L[idx], nr = R[idx];
            assert(l <= nl && r <= nr);
            for(; r < nr; ++r) {
                head = Monoid::op(head, data[r]);
            }
            for(; l < nl; ++l) {
                if(tail.empty()) {
                    // [l, r) の中身をすべて tail に移す
                    S acc = Monoid::e();
                    for(i32 i = r - 1; i >= l; --i) {
                        acc = Monoid::op(data[i], acc);
                        tail.emplace_back(acc);
                    }
                    head = Monoid::e();
                }
                tail.pop_back();
            }
            ret[idx] = Monoid::op((tail.empty() ? Monoid::e() : tail.back()), head);
        }
        return ret;
    }
};

}