#pragma once

#include <vector>

#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

namespace gwen {

namespace internal {

struct prime_factorize_table {
    i32 n;
    // p[i] = 0     if i < 2
    // p[i] = lpf   if 2 <= i, especially p[i] = i if i is prime
    std::vector<i32> p;

    prime_factorize_table() : n(2), p(2, 0) {}

    void extend() {
        assert(n <= 1e8);
        n <<= 1;
        p.resize(n, 0);
        for (i64 i = 2; i < n; ++i) {
            if (!p[i]) {
                p[i] = i;
                for (i64 j = i * i; j < n; j += i) {
                    if (!p[j]) p[j] = i;
                }
            }
        }
    }

    // if x <= max() can factorize
    // else cannot factorize
    i32 max() const { return n - 1; }
};

}  // namespace internal

std::vector<i32> factorize(i32 x) {
    assert(0 <= x && x <= 1e8);
    if (x <= 1) return {};
    static gwen::internal::prime_factorize_table table;
    while (table.max() < x) table.extend();
    std::vector<i32> ret;
    while (x > 1) {
        // table.p[x] = lpf
        // if table.p[x] = 0, x = 1
        ret.emplace_back(table.p[x]);
        x /= table.p[x];
    }
    return ret;
}

bool miller32(u32 n) {
    assert(n < 4759123141u);
    static u64 a[3] = {2, 7, 61};
    using mint = dynamic_modint64;
    mint::set_mod(n);
    u64 s = 0, d = n - 1;
    while (!(d & 1)) {
        ++s;
        d >>= 1;
    }
    for (int i = 0; i < 3; ++i) {
        if (n <= a[i]) return true;
        mint x{a[i]};
        x = x.pow(d);
        if (x.val() != 1u) {
            u64 t;
            for (t = 0; t < s; ++t) {
                if (x.val() == n - 1) break;
                x *= x;
            }
            if (t == s) return false;
        }
    }
    return true;
}

bool miller64(u64 n) {
    static u64 a[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    using mint = dynamic_modint64;
    mint::set_mod(n);
    u64 s = 0, d = n - 1;
    while (!(d & 1)) {
        ++s;
        d >>= 1;
    }
    for (int i = 0; i < 7; ++i) {
        if (n <= a[i]) return true;
        mint x{a[i]};
        x = x.pow(d);
        u64 t;
        if (x.val() != 1u) {
            for (t = 0; t < s; ++t) {
                if (x.val() == n - 1) break;
                x *= x;
            }
            if (t == s) return false;
        }
    }
    return true;
}

bool is_prime_small(i32 n) {
    assert(n <= 1e8);
    if (n <= 1) return false;
    static gwen::internal::prime_factorize_table table;
    while (table.max() < n) table.extend();
    return table.p[n] == n;
}

bool miller(u64 n) {
    if (n <= 1)
        return false;
    else if (n == 2)
        return true;
    else if (!(n & 1))
        return false;
    else if (n < (1u << 23))
        return is_prime_small(n);
    else if (n < 4759123141u)
        return miller32(n);
    else
        return miller64(n);
}

}  // namespace gwen