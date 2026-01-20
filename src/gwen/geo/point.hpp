#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <cassert>
#include "gwen/types.hpp"
namespace gwen {

template<typename T>
struct point {
    T x, y;

    point() : x(0), y(0) {}
    point(T x_, T y_) : x(x_), y(y_) {}

    auto norm() const { return std::hypot(x, y); }
    T norm_sq() const { return x * x + y * y; }
    T manhattan() const { return std::abs(x) + std::abs(y); }

    point operator+(const point& other) const { return { x + other.x, y + other.y }; }
    point operator-(const point& other) const { return { x - other.x, y - other.y }; }
    point operator*(T k) const { return { x * k, y * k }; }
    point operator/(T k) const { assert(k != 0); return { x / k, y / k }; }
    friend point operator*(T k, const point& p) { return { p.x * k, p.y * k }; }

    point& operator+=(const point& other) { x = x + other.x; y = y + other.y; return *this; }
    point& operator-=(const point& other) { x = x - other.x; y = y - other.y; return *this; }
    point& operator*=(T k) { x = x * k; y = y * k; return *this; }
    point& operator/=(T k) { assert(k != 0); x = x / k; y = y / k; return *this; }
    
    bool operator==(const point& other) const { return x == other.x && y == other.y; }
    bool operator!=(const point& other) const { return x != other.x || y != other.y; }

    friend std::istream& operator>>(std::istream& is, point& p) {
        is >> p.x >> p.y; return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const point& p) {
        os << p.x << ' ' << p.y; return os;
    }
    std::string dump() const {
        return std::string("(") + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    static T dot_product(const point& a, const point& b) { return a.x * b.x + a.y * b.y; }
    static T cross_product(const point& a, const point& b) { return a.x * b.y - a.y * b.x; }
    T dot(const point& other) const { return x * other.x + y * other.y; }
    T cross(const point& other) const { return x * other.y - y * other.x; }

    i32 quadrant() const {
        if(y >= 0) return x >= 0 ? 0 : 1;
        else return x >= 0 ? 3 : 2;
    }

    static bool less_xy(const point& a, const point& b) {
        if(a.x == b.x) return a.y < b.y;
        return a.x < b.x;
    }
    static bool less_yx(const point& a, const point& b) {
        if(a.y == b.y) return a.x < b.x;
        return a.y < b.y;
    }
    static bool less_ang(const point& a, const point& b) {
        if(a == b) return false;
        if(a.x == 0 && a.y == 0) return true;
        if(b.x == 0 && b.y == 0) return false;

        i32 qa = a.quadrant(), qb = b.quadrant();
        if(qa != qb) return qa < qb;

        T c = cross_product(a, b);
        if(c == 0) return a.norm_sq() < b.norm_sq();
        return c > 0;
    }
    // 進行方向 a -> b に対して、点 c がどこにあるか判定する
    // 1: Counter Clockwise (左に折れる)
    // -1: Clockwise (右に折れる)
    // 2: Online Back (c-a-b の順で一直線上)
    // -2: Online Front (a-b-c の順で一直線上)
    // 0: On Segment (a-c-b の順で一直線上 / 線分ab上にcがある)
    static i32 ccw(const point& a, point b, point c) {
        b -= a; c -= a; // aを原点に移動して考える
        T cp = cross_product(b, c);
        
        if (cp > 0) return 1;   // ccw
        if (cp < 0) return -1;  // cw
        if (dot_product(b, c) < 0) return 2; // back
        if (b.norm_sq() < c.norm_sq()) return -2; // front
        return 0; // on segment
    }
};

}