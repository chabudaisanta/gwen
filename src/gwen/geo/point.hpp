#pragma once

#include <algorithm>
#include <cmath>
#include <cassert>
namespace gwen {

template<typename T>
struct point {
    T x, y;

    point() : x(0), y(0) {}
    point(T x_, T y_) : x(x_), y(y_) {}

    auto norm() const { return std::hypot(x, y); }
    T norm_sq() const { return x * x + y * y; }

    point operator+(const point& other) const { return { x + other.x, y + other.y }; }
    point operator-(const point& other) const { return { x - other.x, y - other.y }; }
    point operator*(T k) const { return { x * k, y * k }; }
    point operator/(T k) const { assert(k != 0); return { x / k, y / k }; }
    friend point operator*(T k, const point& p) { return { p.x * k, p.y * k }; }

    point& operator+=(const point& other) { x = x + other.x; y = y + other.y; return *this; }
    point& operator-=(const point& other) { x = x - other.x; y = y - other.y; return *this; }
    point& operator*=(T k) { x = x * k; y = y * k; return *this; }
    point& operator/=(T k) { assert(k != 0); x = x / k; y = y / k; return *this; }
    
    
};

}