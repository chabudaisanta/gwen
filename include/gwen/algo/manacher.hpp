#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <limits>
#include <ranges>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 各位置を中心とする奇数長・偶数長回文の半径を求める。
 * @tparam Range 等値比較できる要素を持つランダムアクセス可能な列
 * @param s 入力列
 * @return `{odd, even}`。`odd[i]` は `i` 中心、`even[i]` は `i-1` と `i` の間を中心とする半径
 */
template <typename Range>
    requires std::ranges::random_access_range<const Range> && std::ranges::sized_range<const Range> &&
             std::equality_comparable<std::ranges::range_reference_t<const Range>>
auto manacher(const Range& s) -> std::pair<std::vector<i32>, std::vector<i32>> {
    assert(std::ranges::size(s) <= static_cast<usize>(std::numeric_limits<i32>::max()));
    const i32 n = static_cast<i32>(std::ranges::size(s));
    const auto first = std::ranges::begin(s);
    std::vector<i32> odd(n), even(n);

    for (i32 i = 0, left = 0, right = -1; i < n; ++i) {
        i32 radius = i > right ? 1 : std::min(odd[left + right - i], right - i + 1);
        while (radius <= i && radius < n - i && first[i - radius] == first[i + radius]) ++radius;
        odd[i] = radius;
        if (i + radius - 1 > right) {
            left = i - radius + 1;
            right = i + radius - 1;
        }
    }

    for (i32 i = 0, left = 0, right = -1; i < n; ++i) {
        i32 radius = i > right ? 0 : std::min(even[left + right - i + 1], right - i + 1);
        while (radius < i && radius < n - i && first[i - radius - 1] == first[i + radius]) ++radius;
        even[i] = radius;
        if (i + radius - 1 > right) {
            left = i - radius;
            right = i + radius - 1;
        }
    }
    return {std::move(odd), std::move(even)};
}

}  // namespace gwen
