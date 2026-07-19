#pragma once
// https://rsk0315.hatenablog.com/entry/2022/11/27/060616
#include <cassert>
#include <concepts>
#include <type_traits>

#include "gwen/alge/ring.hpp"
#include "gwen/mod/mod.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief modint型としての要件を定義するコンセプト
 * ACLのmodintと互換性のあるインターフェースを持つことを要求する。
 */
template <typename T>
concept modint = ring<T> && requires(T a, T b, u64 n) {
    { a / b } -> std::same_as<T>;
    { a /= b } -> std::same_as<T&>;
    { a.val() } -> std::integral;
    { T::mod() } -> std::integral;
    { a.inv() } -> std::same_as<T>;
    { a.pow(n) } -> std::same_as<T>;
};

/**
 * @brief 実行時に法を設定可能なモジュラ演算クラス (64bit)
 * @details 内部で Montgomery 乗算を用いることで高速化されている。
 */
struct DynamicModInt64 {
private:
    using m64 = DynamicModInt64;
    static inline u64 n = 1;
    static inline u64 ns = 0;
    static inline u64 r2 = 0;
    static inline constexpr u64 msk = -1;

    u64 reduce_mul(u64 a, u64 b) const {
        u128 t = static_cast<u128>(a) * b;
        u128 m = (t * ns) & msk;
        a = (m * n + t) >> 64;
        return a < n ? a : a - n;
    }

    u64 tr;

public:
    /**
     * @brief クラス全体の法を設定する
     * @param n_ 設定する法 (奇数かつ 1 <= n_ < 2^62)
     */
    static void set_mod(u64 n_) {
        assert(n_ < (1ull << 62));
        assert(n_ & 1);
        n = n_;
        ns = n;
        for (i32 i = 0; i < 5; ++i) ns *= 2 - ns * n;
        assert(ns * n == 1);
        ns = -ns;
        r2 = -static_cast<u128>(n) % n;
    }

    /**
     * @brief 現在設定されている法を取得する
     * @return 法
     */
    static u64 mod() { return n; }

    /**
     * @brief デフォルトコンストラクタ (0で初期化)
     */
    DynamicModInt64() : tr(0) {}

    /**
     * @brief 符号なし整数からのコンストラクタ
     */
    template <std::unsigned_integral T> DynamicModInt64(T x) {
        static_assert(sizeof(T) <= sizeof(u64), "T must be 64-bit or smaller");
        tr = reduce_mul(static_cast<u64>(x), r2);
    }

    /**
     * @brief 符号付き整数からのコンストラクタ
     */
    template <std::signed_integral T> DynamicModInt64(T x) : tr(0) {
        static_assert(sizeof(T) <= sizeof(u64), "T must be 64-bit or smaller");
        if (x < 0) {
            sub(m64{static_cast<u64>(-static_cast<i64>(x))});
        }
        else {
            tr = reduce_mul(static_cast<u64>(x), r2);
        }
    }

    /**
     * @brief 現在の値を返す
     */
    u64 val() const { return reduce_mul(tr, 1); }

    // basic operation
    m64& sub(const m64& x) {
        if (tr < x.tr) tr += n;
        tr -= x.tr;
        return *this;
    }
    m64& add(const m64& x) {
        tr += x.tr;
        if (tr >= n) tr -= n;
        return *this;
    }
    m64& mul(const m64& x) {
        tr = reduce_mul(tr, x.tr);
        return *this;
    }

    m64& operator+=(const m64& x) { return add(x); }
    m64& operator-=(const m64& x) { return sub(x); }
    m64& operator*=(const m64& x) { return mul(x); }
    m64& operator/=(const m64& x) { return mul(x.inv()); }

    /**
     * @brief 単項マイナス演算子
     */
    m64 operator-() const { return tr == 0 ? m64() : m64(n - val()); }

    friend m64 operator+(const m64& lhs, const m64& rhs) { return m64(lhs) += rhs; }
    friend m64 operator-(const m64& lhs, const m64& rhs) { return m64(lhs) -= rhs; }
    friend m64 operator*(const m64& lhs, const m64& rhs) { return m64(lhs) *= rhs; }
    friend m64 operator/(const m64& lhs, const m64& rhs) { return m64(lhs) /= rhs; }

    friend bool operator==(const m64& lhs, const m64& rhs) { return lhs.tr == rhs.tr; }
    friend bool operator!=(const m64& lhs, const m64& rhs) { return lhs.tr != rhs.tr; }

    /**
     * @brief 逆元を計算する
     */
    m64 inv() const {
        u64 v = val();
        assert(v != 0 && "DynamicModInt64::inv(): division by zero");
        return m64(inv_mod_64(v, n));
    }

