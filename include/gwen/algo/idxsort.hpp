#pragma once
#include <vector>
#include <numeric>
#include <algorithm>

namespace gwen {

/**
 * @brief 配列の要素を元に、インデックスをソートする
 * @tparam T 配列の要素型
 * @param vec ソート基準となる配列
 * @param rev true の場合、降順にソートする (デフォルトは false)
 * @return std::vector<int> ソートされたインデックスの配列
 */
template <typename T>
std::vector<int> idxsort(const std::vector<T>& vec, bool rev = false) {
    std::vector<int> ret(vec.size());
    std::iota(ret.begin(), ret.end(), 0);
    std::sort(ret.begin(), ret.end(), [&vec, rev](int a, int b) {
        return (rev ? vec[a] > vec[b] : vec[a] < vec[b]);
    });
    return ret;
}

} // namespace gwen
