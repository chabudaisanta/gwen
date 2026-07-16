#pragma once
#include <vector>
#include <numeric>
#include <algorithm>
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 配列の要素を元に、インデックスをソートする
 * @tparam T 配列の要素型
 * @param vec ソート基準となる配列
 * @param rev true の場合、降順にソートする (デフォルトは false)
 * @return std::vector<i32> ソートされたインデックスの配列
 */
template <typename T>
std::vector<i32> idxsort(const std::vector<T>& vec, bool rev = false) {
    std::vector<i32> ret(vec.size());
    std::iota(ret.begin(), ret.end(), 0);
    std::sort(ret.begin(), ret.end(), [&vec, rev](i32 a, i32 b) {
        return (rev ? vec[a] > vec[b] : vec[a] < vec[b]);
    });
    return ret;
}

} // namespace gwen
