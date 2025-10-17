#pragma once

#include <vector>

#include "gwen/types.hpp"

namespace gwen {

namespace internal {

template <typename T> struct combination_table {
    i32 n;
    std::vector<T> F, I;

    explicit combination_table() : n(1), F({T(1)}), I({T(1)}) {}

    void extend() {
        assert(n < (1 << 30));
        n *= 2;
        F.resize(n, T(1));
        I.resize(n, T(1));
        F[0] = T(1);
        for (i32 i = 0; i < n - 1; ++i) {
            F[i + 1] = F[i] * (i + 1);
        }
        I.back() = T(1) / F.back();
        for (i32 i = n - 1; i > 0; --i) {
            I[i - 1] = I[i] * i;
        }
    }
};

template<typename T>
combination_table<T>& get_comb_table() {
    static combination_table<T> table;
    return table;
}

}  // namespace internal

template <typename T> T fact(i32 n) {
    auto& table = internal::get_comb_table();
    while (table.n <= n) table.extend();
    return table.F[n];
}

template <typename T> T fact_inv(i32 n) {
    auto& table = internal::get_comb_table();
    while (table.n <= n) table.extend();
    return table.I[n];
}

template<typename T> T comb(i32 n, i32 k) {
    if(k < 0 || n < k) return T(0);
    return fact<T>(n) * fact_inv<T>(k) * fact_inv<T>(n - k);
}

}  // namespace gwen