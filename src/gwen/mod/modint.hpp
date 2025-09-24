#pragma once
// https://rsk0315.hatenablog.com/entry/2022/11/27/060616
#include <cassert>

#include "gwen/mod/mod.hpp"
#include "gwen/types.hpp"

namespace gwen {

class dynamic_modint64 {
    using m64 = dynamic_modint64;
    static inline u64 n = 1;
    static inline u64 ns = 0;
    static inline u64 r2 = 0;
    static inline constexpr u64 msk = -1;

    u64 reduce_mul(u64 a, u64 b) const {
        u128 t = static_cast<u128>(a) * b;
        u128 m = (t * ns) & msk;  // maybe unnecessary mask ?
        a = (m * n + t) >> 64;    // t ← (t + mN) / R
        return a < n ? a : a - n;
    }

    u64 tr;

public:
    static void set_mod(u64 n_) {
        assert(n_ < (1ull << 62));
        assert(n_ & 1);
        n = n_;
        ns = n;
        for (int i = 0; i < 5; ++i) ns *= 2 - ns * n;
        assert(ns * n == 1);
        ns = -ns;
        r2 = -static_cast<u128>(n) % n;
    }

    dynamic_modint64() : tr(0) {}

    // need 0 <= abs(x) < RN
    template <std::unsigned_integral T>
    dynamic_modint64(T x) : tr(reduce_mul(x, r2)) {}
    template <std::signed_integral T>
    dynamic_modint64(T x) : tr(0) {
        if (x < 0)
            sub(m64{static_cast<u64>(-x)});
        else
            tr = reduce_mul(x, r2);
    }

    u64 val() const { return reduce_mul(tr, 1); }

    // basic operation
    m64& sub(const m64& x) {
        if (tr < x.tr) tr += n;
        tr -= x.tr;
        return *this;
    }
    m64& add(const m64& x) {
        tr += x.tr;
        if (tr >= n) tr -= n;
        return *this;
    }
    m64& mul(const m64& x) {
        tr = reduce_mul(tr, x.tr);
        return *this;
    }

    // operator overload
    m64& operator+=(const m64& x) { return add(x); }
    m64 operator+(const m64& x) const {
        m64 tmp = *this;
        tmp.add(x);
        return tmp;
    }

    m64& operator-=(const m64& x) { return sub(x); }
    m64 operator-(const m64& x) const {
        m64 tmp = *this;
        tmp.sub(x);
        return tmp;
    }

    m64& operator*=(const m64& x) { return mul(x); }
    m64 operator*(const m64& x) const {
        m64 tmp = *this;
        tmp.mul(x);
        return tmp;
    }

    // operator overload (integral)
    template <std::integral T>
    m64& operator+=(T x) { return add(m64(x)); }
    template <std::integral T>
    m64 operator+(T x) const {
        return *this + m64(x);
    }

    template <std::integral T>
    m64& operator-=(T x) { return sub(m64(x)); }
    template <std::integral T>
    m64 operator-(T x) const {
        return *this - m64(x);
    }

    template <std::integral T>
    m64& operator*=(T x) { return mul(m64(x)); }
    template <std::integral T>
    m64 operator*(T x) const {
        return *this * m64(x);
    }

    template <std::integral T>
    m64 pow(T x) const {
        assert(x >= 0);
        m64 ret(1);
        m64 tmp(*this);
        while (x) {
            if (x & 1) ret.mul(tmp);
            tmp.tr = reduce_mul(tmp.tr, tmp.tr);
            x >>= 1;
        }
        return ret;
    }
};

}  // namespace gwen