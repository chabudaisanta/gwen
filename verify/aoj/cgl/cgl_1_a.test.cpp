#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/CGL_1_A"
#define ERROR   1e-8

#include <iomanip>
#include <iostream>

#include "gwen/geo/line.hpp"

using namespace gwen;
using namespace gwen::geo;

int main() {
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    Point<f80> a, b;
    std::cin >> a.x >> a.y >> b.x >> b.y;
    const Line<f80> line(a, b);
    i32 q;
    std::cin >> q;
    std::cout << std::fixed << std::setprecision(15);
    while (q--) {
        Point<f80> p;
        std::cin >> p.x >> p.y;
        const auto result = line.projection(p);
        std::cout << result.x << ' ' << result.y << '\n';
    }
}
