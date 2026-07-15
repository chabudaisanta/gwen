#pragma once

// std
#include <cassert>
#include <utility>
#include <vector>

#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 順列・組合せ (階乗、階乗の逆元、二項係数など) を高速に計算するクラス
 * @details 内部でテーブルを保持し、必要に応じて自動でサイズを倍加拡張（extend）します。
 * クラスのインスタンスごとにテーブルを持つため、法が変化する DynamicModInt64 でも安全に使用できます。
 * 注意: 階乗の前計算として \f$O(N)\f$ のメモリと時間を消費するため、\f$N\f$ が \f$10^8\f$ を超えるような巨大な値での呼び出しには適しません（MLE/TLEになります）。
 * 
 * @tparam T `gwen::modint` コンセプトを満たす型（`ModInt61` や `DynamicModInt64` など）
 */
template <gwen::modint T>
class Combination {
    std::vector<T> _fact, _inv;

public:
    /**
     * @brief 初期サイズ `n` でテーブルを初期化します。
     * @param n 初期状態でのテーブルサイズ。指定しない場合は内部的に最小サイズで初期化されます。
     */
    explicit Combination(i32 n = 0) : _fact({T(1)}), _inv({T(1)}) {
        if (n > 0) extend(n);
    }

    /**
     * @brief テーブルのサイズが `n` 以上になるように自動拡張します。
     * @param n 必要な最小サイズ
     */
    void extend(i32 n) {
        if ((i32)_fact.size() > n) return;
        // n が法以上の場合は階乗が0になり逆元が計算できないため弾く
        assert(std::cmp_less(n, T::mod()) && "n must be strictly less than mod");
        
        i32 old_size = _fact.size();
        i32 new_size = old_size;
        while (new_size <= n) new_size *= 2;
        
        // 倍加によって法を超えてしまうと逆元計算でゼロ除算となるためクリップする
        if (T::mod() <= (u64)2147483647 && std::cmp_greater_equal(new_size, T::mod())) {
            new_size = (i32)T::mod();
        }
        
        _fact.resize(new_size, T(1));
        _inv.resize(new_size, T(1));
        
        for (i32 i = old_size - 1; i < new_size - 1; ++i) {
            _fact[i + 1] = _fact[i] * T(i + 1);
        }
        _inv.back() = _fact.back().inv();
        for (i32 i = new_size - 1; i > old_size; --i) {
            _inv[i - 1] = _inv[i] * T(i);
        }
    }

    /**
     * @brief `n` の階乗 `n!` を返します。
     * @details 必要に応じて内部テーブルを自動拡張するため、非constメソッドとなっています。
     * 計算量: ならし \f$O(1)\f$ (テーブルの自動拡張を除く)
     * @param n 階乗を計算する値
     * @pre `n >= 0`
     * @return `n!`
     */
    T fact(i32 n) {
        assert(n >= 0);
        if ((i32)_fact.size() <= n) extend(n);
        return _fact[n];
    }

    /**
     * @brief `n` の階乗の逆元 `1 / (n!)` を返します。
     * @details 必要に応じて内部テーブルを自動拡張するため、非constメソッドとなっています。
     * 計算量: ならし \f$O(1)\f$ (テーブルの自動拡張を除く)
     * @param n 階乗の逆元を計算する値
     * @pre `n >= 0`
     * @return `1 / (n!)`
     */
    T fact_inv(i32 n) {
        assert(n >= 0);
        if ((i32)_inv.size() <= n) extend(n);
        return _inv[n];
    }

    /**
     * @brief 二項係数 `nCk` (n個からk個を選ぶ組合せの数) を返します。
     * @details 計算量: ならし \f$O(1)\f$ (テーブルの自動拡張を除く)
     * @param n 総数
     * @param k 選ぶ数
     * @return `nCk` (ただし `k < 0` または `n < k` の場合は `0`)
     */
    T comb(i32 n, i32 k) {
        if (k < 0 || n < k) return T(0);
        return fact(n) * fact_inv(k) * fact_inv(n - k);
    }

    /**
     * @brief 順列 `nPk` (n個からk個を選んで並べる順列の数) を返します。
     * @details 計算量: ならし \f$O(1)\f$ (テーブルの自動拡張を除く)
     * @param n 総数
     * @param k 選ぶ数
     * @return `nPk` (ただし `k < 0` または `n < k` の場合は `0`)
     */
    T perm(i32 n, i32 k) {
        if (k < 0 || n < k) return T(0);
        return fact(n) * fact_inv(n - k);
    }
};

} // namespace gwen
