#pragma once

#include "gwen/types.hpp"
#include "gwen/geo/point.hpp"
#include <vector>
#include <algorithm>

namespace gwen {
namespace geo {

/**
 * @brief 凸多角形（凸包）を構築する
 * @tparam T 座標の型
 * @param pts 点の集合
 * @return std::vector<Point<T>> 凸多角形を構成する点の列 (反時計回り)
 */
template <typename T>
std::vector<Point<T>> convex_polygon(std::vector<Point<T>> pts) {
    if (pts.size() <= 2) {
        std::sort(pts.begin(), pts.end(), CompareXy{});
        pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
        return pts;
    }
    std::sort(pts.begin(), pts.end(), CompareXy{});
    pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
    if (pts.size() <= 2) return pts;

    std::vector<Point<T>> res;
    res.reserve(pts.size() * 2);

    // 下側凸包
    for (const auto& p : pts) {
        while (res.size() > 1) {
            Point<T> pre = res[res.size() - 2];
            Point<T> back = res.back();
            if ((back - pre).cross(p - pre) <= 0) {
                res.pop_back();
            } else {
                break;
            }
        }
        res.push_back(p);
    }

    // 上側凸包
    usize t = res.size();
    for (i32 i = static_cast<i32>(pts.size()) - 2; i >= 0; --i) {
        const auto& p = pts[i];
        while (res.size() > t) {
            Point<T> pre = res[res.size() - 2];
            Point<T> back = res.back();
            if ((back - pre).cross(p - pre) <= 0) {
                res.pop_back();
            } else {
                break;
            }
        }
        res.push_back(p);
    }
    res.pop_back();
    return res;
}

} // namespace geo
} // namespace gwen
