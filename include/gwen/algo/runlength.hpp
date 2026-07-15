#pragma once
#include <vector>
#include <utility>
#include <iterator>
#include "gwen/types.hpp"

namespace gwen {

template <typename Iterator>
auto runlength(Iterator begin, Iterator end) {
    using ValueType = typename std::iterator_traits<Iterator>::value_type;
    std::vector<std::pair<ValueType, i32>> ret;

    for (auto it = begin; it != end; ++it) {
        if (ret.empty() || ret.back().first != *it) {
            ret.emplace_back(*it, 0);
        }
        ret.back().second++;
    }
    return ret;
}

} // namespace gwen
