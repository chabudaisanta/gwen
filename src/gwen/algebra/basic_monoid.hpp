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

}  // namespace gwen