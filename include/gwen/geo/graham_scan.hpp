#pragma once
#include <algorithm>
#include <vector>

#include "gwen/geo/point.hpp"

namespace gwen {
template <typename T, bool contain_on_boundary = false>
std::vector<point<T>> graham_scan(std::vector<point<T>> points) {
    using P = point<T>;
    if (points.size() <= 2) return points;

    P base = *std::min_element(points.begin(), points.end(), P::less_yx);
    for (P& p : points) p -= base;
    std::sort(points.begin(), points.end(), P::less_ang);

    if constexpr (contain_on_boundary) {
        i32 i = points.size() - 1;
        while (i >= 0 && P::cross_product(points.back(), points[i]) == 0) i--;
        std::reverse(points.begin() + (i + 1), points.end());
    }

    std::vector<P> ret;
    for (P p : points) {
        while (ret.size() >= 2) {
            i32 dir = P::ccw(ret[ret.size() - 2], ret[ret.size() - 1], p);
            if constexpr (contain_on_boundary) {
                if (dir != -1) break;
                ret.pop_back();
            }
            else {
                if (dir == 1) break;
                ret.pop_back();
            }
        }
        ret.emplace_back(p);
    }

    for (P& p : ret) p += base;
    return ret;
}

}  // namespace gwen