#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <queue>

#include "gwen/graph/doubling_tree.hpp"

using namespace std;
using namespace gwen;

int main() {
    int N; cin >> N;
    vector<vector<int>> G(N);
    for(int i = 0; i < N-1; ++i) {
        int u, v; cin >> u >> v;
        --u; --v;
        G[u].emplace_back(v);
        G[v].emplace_back(u);
    }

    queue<int> Q;
    int r1 = 0;
    Q.emplace(0);
    vector<int> vis(N, -1);
    while(Q.size()) {
        int cur = Q.front();
        Q.pop();

        if(vis[cur] == 0) continue;
        vis[cur] = 0;

        r1 = cur;
        for(int v : G[cur]) Q.emplace(v);
    }

    Q.emplace(r1);
    int r2 = 0;
    while(Q.size()) {
        int cur = Q.front();
        Q.pop();

        if(vis[cur] == 1) continue;
        vis[cur] = 1;

        r2 = cur;
        for(int v : G[cur]) Q.emplace(v);
    }

    doubling_tree X(N, r1, G), Y(N, r2, G);
    int T; cin >> T;
    while(T--) {
        int u, k; cin >> u >> k;
        --u;

        int ans = X.kth_anc(u, k);
        if(ans == -1) ans = Y.kth_anc(u, k);

        cout << (ans == -1 ? -1 : ans + 1) << '\n';
    }
    return 0;
}