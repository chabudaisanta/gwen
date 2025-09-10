#pragma once

#include <vector>

#include "gwen/mod/mod.hpp"
#include "gwen/types.hpp"

namespace gwen {

template <i32 M = 998244353> class combination {
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