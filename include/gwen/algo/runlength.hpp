#pragma once
#include <vector>
#include <utility>
#include <iterator>
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief ランレングス圧縮（連長圧縮）を行う
 * @tparam Iterator イテレータの型
 * @param begin 圧縮対象の開始イテレータ
 * @param end 圧縮対象の終了イテレータ
 * @return std::vector<std::pair<ValueType, i32>> 要素とその連続回数のペアの配列
 */
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
