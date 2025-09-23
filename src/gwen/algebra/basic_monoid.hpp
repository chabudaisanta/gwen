#pragma once

#include <algorithm>
#include <concepts>
#include <limits>

#include "gwen/algebra/monoid.hpp"

namespace gwen {

template <typename T> struct min_monoid {
    using S = T;
    S e = std::numeric_limits<S>::max();
    S op(const S& a, const S& b) const { return std::min(a, b); }
};

template <typename T> struct max_monoid {
    using S = T;
    S e = std::numeric_limits<S>::min();
    S op(const S& a, const S& b) const { return std::max(a, b); }
};

template <std::integral T> struct sum_monoid {
    using S = T;
    S e = S(0);
    S op(const S& a, const S& b) const { return a + b; }
};

template <std::unsigned_integral T> struct xor_monoid {
    using S = T;
    S op(const S& a, const S& b) const { return a ^ b; }
    S e = S{0};
};

template <typename T> struct rangesum_monoid {
    struct S {
        T val;
        int len;
    };
    S op(const S& a, const S& b) const {
        return {a.val + b.val, a.len + b.len};
    }
    S e = {T{0}, 0};
};
template <typename T, typename U> struct rangesum_mapping {
    using S = T;
    using F = U;
    S operator()(const F& f, const S& x) const {
        return {f * x.len + x.val, x.len};
    }
};

}  // namespace gwen