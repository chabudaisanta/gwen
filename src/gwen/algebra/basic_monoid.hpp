#pragma once

#include <algorithm>
#include <concepts>
#include <limits>

#include "gwen/types.hpp"
#include "gwen/algebra/monoid.hpp"

namespace gwen {
template<typename T>
struct min_monoid {
    using S = T;
    static S op(S a, S b) {
        return std::min(a, b);
    }
    static inline const S e = std::numeric_limits<S>::max();
};
template<typename T>
struct max_monoid {
    using S = T;
    static S op(S a, S b) {
        return std::max(a, b);
    }
    static inline const S e = std::numeric_limits<S>::min();
};
template<typename T>
struct add_monoid {
    using S = T;
    static S op(S a, S b) {
        return a + b;
    }
    static inline const S e = S();
};
template<typename T>
struct rangesum_monoid {
    struct S {
        T val;
        i64 len;
    };
    static S op(const S& a, const S& b) {
        return { a.val + b.val, a.len + b.len };
    }
    static inline const S e = { 0, 0 };
};

} // namespace gwen