#pragma once

// std
#include <algorithm>
#include <cassert>
#include <vector>

#include "gwen/mod/mod61.hpp"
#include "gwen/types.hpp"
#include "gwen/utils/xorshift.hpp"

namespace gwen {
namespace rhash {

/**
 * @brief セグメント木等に乗せるためのローリングハッシュ用モノイド
 * @details `ID` ごとに独立した基数 `r` を静的に生成して持ちます。
 *
 * @tparam ID 異なる基数を持たせるための識別子（デフォルトは0）
 */
template <i32 ID = 0> struct rolling_hash_monoid {
    /// @brief 基数 (法 $2^{61}-1$ 上での乱数)
    static inline const ModInt61 r = ModInt61(rand64() % (ModInt61::mod() - 2) + 2);

    /**
     * @brief ハッシュ値とその長さ（基数のべき乗）の組を表す構造体
     */
    struct S {
        ModInt61 v, p;
        friend bool operator==(const S& a, const S& b) = default;
        friend bool operator!=(const S& a, const S& b) = default;
    };

    /**
     * @brief 2つのハッシュ値を結合します。
     * @details 左側のハッシュ値 `a` に、右側のハッシュ値 `b` を連結します。
     * 計算量: \f$O(1)\f$
     * @param a 左側のハッシュ
     * @param b 右側のハッシュ
     * @return 結合されたハッシュ
     */
    static S op(S a, S b) {
        ModInt61 v = b.v * a.p + a.v;
        ModInt61 p = a.p * b.p;
        return {v, p};
    }

    /**
     * @brief 単位元（空文字列のハッシュ）を返します。
     * @details 計算量: \f$O(1)\f$
     * @return 単位元
     */
    static S e() { return {ModInt61(0), ModInt61(1)}; }

    /**
     * @brief 1文字からハッシュ（長さ1の要素）を生成します。
     * @details 計算量: \f$O(1)\f$
     * @tparam T 文字や整数の型
     * @param x 要素
     * @return 生成されたハッシュ要素
     */
    template <typename T> static S unit(T x) { return {ModInt61(x), r}; }

    /**
     * @brief イテレータ範囲からハッシュ値を計算します。
     * @details 計算量: \f$O(N)\f$ (範囲の長さに比例)
     * @tparam Iterator イテレータの型
     * @param begin 範囲の開始
     * @param end 範囲の終端
     * @return 範囲全体のハッシュ
     */
    template <typename Iterator> static S range(Iterator begin, Iterator end) {
        ModInt61 v = 0, p = 1;
        for (auto it = begin; it != end; ++it) {
            v += p * ModInt61(*it);
            p *= r;
        }
        return {v, p};
    }

    /**
     * @brief イテレータ範囲をセグメント木用の要素配列（`std::vector<S>`）に変換します。
     * @details 計算量: \f$O(N)\f$
     * @tparam Iterator イテレータの型
     * @param begin 範囲の開始
     * @param end 範囲の終端
     * @return セグメント木構築用の配列
     */
    template <typename Iterator> static std::vector<S> build(Iterator begin, Iterator end) {
        std::vector<S> res;
        for (auto it = begin; it != end; ++it) {
            res.push_back(unit(*it));
        }
        return res;
    }

    /**
     * @brief シーケンスをセグメント木用の要素配列（`std::vector<S>`）に変換します。
     * @details 計算量: \f$O(N)\f$
     * @tparam Container シーケンスの型
     * @param seq 変換する対象のシーケンス
     * @return セグメント木構築用の配列
     */
    template <typename Container> static std::vector<S> build(const Container& seq) {
        std::vector<S> res;
        if constexpr (requires { std::size(seq); }) {
            res.reserve(std::size(seq));
        }
        for (const auto& x : seq) {
            res.push_back(unit(x));
        }
        return res;
    }
};

/**
 * @brief 基数のべき乗をメモ化する内部クラス
 * @tparam ID モノイドと共通の識別子
 */
template <i32 ID> struct PowerTable {
    static std::vector<ModInt61>& data() {
        static std::vector<ModInt61> table{ModInt61(1)};
        return table;
    }

    static void ensure(i32 n) {
        auto& t = data();
        const ModInt61 base = rolling_hash_monoid<ID>::r;
        while (static_cast<i32>(t.size()) <= n) {
            t.push_back(t.back() * base);
        }
    }

    static ModInt61 pow(i32 len) {
        assert(0 <= len && len < static_cast<i32>(data().size()));
        return data()[static_cast<usize>(len)];
    }
};

}  // namespace rhash

/**
 * @brief 静的文字列用のローリングハッシュクラス
 * @details 構築に \f$O(N)\f$ かかりますが、以降は任意の部分文字列のハッシュを \f$O(1)\f$ で取得できます。
 *
 * @tparam ID 異なる基数を持たせるための識別子（デフォルトは0）
 */
template <i32 ID = 0> class RollingHash {
public:
    using Monoid = rhash::rolling_hash_monoid<ID>;
    using S = typename Monoid::S;

private:
    i32 n;
    std::vector<ModInt61> suf;

public:
    /**
     * @brief シーケンスからローリングハッシュを構築します。
     * @details 計算量: \f$O(N)\f$
     * @tparam Container シーケンスの型 (std::string や std::vector など)
     * @param seq ハッシュ化する対象のシーケンス
     */
    template <typename Container>
    explicit RollingHash(const Container& seq) : n(static_cast<i32>(std::size(seq))), suf(n + 1, ModInt61(0)) {
        rhash::PowerTable<ID>::ensure(n);
        for (i32 i = n - 1; i >= 0; --i) {
            suf[i] = ModInt61(seq[i]) + Monoid::r * suf[i + 1];
        }
    }

    /**
     * @brief 文字列の長さを返します。
     * @details 計算量: \f$O(1)\f$
     * @return 文字列の長さ
     */
    i32 size() const { return n; }

    /**
     * @brief 部分区間 `[l, r)` のハッシュ値を取得します。
     * @details 先頭の要素が基数の $0$
     * 乗に対応するように正規化されたハッシュと、その長さ（基数のべき乗）の組を返します。 計算量: \f$O(1)\f$
     * @param l 区間の開始インデックス (包含)
     * @param r 区間の終了インデックス (排他)
     * @pre `0 <= l <= r <= n`
     * @return 部分区間のハッシュ値
     */
    S get(i32 l, i32 r) const {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return Monoid::e();
        const i32 len = r - l;
        const ModInt61 rp = rhash::PowerTable<ID>::pow(len);
        ModInt61 v = suf[l] - suf[r] * rp;
        return {v, rp};
    }

    /**
     * @brief 区間 `[l, r)` を左に `k` 文字巡回シフトしたハッシュを取得します。
     * @details 計算量: \f$O(1)\f$
     * @param l 区間の開始インデックス
     * @param r 区間の終了インデックス
     * @param k シフトする文字数 (負の場合は右シフト)
     * @pre `0 <= l <= r <= n`
     * @return 巡回シフト後のハッシュ値
     */
    S rotl(i32 l, i32 r, i32 k) const {
        assert(0 <= l && l <= r && r <= n);
        const i32 len = r - l;
        if (len == 0) return Monoid::e();
        k %= len;
        if (k < 0) k += len;
        if (k == 0) return get(l, r);
        return Monoid::op(get(l + k, r), get(l, l + k));
    }

    /**
     * @brief 区間 `[l, r)` を右に `k` 文字巡回シフトしたハッシュを取得します。
     * @details 計算量: \f$O(1)\f$
     * @param l 区間の開始インデックス
     * @param r 区間の終了インデックス
     * @param k シフトする文字数 (負の場合は左シフト)
     * @pre `0 <= l <= r <= n`
     * @return 巡回シフト後のハッシュ値
     */
    S rotr(i32 l, i32 r, i32 k) const {
        assert(0 <= l && l <= r && r <= n);
        const i32 len = r - l;
        if (len == 0) return Monoid::e();
        k %= len;
        if (k < 0) k += len;
        if (k == 0) return get(l, r);
        return Monoid::op(get(r - k, r), get(l, r - k));
    }

    /**
     * @brief 2つの部分文字列が一致するか判定します。
     * @details 計算量: \f$O(1)\f$
     * @param l1 1つ目の部分文字列の開始
     * @param r1 1つ目の部分文字列の終了
     * @param l2 2つ目の部分文字列の開始
     * @param r2 2つ目の部分文字列の終了
     * @return 一致すれば `true`
     */
    bool equal(i32 l1, i32 r1, i32 l2, i32 r2) const {
        assert(0 <= l1 && l1 <= r1 && r1 <= n);
        assert(0 <= l2 && l2 <= r2 && r2 <= n);
        if (r1 - l1 != r2 - l2) return false;
        if (l1 == r1) return true;
        return get(l1, r1) == get(l2, r2);
    }

    /**
     * @brief 接尾辞 `[l1, n)` と `[l2, n)` の最長共通接頭辞 (LCP) の長さを求めます。
     * @details 二分探索を用いて計算します。
     * 計算量: \f$O(\log N)\f$
     * @param l1 1つ目の開始インデックス
     * @param l2 2つ目の開始インデックス
     * @return LCP の長さ
     */
    i32 lcp(i32 l1, i32 l2) const {
        assert(0 <= l1 && l1 <= n);
        assert(0 <= l2 && l2 <= n);
        i32 lo = 0, hi = std::min(n - l1, n - l2) + 1;
        while (hi - lo > 1) {
            i32 mid = (lo + hi) / 2;
            if (equal(l1, l1 + mid, l2, l2 + mid))
                lo = mid;
            else
                hi = mid;
        }
        return lo;
    }
};

}  // namespace gwen
