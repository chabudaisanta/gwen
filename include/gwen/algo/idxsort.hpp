#pragma once
#include <vector>
#include <numeric>
#include <algorithm>

namespace gwen {

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
