#pragma once

#include "gwen/types.hpp"
#include <concepts>
#include <cmath>

namespace gwen {
namespace geo {

/**
 * @brief 2次元座標を表すクラス
 * @tparam T 座標の型
 */
template <typename T>
    requires std::integral<T> || std::floating_point<T>
struct Point {
    /**
     * @brief x座標
     */
    T x;
    /**
     * @brief y座標
     */
    T y;

    /**
     * @brief デフォルトコンストラクタ。原点で初期化する。
     */
    Point() : x(0), y(0) {}

    /**
     * @brief 座標を指定して初期化するコンストラクタ
     * @param x x座標
     * @param y y座標
     */
    Point(T x, T y) : x(x), y(y) {}

    /**
     * @brief ベクトルの加算
     * @param p 加算するベクトル
     * @return Point 加算結果
     */
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }

    /**
     * @brief ベクトルの減算
     * @param p 減算するベクトル
     * @return Point 減算結果
     */
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }

    /**
     * @brief ベクトルのスカラー倍
     * @param k 乗数
     * @return Point 乗算結果
     */
    Point operator*(T k) const { return Point(x * k, y * k); }

    /**
     * @brief ベクトルのスカラー除算
     * @param k 除数
     * @return Point 除算結果
     */
    Point operator/(T k) const { return Point(x / k, y / k); }

    /**
     * @brief 複合代入演算子 (加算)
     * @param p 加算するベクトル
     * @return Point& 自身の参照
     */
    Point& operator+=(const Point& p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    /**
     * @brief 複合代入演算子 (減算)
     * @param p 減算するベクトル
     * @return Point& 自身の参照
     */
    Point& operator-=(const Point& p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    /**
     * @brief 複合代入演算子 (スカラー倍)
     * @param k 乗数
     * @return Point& 自身の参照
     */
    Point& operator*=(T k) {
        x *= k;
        y *= k;
        return *this;
    }

    /**
     * @brief 複合代入演算子 (スカラー除算)
     * @param k 除数
     * @return Point& 自身の参照
     */
    Point& operator/=(T k) {
        x /= k;
        y /= k;
        return *this;
    }

    /**
     * @brief 等価比較演算子
     * @param p 比較する点
     * @return true 座標が等しい場合
     * @return false 座標が異なる場合
     */
    bool operator==(const Point& p) const { return x == p.x && y == p.y; }

    /**
     * @brief 非等価比較演算子
     * @param p 比較する点
     * @return true 座標が異なる場合
     * @return false 座標が等しい場合
     */
    bool operator!=(const Point& p) const { return !(*this == p); }

    /**
     * @brief 内積を計算する
     * @param p 対象の点
     * @return T 内積
     */
    T dot(const Point& p) const { return x * p.x + y * p.y; }

    /**
     * @brief 外積を計算する
     * @param p 対象の点
     * @return T 外積
     */
    T cross(const Point& p) const { return x * p.y - y * p.x; }

    /**
     * @brief 原点からの距離の2乗を計算する
     * @return T 距離の2乗
     */
    T norm() const { return x * x + y * y; }
};

/**
 * @brief x座標を基準に比較するファンクタ
 */
struct CompareX {
    /**
     * @brief 関数呼び出し演算子
     * @tparam T 座標の型
     * @param a 比較する点a
     * @param b 比較する点b
     * @return true aのx座標がbより小さい場合
     */
    template <typename T>
    bool operator()(const Point<T>& a, const Point<T>& b) const {
        return a.x < b.x;
    }
};

/**
 * @brief y座標を基準に比較するファンクタ
 */
struct CompareY {
    /**
     * @brief 関数呼び出し演算子
     * @tparam T 座標の型
     * @param a 比較する点a
     * @param b 比較する点b
     * @return true aのy座標がbより小さい場合
     */
    template <typename T>
    bool operator()(const Point<T>& a, const Point<T>& b) const {
        return a.y < b.y;
    }
};

/**
 * @brief x座標、y座標の辞書式順に比較するファンクタ
 */
struct CompareXy {
    /**
     * @brief 関数呼び出し演算子
     * @tparam T 座標の型
     * @param a 比較する点a
     * @param b 比較する点b
     * @return true aが辞書式順でbより小さい場合
     */
    template <typename T>
    bool operator()(const Point<T>& a, const Point<T>& b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

namespace internal {
/**
 * @brief 点が属する象限を返す (内部用)
 * @tparam T 座標の型
 * @param p 対象の点
 * @return int 象限番号。原点は-1
 */
template <typename T>
i32 quadrant(const Point<T>& p) {
    if (p.y < 0) return 3;
    if (p.y > 0) return 1;
    if (p.x < 0) return 2;
    if (p.x > 0) return 0;
    return -1; // origin
}
} // namespace internal

/**
 * @brief 偏角を基準に比較するファンクタ
 */
struct CompareArg {
    /**
     * @brief 関数呼び出し演算子
     * @tparam T 座標の型
     * @param a 比較する点a
     * @param b 比較する点b
     * @return true aの偏角がbの偏角より小さい場合
     */
    template <typename T>
    bool operator()(const Point<T>& a, const Point<T>& b) const {
        i32 qa = internal::quadrant(a);
        i32 qb = internal::quadrant(b);
        if (qa != qb) return qa < qb;
        T c = a.cross(b);
        return c > 0;
    }
};

/**
 * @brief 偏角、次いでノルムを基準に比較するファンクタ
 */
struct CompareArgNorm {
    /**
     * @brief 関数呼び出し演算子
     * @tparam T 座標の型
     * @param a 比較する点a
     * @param b 比較する点b
     * @return true 偏角・距離でaがbより小さい場合
     */
    template <typename T>
    bool operator()(const Point<T>& a, const Point<T>& b) const {
        i32 qa = internal::quadrant(a);
        i32 qb = internal::quadrant(b);
        if (qa != qb) return qa < qb;
        T c = a.cross(b);
        if (c != 0) return c > 0;
        return a.norm() < b.norm();
    }
};

/**
 * @brief 3点の位置関係（CCW）を判定する
 * @tparam T 座標の型
 * @param a 点a
 * @param b 点b
 * @param c 点c
 * @return int 1: 反時計回り, -1: 時計回り, 2: c-a-b 直線, -2: a-b-c 直線, 0: a-c-b 直線
 */
template <typename T>
i32 ccw(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
    Point<T> ab = b - a;
    Point<T> ac = c - a;
    T cr = ab.cross(ac);
    if (cr > 0) return 1;
    if (cr < 0) return -1;
    if (ab.dot(ac) < 0) return 2;
    if (ab.norm() < ac.norm()) return -2;
    return 0;
}

} // namespace geo
} // namespace gwen
