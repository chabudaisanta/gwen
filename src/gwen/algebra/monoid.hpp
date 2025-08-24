#pragma once

#include <concepts>

namespace gwen {

template <class T>
concept monoid = requires(T t, const typename T::S& s) {
    requires std::same_as<decltype(t.e), typename T::S>;
    { t.op(s, s) } -> std::same_as<typename T::S>;
};

template <class _S, class OP>
    requires requires(OP op, const _S& a, const _S& b) {
        { op(a, b) } -> std::same_as<_S>;
    }
struct Monoid {
    using S = _S;
    S e;
    OP op;
    explicit Monoid(S _e, const OP& _op = OP()) : e(_e), op(_op) {}
};

}  // namespace gwen