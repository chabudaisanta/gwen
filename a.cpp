#ifdef LOCAL
#else
#define NDEBUG
#endif
// # pragma GCC target("avx2")
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#include "gwen/geo/graham_scan.hpp"
#include <iostream>
#include <vector>
#include <ctime>

#include "gwen/dump.hpp"
#include "gwen/io.hpp"
#include "gwen/types.hpp"
#define rp(i, n) for (i32 i = 0; i < (i32)(n); ++i)
constexpr char EL = '\n';
#define BAR std::cerr << "-------------------------\n"
#include <atcoder/modint>
#include "gwen/container/persistent_stack.hpp"
#include <map>

namespace gwen {
using Stack = persistent_stack<i32>;
void solve() {
    i32 Q; input >> Q;
    std::map<i32,Stack> MP;
    Stack A;
    while(Q--) {
        std::string s; input >> s;
        if(s == "ADD") {
            i32 x; input >> x;
            A.push(x);
        }
        else if(s == "DELETE") {
            if(A.size()) A.pop();
        }
        else if(s == "SAVE") {
            i32 y; input >> y;
            MP[y] = A;
        }
        else {
            i32 z; input >> z;
            A = MP[z];
        }
        output << (A.size() ? A.top() : -1) << EL;
    }
}

}

int main() {
    int t = 1;
    // std::cin >> t;
    while (t--) gwen::solve();
    std::cerr << "execution time: " << 1.0 * clock() / CLOCKS_PER_SEC * 1000
              << "(ms)\n";
    return 0;
}