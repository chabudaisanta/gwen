#pragma once
#include <algorithm>
#include <numeric>
#include <limits>
namespace gwen {

template <typename T> struct sum_monoid {
    using S = T;
    static S op(S a, S b) { return a + b; }
    static S e() { return 0; }
};

template <typename T> struct xor_monoid {
    using S = T;
    static S op(S a, S b) { return a ^ b; }
    static S e() { return 0; }
};

template <typename T> struct min_monoid {
    using S = T;
    static S op(S a, S b) { return std::min(a, b); }
    static S e() { return std::numeric_limits<S>::max(); }
};

template <typename T> struct max_monoid {
    using S = T;
    static S op(S a, S b) { return std::max(a, b); }
    static S e() { return std::numeric_limits<S>::min(); }
};

template <typename T> struct minmax_monoid {
    struct S {
        i32 min, max;
    };
    static S op(S a, S b) {
        return { std::min(a.min, b.min), std::max(a.max, b.max) };
    }
    static S e() {
        return { std::numeric_limits<T>::min(), std::numeric_limits<T>::max() };
    }
};

template <typename T> struct gcd_monoid {
    using S = T;
    static S op(S a, S b) {
        return std::gcd(a, b);
    }
    static S e() { return 0; }
};

template<typename T> struct affine_monoid {
    struct S {
        T a, b;
    };
    static S op(S f, S g) {
        return { f.a * g.a, f.a * g.b + f.b };
    }
    static S e() { return {1, 0}; }
};

}  // namespace gwen