#pragma once

#include <cassert>
#include <concepts>

#include "gwen/geo/point.hpp"

namespace gwen::geo {

/**
 * @brief 異なる2点を通る直線
 * @tparam T 座標の浮動小数点型
 */
template <std::floating_point T> struct Line {
    /** @brief 直線上の第1点 */
    Point<T> a;
    /** @brief 直線上の第2点 */
    Point<T> b;

    /**
     * @brief 異なる2点を指定して直線を構築する
     * @param a 直線上の第1点
     * @param b 直線上の第2点
     * @pre 両点の座標が有限で、a != b
     */
    Line(Point<T> a, Point<T> b) : a(a), b(b) { validate(); }

    /**
     * @brief 点から直線への射影を求める
     * @param p 射影する点
     * @pre 有限の座標、a != b、中間演算と正の直線方向の長さが T で表現できること
     * @return 垂線の足。計算量 O(1)
     */
    Point<T> projection(const Point<T>& p) const {
        validate();
        internal::assert_finite(p);
        const auto direction = (b - a) / geo::distance(a, b);
        return a + direction * (p - a).dot(direction);
    }

    /**
     * @brief 点から直線までの距離を求める
     * @param p 対象の点
     * @pre projection と同じ制約
     * @return 最短距離。計算量 O(1)
     */
    T distance(const Point<T>& p) const { return geo::distance(p, projection(p)); }

private:
    void validate() const {
        internal::assert_finite(a);
        internal::assert_finite(b);
        assert(a != b);
    }
};

}  // namespace gwen::geo
