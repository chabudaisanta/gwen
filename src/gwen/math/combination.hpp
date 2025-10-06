#pragma once

#include <vector>

#include "gwen/mod/mod.hpp"
#include "gwen/types.hpp"

namespace gwen {

namespace internal {

template <typename T>
struct combination_table {
    i32 n;
    std::vector<T> F, I;

    explicit combination_table() : n(1), F({T(1)}), I({T(1)}) {
    }

    void extend() {
        assert(n < (1 << 30));
        n *= 2;
        F.resize(n, T(1));
        I.resize(n, T(1));
        F[0] = T(1);
        for (int i = 0; i < n - 1; ++i) {
            F[i + 1] = F[i] * (i + 1);
        }
        I.back() = T(1) / F.back();
        for (int i = n - 1; i > 0; --i) {
            I[i - 1] = I[i] * i;
        }
    }
};

}  // namespace internal

template <typename T>
T fact(int n) {
    static internal::combination_table<T> table;
    while (table.n <= n) table.extend();
    return table.F[n];
}

template <typename T>
T fact_inv(int n) {
    static internal::combination_table<T> table;
    while (table.n <= n) table.extend();
    return table.I[n];
}

template <i32 M = 998244353>
class combination {
private:
    const i32 n;
    std::vector<i64> F, I;

public:
    explicit combination() : combination(0) {}
    explicit combination(i32 n_) : n(n_), F(n + 1), I(n + 1) {
        F[0] = 1;
        for (i32 i = 1; i <= n; ++i) {
            F[i] = (F[i - 1] * i) % M;
        }
        I[n] = inv_mod(F[n], M);
        for (i32 i = n; i >= 1; --i) {
            I[i - 1] = (I[i] * i) % M;
        }
    }

    inline i64 C(i32 n_, i32 k_) const {
        assert(0 <= n_ && n_ <= n);
        if (k_ < 0 || n_ < k_) return 0;
        return (((F[n_] * I[n_ - k_]) % M) * I[k_]) % M;
    }

    inline i64 Pm(i32 n_, i32 k_) const {
        assert(0 <= n_ && n_ <= n);
        if (k_ < 0 || n_ < k_) return 0;
        return (F[n_] * I[n_ - k_]) % M;
    }
};

}  // namespace gwen