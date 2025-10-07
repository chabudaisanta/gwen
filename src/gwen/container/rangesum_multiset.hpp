#pragma once

#include <functional>

#include "gwen/algebra/basic_monoid.hpp"
#include "gwen/container/sorted_treap.hpp"

namespace gwen {

template <typename S>
class RangeSumMultiset : public sorted_treap<sum_monoid<S>, std::less<S>> {
private:
    inline static bool _initialized = []() {
        using Base = sorted_treap<sum_monoid<S>, std::less<S>>;
        Base::init(sum_monoid<S>{}, std::less<S>{});
        return true;
    }();

public:
    using sorted_treap<sum_monoid<S>, std::less<S>>::sorted_treap;
};

}  // namespace gwen