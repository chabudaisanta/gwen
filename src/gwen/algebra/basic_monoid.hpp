#pragma once
#include <algorithm>
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

template <typename T> struct gcd_monoid {
    using S = T;
    static S op(S a, S b) {
        if(b == 0) return a;
        return op(b, a % b);
    }
    static S e() { return 0; }
}

}  // namespace gwen