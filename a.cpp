#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

#include "gwen/algebra/rolling_hash_monoid.hpp"
#include "gwen/container/mergesort_tree.hpp"
#include "gwen/container/segment_tree.hpp"
#include "gwen/mod/rolling_hash.hpp"

using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];
    gwen::mergesort_tree<int> T(A);
    int Q;
    cin >> Q;
    while (Q--) {
        int l, r, x;
        cin >> l >> r >> x;
        cout << T.lower_cnt(l - 1, r, x) << '\n';
    }

    return 0;
}