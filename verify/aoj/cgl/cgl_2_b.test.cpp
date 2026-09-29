#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/CGL_2_B"

#include <iostream>

#include "gwen/geo/segment.hpp"

using namespace gwen;
using namespace gwen::geo;

int main() {
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    i32 q;
    std::cin >> q;
    while (q--) {
        Point<f80> a, b, c, d;
        std::cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y >> d.x >> d.y;
        std::cout << Segment<f80>(a, b).intersects(Segment<f80>(c, d)) << '\n';
    }
}
