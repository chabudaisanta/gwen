#pragma once
#include <vector>
#include <cassert>
#include "gwen/types.hpp"
namespace gwen {

// 値域が [0, M] である整数列 A に対して、以下の条件を満たす順列の並び替え P を返す
// 0 <= l < r < N を満たす (l, r) の組に対して、
// P[l] < P[r] であるときには A[P[l]] <= A[P[r]] である
// P[l] > P[r] であるときには A[P[l]] > A[P[r]] である
// O(N + M)
template<typename T>
std::vector<i32> counting_sort(const std::vector<T>& A, i32 M) {
    std::vector<i32> S(M+2);
    i32 N = A.size();
    for(i32 i = 0; i < N; ++i) {
        assert(0 <= A[i] && A[i] <= M);
        S[A[i]+1]++;
    }
    for(i32 j = 0; j <= M; ++j) S[j+1] += S[j];
    std::vector<i32> ret(N);
    for(i32 i = 0; i < N; ++i) ret[S[A[i]]++] = i;
    return ret;
}
}