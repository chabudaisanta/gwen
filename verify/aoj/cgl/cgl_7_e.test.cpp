#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_E"
#define ERROR   1e-6

#include <cassert>
#include <iomanip>
#include <iostream>

#include "gwen/geo/circle.hpp"

using namespace gwen;
using namespace gwen::geo;

int main() {
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    f80 x1, y1, r1, x2, y2, r2;
    std::cin >> x1 >> y1 >> r1 >> x2 >> y2 >> r2;
    const auto [count, points] = intersection(Circle<f80>({x1, y1}, r1), Circle<f80>({x2, y2}, r2));
    assert(count == 1 || count == 2);
    const auto& p = points[0];
    const auto& q = points[count == 1 ? 0 : 1];
    std::cout << std::fixed << std::setprecision(15) << p.x << ' ' << p.y << ' ' << q.x << ' ' << q.y << '\n';
}
