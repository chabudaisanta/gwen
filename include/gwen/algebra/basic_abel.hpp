#pragma once

#include "gwen/algebra/basic_monoid.hpp"
namespace gwen {

template <typename T> struct sum_abel : sum_monoid<T> {
    using S = typename sum_monoid<T>::S;
    static S inv(S x) { return -x; }
};

template <typename T> struct xor_abel : xor_monoid<T> {
    using S = typename xor_monoid<T>::S;
    static S inv(S x) { return x; }
};

}  // namespace gwen