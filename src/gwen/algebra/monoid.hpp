#pragma once

// https://github.com/yosupo06/yosupo-library/blob/main/src/yosupo/algebra.hpp

#include <concepts>

namespace gwen {

template<class T>
concept monoid =
    requires (const typename T::S& a, const typename T::S& b) {
        { T::op(a, b) } -> std::same_as<typename T::S>;
    } && (
        std::same_as<decltype(T::e), typename T::S> ||
        std::same_as<decltype(T::e), const typename T::S>
    );

template<class Monoid, class Act, class Mapping>
concept acted_monoid =
    monoid<Monoid> &&
    monoid<Act> &&
    requires (const typename Act::S& f, const typename Monoid::S& x) {
        { Mapping::op(f, x) } -> std::same_as<typename Monoid::S>;
    };

} // namespace gwen