#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1232"

#include <iostream>
#include <algorithm>

#include "gwen/types.hpp"
#include "gwen/math/prime.hpp"

using namespace std;
using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // m > 4, 0 < a/b <= 1
    // find (p, q) that satisfying pq <= m and a/b <= p/q <= 1
    // pq takes the maximum value
    // 4 < m < 1e5, 1 <= a <= b <= 1000
    // Q = 2000
    std::vector<i64> P;
    auto LPF = LpfTable(200000);
    for(i64 i = 2; i < 200000; ++i) if(LPF.is_prime(i)) P.push_back(i);

    i64 m, a, b;
    while(true) {
        cin >> m >> a >> b;
        if(m == 0 && a == 0 && b == 0) return 0;

        i64 ansp = 0, ansq = 0, prod = 0;
        for(i64 p : P) if(p * p <= m) {
            // p*q <= m
            // a*q <= b*p
            // q <= m/p
            // q <= b*p/a
            // p <= q
            i64 lim = min(m / p, b * p / a);
            auto ub = upper_bound(P.begin(), P.end(), lim);
            if(ub == P.begin()) continue;
            i64 q = *prev(ub);
            if(prod < p * q && p <= q) {
                ansp = p, ansq = q, prod = p * q;
            }
        }
        cout << ansp << ' ' << ansq << '\n';
    }

    return 0;
}
