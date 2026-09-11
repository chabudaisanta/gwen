#pragma once

#include <cassert>
#include <concepts>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 配列の過半数要素を求める
 * @tparam T コピー構築・コピー代入および等値比較が可能な型
 * @param values 空でない入力配列
 * @return 過半数要素が存在するかと、その候補の組
 * @details 戻り値の first が false のとき、second の値は意味を持たない。
 */
template <std::copy_constructible T>
    requires std::assignable_from<T&, const T&> && std::equality_comparable<T>
std::pair<bool, T> majority_vote(const std::vector<T>& values) {
    assert(!values.empty());

    std::pair<T, usize> candidate{values.front(), 0};
    for (const T& value : values) {
        if (candidate.second == 0) {
            candidate = {value, 1};
        }
        else if (candidate.first == value) {
            ++candidate.second;
        }
        else {
            --candidate.second;
        }
    }

    usize count = 0;
    for (const T& value : values) {
        count += value == candidate.first;
    }
    return {count > values.size() / 2, candidate.first};
}

}  // namespace gwen
