#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 座標圧縮を行います。
 * @details 計算量: \f$O(N \log N)\f$
 * @tparam T 要素の型
 * @param vec 圧縮する配列
 * @return 元の配列の各要素に対応する、0 から始まる圧縮後のインデックス（0-indexed）の配列
 */
template <typename T>
std::vector<i32> compress(const std::vector<T>& vec) {
    std::vector<T> copy = vec;
    std::ranges::sort(copy);
    auto [first, last] = std::ranges::unique(copy);
    copy.erase(first, last);
    
    std::vector<i32> ret(vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
        auto it = std::ranges::lower_bound(copy, vec[i]);
        ret[i] = static_cast<i32>(it - copy.begin());
    }
    return ret;
}

} // namespace gwen
