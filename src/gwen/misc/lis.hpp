#pragma once
#include <algorithm>
#include <limits>
#include <vector>
namespace gwen {
template <typename T,
          T min = std::numeric_limits<T>::min(),
          T max = std::numeric_limits<T>::max()>
std::vector<T> LIS(const std::vector<T>& vec) {
    int n = vec.size();
    std::vector<T> dp(n, max), prv(n, min);
    for (int i = 0; i < n; ++i) {
        int p = std::upper_bound(dp.begin(), dp.end(), vec[i]) - dp.begin();
        prv[i] = (p == 0 ? min : dp[p - 1]);
        dp[p] = vec[i];
    }
    while (dp.size() && dp.back() == max) dp.pop_back();
    assert(dp.size());

    T x = dp.back();
    std::vector<T> ret;
    for (int i = n - 1; i >= 0 && x != min; --i) {
        if (vec[i] == x) {
            ret.emplace_back(x);
            x = prv[i];
        }
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
}

template <typename T,
          T min = std::numeric_limits<T>::min(),
          T max = std::numeric_limits<T>::max()>
std::vector<T> LIS_strict(const std::vector<T>& vec) {
    int n = vec.size();
    std::vector<T> dp(n, max), prv(n, min);
    for (int i = 0; i < n; ++i) {
        int p = std::lower_bound(dp.begin(), dp.end(), vec[i]) - dp.begin();
        prv[i] = (p == 0 ? min : dp[p - 1]);
        dp[p] = vec[i];
    }
    while (dp.size() && dp.back() == max) dp.pop_back();
    assert(dp.size());

    T x = dp.back();
    std::vector<T> ret;
    for (int i = n - 1; i >= 0 && x != min; --i) {
        if (vec[i] == x) {
            ret.emplace_back(x);
            x = prv[i];
        }
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
}

}  // namespace gwen