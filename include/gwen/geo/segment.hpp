#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <concepts>

#include "gwen/geo/point.hpp"

namespace gwen::geo {

/**
 * @brief 端点を含む線分。長さ0も許す
 * @tparam T 座標の浮動小数点型
 */
template <std::floating_point T> struct Segment {
    /** @brief 第1端点 */
    Point<T> a;
    /** @brief 第2端点 */
    Point<T> b;

    /**
     * @brief 2つの端点で線分を構築する
     * @param a 第1端点
     * @param b 第2端点
     * @pre 両点の座標が有限であること
     */
    Segment(Point<T> a, Point<T> b) : a(a), b(b) { validate(); }

    /**
     * @brief 点に最も近い線分上の点を求める
     * @param p 対象の点
     * @pre 有限の座標、中間演算と非退化時の正の長さが T で表現できること
     * @return 線分上の最近点。長さ0なら a。計算量 O(1)
     */
    Point<T> projection(const Point<T>& p) const {
        validate();
        internal::assert_finite(p);
        if (a == b) return a;
        if (p == a || p == b) return p;
        const T length = geo::distance(a, b);
        const auto direction = (b - a) / length;
        const T offset = (p - a).dot(direction);
        if (offset <= 0) return a;
        if (offset >= length) return b;
        return a + direction * offset;
    }

    /**
     * @brief 点から線分までの距離を求める
     * @param p 対象の点
     * @pre projection と同じ制約
     * @return 最短距離。計算量 O(1)
     */
    T distance(const Point<T>& p) const { return geo::distance(p, projection(p)); }

    /**
     * @brief 2線分の交差、または eps 以下の距離での近接を判定する
     * @param other 相手の線分
     * @param eps 座標と同じ単位の絶対許容誤差
     * @pre 有限の座標と eps、eps >= 0、中間演算が T で表現できること
     * @return 交差するか、最短距離が eps 以下なら true。計算量 O(1)
     */
    bool intersects(const Segment& other, T eps = T(1e-10)) const {
        validate();
        other.validate();
        assert(std::isfinite(eps) && eps >= 0);
        const T c1 = (b - a).cross(other.a - a), c2 = (b - a).cross(other.b - a);
        const T c3 = (other.b - other.a).cross(a - other.a), c4 = (other.b - other.a).cross(b - other.a);
        const auto opposite = [](T x, T y) { return (x < 0 && y > 0) || (x > 0 && y < 0); };
        if (opposite(c1, c2) && opposite(c3, c4)) return true;
        const auto on_segment = [](const Segment& s, const Point<T>& p, T cross) {
            return cross == 0 && std::min(s.a.x, s.b.x) <= p.x && p.x <= std::max(s.a.x, s.b.x) &&
                   std::min(s.a.y, s.b.y) <= p.y && p.y <= std::max(s.a.y, s.b.y);
        };
        if (on_segment(*this, other.a, c1) || on_segment(*this, other.b, c2) || on_segment(other, a, c3) ||
            on_segment(other, b, c4))
            return true;
        return std::min({distance(other.a), distance(other.b), other.distance(a), other.distance(b)}) <= eps;
    }

private:
    void validate() const {
        internal::assert_finite(a);
        internal::assert_finite(b);
    }
};

}  // namespace gwen::geo