    /**
     * @brief 累乗を計算する
     */
    template <std::integral T> m64 pow(T x) const {
        if constexpr (std::is_signed_v<T>) {
            assert(x >= 0);
        }
        m64 res(1);
        m64 tmp(*this);
        while (x) {
            if (x & 1) res.mul(tmp);
            tmp.tr = reduce_mul(tmp.tr, tmp.tr);
            x >>= 1;
        }
        return res;
    }
};

/**
 * @brief コンパイル時に法が定まるモジュラ演算クラス (64bit)
 * @details 内部で Montgomery 乗算を用いることで高速化されている。
 *          法 Mod は 2^62 未満の奇数である必要がある。
 */
template <u64 Mod>
struct StaticModInt64 {
private:
    using m64 = StaticModInt64;
    static_assert(Mod < (1ull << 62), "Mod must be less than 2^62");
    static_assert(Mod & 1, "Mod must be odd");

    static constexpr u64 n = Mod;
    static constexpr u64 ns = []() {
        u64 inv = Mod;
        for (i32 i = 0; i < 5; ++i) inv *= 2 - inv * Mod;
        return -inv;
    }();
    static constexpr u64 r2 = -static_cast<u128>(Mod) % Mod;
    static constexpr u64 msk = -1;

    constexpr u64 reduce_mul(u64 a, u64 b) const {
        u128 t = static_cast<u128>(a) * b;
        u128 m = (t * ns) & msk;
        a = (m * n + t) >> 64;
        return a < n ? a : a - n;
    }

    u64 tr;

public:
    /**
     * @brief 現在設定されている法を取得する
     * @return 法
     */
    static constexpr u64 mod() { return n; }

    /**
     * @brief デフォルトコンストラクタ (0で初期化)
     */
    constexpr StaticModInt64() : tr(0) {}

    /**
     * @brief 符号なし整数からのコンストラクタ
     */
    template <std::unsigned_integral T> constexpr StaticModInt64(T x) {
        static_assert(sizeof(T) <= sizeof(u64), "T must be 64-bit or smaller");
        tr = reduce_mul(static_cast<u64>(x), r2);
    }

    /**
     * @brief 符号付き整数からのコンストラクタ
     */
    template <std::signed_integral T> constexpr StaticModInt64(T x) : tr(0) {
        static_assert(sizeof(T) <= sizeof(u64), "T must be 64-bit or smaller");
        if (x < 0) {
            sub(m64{static_cast<u64>(-static_cast<i64>(x))});
        } else {
            tr = reduce_mul(static_cast<u64>(x), r2);
        }
    }

    /**
     * @brief 現在の値を返す
     */
    constexpr u64 val() const { return reduce_mul(tr, 1); }

    // basic operation
    constexpr m64& sub(const m64& x) {
        if (tr < x.tr) tr += n;
        tr -= x.tr;
        return *this;
    }
    constexpr m64& add(const m64& x) {
        tr += x.tr;
        if (tr >= n) tr -= n;
        return *this;
    }
    constexpr m64& mul(const m64& x) {
        tr = reduce_mul(tr, x.tr);
        return *this;
    }

    constexpr m64& operator+=(const m64& x) { return add(x); }
    constexpr m64& operator-=(const m64& x) { return sub(x); }
    constexpr m64& operator*=(const m64& x) { return mul(x); }
    constexpr m64& operator/=(const m64& x) { return mul(x.inv()); }

    /**
     * @brief 単項マイナス演算子
     */
    constexpr m64 operator-() const { return tr == 0 ? m64() : m64(n - val()); }

    friend constexpr m64 operator+(const m64& lhs, const m64& rhs) { return m64(lhs) += rhs; }
    friend constexpr m64 operator-(const m64& lhs, const m64& rhs) { return m64(lhs) -= rhs; }
    friend constexpr m64 operator*(const m64& lhs, const m64& rhs) { return m64(lhs) *= rhs; }
    friend constexpr m64 operator/(const m64& lhs, const m64& rhs) { return m64(lhs) /= rhs; }

    friend constexpr bool operator==(const m64& lhs, const m64& rhs) { return lhs.tr == rhs.tr; }
    friend constexpr bool operator!=(const m64& lhs, const m64& rhs) { return lhs.tr != rhs.tr; }

    /**
     * @brief 逆元を計算する
     */
    constexpr m64 inv() const {
        u64 v = val();
        assert(v != 0 && "StaticModInt64::inv(): division by zero");
        return m64(inv_mod_64(v, n));
    }

    /**
     * @brief 累乗を計算する
     */
    template <std::integral T> constexpr m64 pow(T x) const {
        if constexpr (std::is_signed_v<T>) {
            assert(x >= 0);
        }
        m64 res(1);
        m64 tmp(*this);
        while (x) {
            if (x & 1) res.mul(tmp);
            tmp.tr = reduce_mul(tmp.tr, tmp.tr);
            x >>= 1;
        }
        return res;
    }
};

using modint998244353 = StaticModInt64<998244353>;
using modint1000000007 = StaticModInt64<1000000007>;

}  // namespace gwen
