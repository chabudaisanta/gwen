#pragma once

#include <cassert>
#include <concepts>
#include <type_traits>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 2^61 - 1 を法とするモジュラ演算クラス
 * @details 主にローリングハッシュなどで利用される、高速なモジュラ演算を提供するクラスです。
 */
struct ModInt61 {
private:
    using m61 = ModInt61;
    static constexpr u64 mod61 = (1ull << 61) - 1;
    static constexpr u64 msk61 = (1ull << 61) - 1;

    u64 tr;

    static constexpr u64 calc_mod(u64 x) {
        u64 res = (x >> 61) + (x & msk61);
        if (res >= mod61) res -= mod61;
        return res;
    }

    static constexpr u64 mul_mod(u64 a, u64 b) {
        u128 t = static_cast<u128>(a) * b;
        u64 res = static_cast<u64>(t >> 61) + static_cast<u64>(t & msk61);
        if (res >= mod61) res -= mod61;
        return res;
    }

public:
    /**
     * @brief 法を取得する
     * @return 法 (2^61 - 1)
     */
    static constexpr u64 mod() { return mod61; }

    /**
     * @brief デフォルトコンストラクタ (0で初期化)
     */
    constexpr ModInt61() : tr(0) {}

    /**
     * @brief 符号なし整数からのコンストラクタ
     */
    template <std::unsigned_integral T>
    constexpr ModInt61(T x) {
        static_assert(sizeof(T) <= sizeof(u64), "T must be 64-bit or smaller");
        tr = calc_mod(static_cast<u64>(x));
    }

    /**
     * @brief 符号付き整数からのコンストラクタ
     */
    template <std::signed_integral T>
    constexpr ModInt61(T x) {
        static_assert(sizeof(T) <= sizeof(u64), "T must be 64-bit or smaller");
        if (x < 0) {
            i64 v = x % static_cast<i64>(mod61);
            if (v < 0) v += mod61;
            tr = v;
        } else {
            tr = calc_mod(static_cast<u64>(x));
        }
    }

    /**
     * @brief 現在の値を返す
     * @return 0以上 2^61-2 以下の整数
     */
    constexpr u64 val() const { return tr; }

    constexpr m61& operator+=(const m61& x) {
        tr += x.tr;
        if (tr >= mod61) tr -= mod61;
        return *this;
    }
    constexpr m61& operator-=(const m61& x) {
        if (tr < x.tr) tr += mod61;
        tr -= x.tr;
        return *this;
    }
    constexpr m61& operator*=(const m61& x) {
        tr = mul_mod(tr, x.tr);
        return *this;
    }
    constexpr m61& operator/=(const m61& x) { return *this *= x.inv(); }

    /**
     * @brief 単項マイナス演算子
     */
    constexpr m61 operator-() const { return tr == 0 ? m61() : m61(mod61 - tr); }

    friend constexpr m61 operator+(const m61& lhs, const m61& rhs) { return m61(lhs) += rhs; }
    friend constexpr m61 operator-(const m61& lhs, const m61& rhs) { return m61(lhs) -= rhs; }
    friend constexpr m61 operator*(const m61& lhs, const m61& rhs) { return m61(lhs) *= rhs; }
    friend constexpr m61 operator/(const m61& lhs, const m61& rhs) { return m61(lhs) /= rhs; }

    friend constexpr bool operator==(const m61& lhs, const m61& rhs) { return lhs.tr == rhs.tr; }
    friend constexpr bool operator!=(const m61& lhs, const m61& rhs) { return lhs.tr != rhs.tr; }

    /**
     * @brief 累乗を計算する
     * @param x 指数
     * @return this^x
     */
    template <std::integral T>
    constexpr m61 pow(T x) const {
        if constexpr (std::is_signed_v<T>) {
            assert(x >= 0);
        }
        m61 res(1);
        m61 tmp(*this);
        while (x) {
            if (x & 1) res *= tmp;
            tmp *= tmp;
            x >>= 1;
        }
        return res;
    }

    /**
     * @brief 逆元を計算する
     * @return this^(-1)
     */
    constexpr m61 inv() const {
        assert(tr != 0 && "ModInt61::inv(): division by zero");
        return pow(mod61 - 2);
    }
};

} // namespace gwen
