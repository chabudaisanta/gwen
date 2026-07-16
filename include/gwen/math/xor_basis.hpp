#pragma once

#include <algorithm>
#include <concepts>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief XOR 基底 (Noshi基底)
 * @details 与えられた整数の集合から、排他的論理和 (XOR) で生成される部分空間の基底を管理します。
 * 内部で降順にソートされた状態を保つことで、追加や判定を $O(B)$ (Bは基底のサイズ) で行います。
 *
 * @tparam T 基底の要素の型 (符号なし整数型)
 */
template <std::unsigned_integral T> class XorBasis {
private:
    std::vector<T> basis;

public:
    /**
     * @brief デフォルトコンストラクタ
     */
    XorBasis() = default;

    /**
     * @brief 現在の基底のサイズを返す
     * @return i32 基底のサイズ
     */
    i32 size() const noexcept { return static_cast<i32>(basis.size()); }

    /**
     * @brief 基底が空かどうかを判定する
     * @return true 空である場合
     * @return false 空でない場合
     */
    bool empty() const noexcept { return basis.empty(); }

    /**
     * @brief 要素 x を追加する
     * @details 内部の基底を用いて掃き出しを行い、独立であれば追加します。
     * @param x 追加する要素
     * @return true 独立な要素として基底に追加された場合
     * @return false 既に基底の線形結合で作れる場合
     */
    bool insert(T x) {
        for (T b : basis) {
            x = std::min(x, x ^ b);
        }
        if (x > 0) {
            basis.push_back(x);
            // 降順を維持する (Bは最大でも64なので毎回ソートしても十分高速)
            std::sort(basis.rbegin(), basis.rend());
            return true;
        }
        return false;
    }

    /**
     * @brief 要素 x が現在の基底の XOR で作れるか判定する
     * @param x 判定する要素
     * @return true 作れる場合
     * @return false 作れない場合
     */
    bool can_form(T x) const {
        for (T b : basis) {
            x = std::min(x, x ^ b);
        }
        return x == 0;
    }

    /** @brief モノイドの要素型 (自身) */
    using S = XorBasis;

    /**
     * @brief 2つの XOR 基底をマージする
     * @param a 1つ目の基底
     * @param b 2つ目の基底
     * @return XorBasis マージされた新しい基底
     */
    static S op(S a, S b) {
        S& large = a;
        S& small = b;
        if (large.size() < small.size()) {
            std::swap(large, small);
        }
        for (T x : small.basis) {
            large.insert(x);
        }
        return large;
    }

    /**
     * @brief 空の XOR 基底を返す (モノイドの単位元)
     * @return XorBasis 空の基底
     */
    static S e() { return S(); }
};

}  // namespace gwen
