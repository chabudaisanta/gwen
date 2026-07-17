#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"

#include <iostream>
#include <string>
#include "gwen/ds/fast_set.hpp"
#include "gwen/types.hpp"

using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    i32 n, q;
    if (!(std::cin >> n >> q)) return 0;

    std::string s;
    std::cin >> s;

    FastSet st(n);
    for (i32 i = 0; i < n; ++i) {
        if (s[i] == '1') {
            st.insert(i);
        }
    }

    while (q--) {
        i32 c, k;
        std::cin >> c >> k;
        if (c == 0) {
            st.insert(k);
        } else if (c == 1) {
            st.erase(k);
        } else if (c == 2) {
            std::cout << (st.contains(k) ? 1 : 0) << "\n";
        } else if (c == 3) {
            i32 res = st.next(k);
            if (res == n) res = -1;
            std::cout << res << "\n";
        } else if (c == 4) {
            i32 res = st.prev(k);
            std::cout << res << "\n";
        }
    }

    return 0;
}
