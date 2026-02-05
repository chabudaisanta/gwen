#pragma once

namespace gwen {

template <typename T> struct range_affine_range_sum_monoid {
    struct S {
        T val, len;
    };
    static S op(S a, S b) { return {a.val + b.val, a.len + b.len}; }
    static S e() { return {0, 0}; }
    static S unit(T x) { return {x, 1}; }

    struct F {
        T a, b;
    };
    static S mapping(F f, S x) { return {f.a * x.val + f.b * x.len, x.len}; }
    // f(g(x)) = a_f(a_g * x + b_g) + b_f = a_f * a_g * x + a_f * b_g + b_f
    static F composition(F f, F g) { return {f.a * g.a, f.a * g.b + f.b}; }
    static F id() { return {1, 0}; }
};

}  // namespace gwen