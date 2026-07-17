#define PROBLEM "https://judge.yosupo.jp/problem/ordered_set"

#include <iostream>

#include "gwen/bbst/sorted_treap.hpp"

using namespace std;
using namespace gwen;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    i32 N, Q; cin >> N >> Q;

    SortedTreap<i32> T;
    for(i32 i = 0; i < N; ++i) {
        i32 a; cin >> a;
        T.insert(a);
    }
    while(Q--) {
        i32 t, x; cin >> t >> x;
        if(t == 0) {
            if(!T.contains(x)) T.insert(x);
        }
        else if(t == 1) {
            if(T.contains(x)) T.erase(x);
        }
        else if(t == 2) {
            if(x - 1 < T.size()) cout << T.kth(x - 1) << '\n';
            else cout << "-1\n";
        }
        else if(t == 3) {
            cout << T.count_lower(x+1) << '\n';
        }
        else if(t == 4) {
            auto ub = T.upper_bound(x);
            if(ub != T.begin()) cout << *(prev(ub)) << '\n';
            else cout << "-1\n";
        }
        else {
            auto lb = T.lower_bound(x);
            if(lb != T.end()) cout << *lb << '\n';
            else cout << "-1\n";
        }
    }
}
