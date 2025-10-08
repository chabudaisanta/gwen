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
        i32 len;
        S() : val(T{0}), len(0) {}
        S(T v, int l) : val(v), len(l) {}
    };
    S op(const S& a, const S& b) const {
        return {a.val + b.val, a.len + b.len};
    }
    S e = {T{0}, 0};
};

template <class Monoid, class Act> struct rangesum_mapping {
    typename Monoid::S operator()(const typename Act::S& f,
                                  const typename Monoid::S& x) const {
        return {f * x.len + x.val, x.len};
    }
};

template <typename T>
struct rangesum_acted_monoid
    : ActedMonoid<rangesum_monoid<T>,
                  Sum<T>,
                  rangesum_mapping<rangesum_monoid<T>, Sum<T>>> {
    using base = ActedMonoid<rangesum_monoid<T>,
                             Sum<T>,
                             rangesum_mapping<rangesum_monoid<T>, Sum<T>>>;

    // デフォルトコンストラクタをちゃんと定義してあげる！
    rangesum_acted_monoid()
        : base(rangesum_monoid<T>{},  // rangesum_monoidをデフォルト構築
               Sum<T>{T{0}},          // Sum<T>の単位元は0だよ、と明示的に指定！
               rangesum_mapping<rangesum_monoid<T>, Sum<T>>{}
               // mappingをデフォルト構築
          ) {}

    using S = typename rangesum_monoid<T>::S;
    template <typename Itr>
    static std::vector<S> make_range(Itr begin, Itr end) {
        int N = end - begin;
        std::vector<S> ret(N);
        for (auto it = begin; auto& e : ret) {
            e = {*(it++), 1};
        }
        return ret;
    }
};

}  // namespace gwen