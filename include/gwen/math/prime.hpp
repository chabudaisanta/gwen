#pragma once

#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>

#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief エラトステネスの篩による最小素因数 (LPF) テーブル
 * @details 前計算に \f$O(N \log \log N)\f$、素数判定に \f$O(1)\f$、素因数分解に \f$O(\log x)\f$ かかります。
 * 定数倍が軽く、通常はこちらの利用が推奨されます。
 * マルチテストケースの場合は、各テストケースで再初期化せず、プログラム全体で1回だけインスタンス化してください。
 */
class LpfTable {
    std::vector<i32> p;

public:
    /**
     * @brief `n` 以下の数に対する LPF テーブルを構築します。
     * @param n テーブルの最大値
     */
    LpfTable(i32 n) : p(n + 1, 0) {
        if (n >= 2) {
            for (i64 i = 2; i <= n; i += 2) p[i] = 2;
            for (i64 i = 3; i <= n; i += 2) {
                if (p[i] == 0) {
                    p[i] = i;
                    for (i64 j = i * i; j <= n; j += i * 2) {
                        if (p[j] == 0) p[j] = i;
                    }
                }
            }
        }
    }

    /**
     * @brief 素数判定を行います。
     * @param x 判定する値
     * @pre `0 <= x <= n`
     * @return `x` が素数であれば `true`、そうでなければ `false`
     */
    bool is_prime(i32 x) const {
        assert(x >= 0 && x < (i32)p.size());
        if (x <= 1) return false;
        return p[x] == x;
    }

    /**
     * @brief 素因数分解を行います。
     * @param x 分解する値
     * @pre `1 <= x <= n`
     * @return 昇順にソートされた素因数のリスト
     */
    std::vector<i32> factorize(i32 x) const {
        assert(x > 0 && x < (i32)p.size());
        std::vector<i32> res;
        while (x > 1) {
            res.push_back(p[x]);
            x /= p[x];
        }
        return res;
    }
};

/**
 * @brief 線形篩による最小素因数 (LPF) テーブル
 * @details 前計算に \f$O(N)\f$、素数判定に \f$O(1)\f$、素因数分解に \f$O(\log x)\f$ かかります。
 * 漸近計算量はエラトステネスの篩より優れますが、メモリアクセスの局所性が低いため定数倍は重めです。
 */
class LinearLpfTable {
    std::vector<i32> lpf;
    std::vector<i32> primes;

public:
    /**
     * @brief `n` 以下の数に対する LPF テーブルを構築します。
     * @param n テーブルの最大値
     */
    LinearLpfTable(i32 n) : lpf(n + 1, 0) {
        if (n >= 2) {
            for (i32 i = 2; i <= n; ++i) {
                if (lpf[i] == 0) {
                    lpf[i] = i;
                    primes.push_back(i);
                }
                for (i32 p : primes) {
                    if (p > lpf[i] || (i64)i * p > n) break;
                    lpf[i * p] = p;
                }
            }
        }
    }

    /**
     * @brief 素数判定を行います。
     * @param x 判定する値
     * @pre `0 <= x <= n`
     * @return `x` が素数であれば `true`、そうでなければ `false`
     */
    bool is_prime(i32 x) const {
        assert(x >= 0 && x < (i32)lpf.size());
        if (x <= 1) return false;
        return lpf[x] == x;
    }

    /**
     * @brief 素因数分解を行います。
     * @param x 分解する値
     * @pre `1 <= x <= n`
     * @return 昇順にソートされた素因数のリスト
     */
    std::vector<i32> factorize(i32 x) const {
        assert(x > 0 && x < (i32)lpf.size());
        std::vector<i32> res;
        while (x > 1) {
            res.push_back(lpf[x]);
            x /= lpf[x];
        }
        return res;
    }
};

namespace internal {
inline bool miller_rabin(u64 n, const std::vector<u64>& bases) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    using mint = DynamicModInt64;
    mint::set_mod(n);
    u64 d = n - 1;
    while (d % 2 == 0) d /= 2;
    for (u64 a : bases) {
        if (n <= a) break;
        u64 t = d;
        mint y = mint(a).pow(t);
        while (t != n - 1 && y.val() != 1 && y.val() != n - 1) {
            y *= y;
            t *= 2;
        }
        if (y.val() != n - 1 && t % 2 == 0) return false;
    }
    return true;
}
}  // namespace internal

/**
 * @brief Miller-Rabin法による高速な素数判定
 * @details \f$2^{64}-1\f$ 以下の任意の数について確定的な素数判定を行います。
 * 内部で `DynamicModInt64` の static 変数（法）を書き換えるため、**スレッドセーフではありません**。
 * @param n 判定する値
 * @return `n` が素数であれば `true`、そうでなければ `false`
 */
inline bool is_prime(u64 n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    if (n < 4759123141ull) {
        return internal::miller_rabin(n, {2, 7, 61});
    }
    else {
        return internal::miller_rabin(n, {2, 325, 9375, 28178, 450775, 9780504, 1795265022});
    }
}

namespace internal {
inline u64 pollard_rho(u64 n) {
    if (n % 2 == 0) return 2;
    if (is_prime(n)) return n;

    using mint = DynamicModInt64;
    mint::set_mod(n);

    u64 step = 0;
    while (true) {
        ++step;
        mint c(step);
        auto f = [&](mint x) { return x * x + c; };

        mint x(step), y = f(x);
        while (true) {
            u64 xv = x.val();
            u64 yv = y.val();
            u64 p = std::gcd(yv > xv ? yv - xv : xv - yv, n);
            if (p == 0 || p == n) break;
            if (p != 1) return p;
            x = f(x);
            y = f(f(y));
        }
    }
}
}  // namespace internal

/**
 * @brief Pollard's rho法による高速な素因数分解
 * @details \f$2^{64}-1\f$ 以下の任意の数について \f$O(x^{1/4})\f$ で素因数分解を行います。
 * 内部で `DynamicModInt64` を利用し、また再帰的に `is_prime` を呼び出します（スレッドセーフではありません）。
 * @param n 分解する値
 * @return 昇順にソートされた素因数のリスト
 */
inline std::vector<u64> factorize(u64 n) {
    if (n <= 1) return {};
    std::vector<u64> res;
    auto dfs = [&](auto self, u64 x) -> void {
        if (x <= 1) return;
        if (is_prime(x)) {
            res.push_back(x);
            return;
        }
        u64 p = internal::pollard_rho(x);
        self(self, p);
        self(self, x / p);
    };
    dfs(dfs, n);
    std::sort(res.begin(), res.end());
    return res;
}

}  // namespace gwen
