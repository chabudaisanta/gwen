#define PROBLEM "https://judge.yosupo.jp/problem/static_convex_hull"

#include <iostream>
#include <vector>
#include "gwen/geo/point.hpp"
#include "gwen/geo/convex_polygon.hpp"

using namespace std;
using namespace gwen::geo;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Point<long long>> pts(n);
    for (int i = 0; i < n; ++i) {
        cin >> pts[i].x >> pts[i].y;
    }

    auto hull = convex_polygon(pts);

    cout << hull.size() << "\n";
    for (const auto& p : hull) {
        cout << p.x << " " << p.y << "\n";
    }

    return 0;
}
