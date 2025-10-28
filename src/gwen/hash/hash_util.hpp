#pragma once
// https://qiita.com/hamamu/items/4d081751b69aa3bb3557

#include <tuple>
#include <utility>
#include <vector>

template <class T> size_t HashCombine(const size_t seed, const T& v) {
    return seed ^ (std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}
/* pair用 */
template <class T, class S> struct std::hash<std::pair<T, S>> {
    size_t operator()(const std::pair<T, S>& keyval) const noexcept {
        return HashCombine(std::hash<T>()(keyval.first), keyval.second);
    }
};

/* vector用 */
template <class T> struct std::hash<std::vector<T>> {
    size_t operator()(const std::vector<T>& keyval) const noexcept {
        size_t s = 0;
        for (auto&& v : keyval) s = HashCombine(s, v);
        return s;
    }
};

/* tuple用 */
template <int N> struct HashTupleCore {
    template <class Tuple>
    size_t operator()(const Tuple& keyval) const noexcept {
        size_t s = HashTupleCore<N - 1>()(keyval);
        return HashCombine(s, std::get<N - 1>(keyval));
    }
};
template <> struct HashTupleCore<0> {
    template <class Tuple>
    size_t operator()(const Tuple& keyval) const noexcept {
        return 0;
    }
};
template <class... Args> struct std::hash<std::tuple<Args...>> {
    size_t operator()(const tuple<Args...>& keyval) const noexcept {
        return HashTupleCore<tuple_size<tuple<Args...>>::value>()(keyval);
    }
};