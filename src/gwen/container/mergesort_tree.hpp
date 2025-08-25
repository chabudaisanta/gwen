#pragma once

#include <algorithm>
#include <bit>
#include <vector>

namespace gwen {

template <typename T> class mergesort_tree {
private:
    int n, bc;
    std::vector<std::vector<T>> d;

public:
    explicit mergesort_tree() {}
    explicit mergesort_tree(const std::vector<T>& vec)
        : n(vec.size()), bc(std::bit_ceil(vec.size())) {
        d.assign(bc * 2, std::vector<T>());
        for (int i = 0; i < n; ++i) {
            d[i + bc].emplace_back(vec[i]);
        }
        for (int i = bc - 1; i >= 1; --i) {
            d[i].reserve(d[i * 2].size() + d[i * 2 + 1].size());
            std::merge(d[i * 2].begin(), d[i * 2].end(), d[i * 2 + 1].begin(),
                       d[i * 2 + 1].end(), std::back_inserter(d[i]));
        }
    }

    int lower_cnt(int l, int r, const T& x) {
        assert(0 <= l && l <= r && r <= n && "lower_cnt");
        l += bc;
        r += bc;
        int ret = 0;

        while (l < r) {
            if (l & 1) {
                ret += lower_bound(d[l].begin(), d[l].end(), x) - d[l].begin();
                l++;
            }
            if (r & 1) {
                --r;
                ret += lower_bound(d[r].begin(), d[r].end(), x) - d[r].begin();
            }
            l >>= 1;
            r >>= 1;
        }
        return ret;
    }

private:
};

}  // namespace gwen