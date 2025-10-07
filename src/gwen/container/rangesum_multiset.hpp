#pragma once

#include <functional>
#include "gwen/container/sorted_treap.hpp"
#include "gwen/algebra/basic_monoid.hpp"

namespace gwen {

template<typename S>
class RangeSumMultiset : public sorted_treap<sum_monoid<S>, std::less<S>> {
private:
    inline static bool _initialized = [](){
        using Base = sorted_treap<sum_monoid<S>, std::less<S>>;
        Base::init(sum_monoid<S>{}, std::less<S>{});
        return true;
    }();

public:
    using sorted_treap<sum_monoid<S>, std::less<S>>::sorted_treap;
};

} // namespace gwen