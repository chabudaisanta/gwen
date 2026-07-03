#pragma once

#include "gwen/algebra/basic_abel.hpp"

namespace gwen {

// 加法群 (sum_abel) の機能に加えて、乗法に関する演算 (mul) と乗法単位元 (e_mul) を定義する環
template <typename T> struct sum_ring : sum_abel<T> {
    using S = typename sum_abel<T>::S;
    static S mul(S a, S b) { return a * b; }
    static S e_mul() { return 1; }
};

}  // namespace gwen
