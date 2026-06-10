#pragma once
#include <cassert>
#include <vector>
namespace gwen {

template <typename T> struct binom {
    std::vector<std::vector<T>> d;
    std::vector<std::vector<T>> pref;  // prefix sums over k: pref[n][i] = sum_{k=0..i} C(n,k)

    explicit binom() : d(1, {T(1)}), pref(1, {T(1)}) {}

    inline int size() const { return d.size(); }
    void extend() {
        assert(size() <= 30000);  // 9e8
        std::vector<T>& prv = d.back();
        int m = prv.size();
        std::vector<T> buf(m + 1, T(0));
        for (int i = 0; i < m; ++i) {
            buf[i] += prv[i];
            buf[i + 1] += prv[i];
        }
        std::vector<T> p(m + 1, T(0));
        T acc = T(0);
        for (int i = 0; i <= m; ++i) {
            acc += buf[i];
            p[i] = acc;
        }
        d.emplace_back(std::move(buf));
        pref.emplace_back(std::move(p));
    }

    T get(int n, int k) {
        if (k < 0 || n < k) return 0;
        while (size() <= n) extend();
        return d[n][k];
    }

    T operator()(int n, int k) { return get(n, k); }

    // sum_{k in [l, r)} C(n, k)
    T range_sum(int n, int l, int r) {
        if (n < 0) return T(0);
        if (l < 0) l = 0;
        if (r > n + 1) r = n + 1;
        if (l >= r) return T(0);
        while (size() <= n) extend();
        // [l, r) -> [0, r-1] - [0, l-1]
        T res = pref[n][r - 1];
        if (l > 0) res -= pref[n][l - 1];
        return res;
    }
};

}  // namespace gwen