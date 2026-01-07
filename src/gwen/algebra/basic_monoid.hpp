#pragma once
#include <algorithm>
#include <limits>
namespace gwen {

template <typename T> struct sum_monoid {
    using S = T;
    static S op(S a, S b) { return a + b; }
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

}  // namespace gwen