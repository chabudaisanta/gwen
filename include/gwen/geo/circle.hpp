#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <utility>

#include "gwen/geo/point.hpp"
#include "gwen/types.hpp"

namespace gwen::geo {

/**
 * @brief 中心と非負の半径で表す円周
 * @tparam T 座標と半径の浮動小数点型
 */
template <std::floating_point T> struct Circle {
    /** @brief 円の中心 */
    Point<T> center;
    /** @brief 半径。0のとき円周は中心1点 */
    T radius;

    /**
     * @brief 中心と半径で円を構築する
     * @param center 中心
     * @param radius 非負の半径
     * @pre 中心と半径が有限で、radius >= 0
     */
    Circle(Point<T> center, T radius) : center(center), radius(radius) {
        internal::assert_finite(center);
        assert(std::isfinite(radius) && radius >= 0);
    }
};

/**
 * @brief 円周同士の交点を求める
 * @tparam T 座標と半径の浮動小数点型
 * @param c1 第1の円
 * @param c2 第2の円
 * @param eps 非交差時に近似接点を許す絶対誤差。座標と同じ単位
 * @pre 全入力が有限、半径と eps が非負、中間演算と非零の除数が T で表現できること
 * @return 交点数と最大2点。0/1/2、同一の正半径円なら -1。
 *         1では配列の先頭だけ、2では辞書式順の両要素が有効。計算量 O(1)
 * @details 真の2交点は eps で統合しない。同中心異半径は常に0。
 *          非交差の異中心円では円周間隔が eps 以下なら最寄り円周点の中点を返す。
 *          点円では相手円周まで eps 以下なら点円の中心を返す。
 */
template <std::floating_point T>
std::pair<i32, std::array<Point<T>, 2>> intersection(const Circle<T>& c1, const Circle<T>& c2, T eps = T(1e-10)) {
    internal::assert_finite(c1.center);
    internal::assert_finite(c2.center);
    assert(std::isfinite(c1.radius) && c1.radius >= 0);
    assert(std::isfinite(c2.radius) && c2.radius >= 0);
    assert(std::isfinite(eps) && eps >= 0);
    if (c1.center == c2.center) {
        if (c1.radius != c2.radius) return {0, {}};
        if (c1.radius == 0) return {1, {c1.center, {}}};
        return {-1, {}};
    }
    const T d = distance(c1.center, c2.center);
    if (c1.radius == 0 || c2.radius == 0) {
        const auto& point_circle = c1.radius == 0 ? c1 : c2;
        const T radius = c1.radius == 0 ? c2.radius : c1.radius;
        if (std::abs(d - radius) <= eps) return {1, {point_circle.center, {}}};
        return {0, {}};
    }
    const auto direction = (c2.center - c1.center) / d;
    const T sum = c1.radius + c2.radius;
    const T delta = std::abs(c1.radius - c2.radius);
    if (d > sum) {
        if (d - sum > eps) return {0, {}};
        const auto p = c1.center + direction * c1.radius;
        const auto q = c2.center - direction * c2.radius;
        return {1, {p + (q - p) / T(2), {}}};
    }
    if (d < delta) {
        if (delta - d > eps) return {0, {}};
        const T sign = c1.radius > c2.radius ? T(1) : T(-1);
        const auto p = c1.center + direction * (sign * c1.radius);
        const auto q = c2.center + direction * (sign * c2.radius);
        return {1, {p + (q - p) / T(2), {}}};
    }
    if (d == sum || d == delta) {
        const T offset = d == sum || c1.radius > c2.radius ? c1.radius : -c1.radius;
        return {1, {c1.center + direction * offset, {}}};
    }
    const T offset = (d + (c1.radius - c2.radius) * sum / d) / T(2);
    const auto base = c1.center + direction * offset;
    const T height = std::sqrt(std::max(T(0), (c1.radius - offset) * (c1.radius + offset)));
    const Point<T> perpendicular(-direction.y * height, direction.x * height);
    std::array<Point<T>, 2> points{base - perpendicular, base + perpendicular};
    if (CompareXy{}(points[1], points[0])) std::swap(points[0], points[1]);
    return {2, points};
}

}  // namespace gwen::geo
