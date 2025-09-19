#pragma once

#include <vector>

#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

namespace gwen {

namespace internal {

struct prime_factorize_table {
    i32 n = 2;
    std::vector<i32> p{0, 0};  // p[i] = 0: i is prime (or 1/0)

    prime_factorize_table() {}

    void extend() {
        assert(n < (1 << 30));
        n *= 2;
        p.resize(n, 0);
        for (int i = 2; i < n; ++i)
            if (!p[i]) {
                for (i64 j = u64(i) * i; j < n; j += i) {
                    p[j] = i;
                }
            }
    }

    // if x <= max() can factorize
    // else cannot factorize
    i32 max() const { return n - 1; }
};

}  // namespace internal

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
    static gwen::internal::prime_factorize_table table;
    while (table.max() < n) table.extend();
    return !table.p[n];
}

bool is_prime(u64 n) {
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

std::vector<i32> factorize(i32 x) {
    assert(0 < x && x < (1 << 30));
    static gwen::internal::prime_factorize_table table;
    while (table.max() < x) table.extend();
    std::vector<i32> ret;
    while (table.p[x] != 0) {
        // if p[x] = 0, x is prime
        ret.emplace_back(table.p[x]);
        x /= table.p[x];
    }
    if (x != 1) ret.emplace_back(x);
    return ret;
}

}  // namespace gwen