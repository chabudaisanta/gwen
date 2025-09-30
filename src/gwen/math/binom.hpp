#pragma once
#include <cassert>
#include <vector>
namespace gwen {

template <typename T>
struct binom {
    std::vector<std::vector<T>> d;

    explicit binom() : d(1, {T(1)}) {}

    inline int size() const {
        return d.size();
    }
    void extend() {
        assert(size() <= 30000);  // 9e8
        std::vector<T>& prv = d.back();
        int m = prv.size();
        std::vector<T> buf(m + 1, T(0));
        for (int i = 0; i < m; ++i) {
            buf[i] += prv[i];
            buf[i + 1] += prv[i];
        }
        d.emplace_back(std::move(buf));
    }

    T get(int n, int k) {
        if (k < 0 || n < k) return 0;
        while (size() <= n) extend();
        return d[n][k];
    }

    T operator()(int n, int k) {
        return get(n, k);
    }
};

}  // namespace gwen