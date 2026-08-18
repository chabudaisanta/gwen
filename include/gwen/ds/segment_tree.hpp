#pragma once

#include <bit>
#include <cassert>
#include <format>
#include <string>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/dump.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 静的セグメント木
 * @details モノイドを要素に持ち、一点更新と区間積の取得を $O(\log N)$ で行います。
 */
template <monoid M> class SegmentTree {
public:
    using S = typename M::S;

private:
    i32 n;
    i32 size;
    i32 log;
    std::vector<S> d;

    void update(i32 k) { d[k] = M::op(d[2 * k], d[2 * k + 1]); }

public:
    /**
     * @brief デフォルトコンストラクタ。空の木を生成する。
     */
    SegmentTree() : SegmentTree(0) {}

    /**
     * @brief 長さ n_ の配列で初期化する
     * @param n_ 配列の長さ
     */
    explicit SegmentTree(i32 n_) : SegmentTree(std::vector<S>(n_, M::e())) {}

    /**
     * @brief 与えられた配列で初期化する
     * @param v 初期値の配列
     */
    explicit SegmentTree(const std::vector<S>& v) {
        n = i32(v.size());
        size = std::bit_ceil(static_cast<u32>(std::max(1, n)));
        log = std::countr_zero(static_cast<u32>(size));
        d.assign(2 * size, M::e());
        for (i32 i = 0; i < n; i++) d[size + i] = v[i];
        for (i32 i = size - 1; i >= 1; i--) update(i);
    }

    /**
     * @brief 座標 p の値を x に更新する
     * @param p 対象のインデックス (0 <= p < n)
     * @param x 更新後の値
     */
    void set(i32 p, S x) {
        assert(0 <= p && p < n);
        p += size;
        d[p] = x;
        for (i32 i = 1; i <= log; i++) update(p >> i);
    }

    /**
     * @brief 座標 p の値を取得する
     * @param p 対象のインデックス (0 <= p < n)
     * @return S インデックス p の値
     */
    S get(i32 p) const {
        assert(0 <= p && p < n);
        return d[p + size];
    }

    /**
     * @brief 区間 [l, r) の総積を取得する
     * @param l 区間の左端 (0 <= l <= r)
     * @param r 区間の右端 (l <= r <= n)
     * @return S 区間 [l, r) の総積
     */
    S prod(i32 l, i32 r) const {
        assert(0 <= l && l <= r && r <= n);
        S sml = M::e(), smr = M::e();
        l += size;
        r += size;
        while (l < r) {
            if (l & 1) sml = M::op(sml, d[l++]);
            if (r & 1) smr = M::op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return M::op(sml, smr);
    }

    /**
     * @brief 配列全体の総積を取得する
     * @return S 区間 [0, n) の総積
     */
    S all_prod() const { return d[1]; }

    /**
     * @brief g(prod(l, r)) = true となる最大の r を返す
     * @tparam g 判定関数 (S -> bool)
     * @param l 区間の左端 (0 <= l <= n)
     * @return i32 条件を満たす最大の r
     */
    template <bool (*g)(S)> i32 max_right(i32 l) const {
        return max_right(l, [](S x) { return g(x); });
    }

    /**
     * @brief g(prod(l, r)) = true となる最大の r を返す
     * @tparam G 判定関数の型 (S -> bool)
     * @param l 区間の左端
     * @param g 判定関数
     * @return i32 条件を満たす最大の r
     */
    template <class G> i32 max_right(i32 l, G g) const {
        assert(0 <= l && l <= n);
        assert(g(M::e()));
        if (l == n) return n;
        l += size;
        S sm = M::e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(M::op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (g(M::op(sm, d[l]))) {
                        sm = M::op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = M::op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    /**
     * @brief g(prod(l, r)) = true となる最小の l を返す
     * @tparam g 判定関数 (S -> bool)
     * @param r 区間の右端 (0 <= r <= n)
     * @return i32 条件を満たす最小の l
     */
    template <bool (*g)(S)> i32 min_left(i32 r) const {
        return min_left(r, [](S x) { return g(x); });
    }

    /**
     * @brief g(prod(l, r)) = true となる最小の l を返す
     * @tparam G 判定関数の型 (S -> bool)
     * @param r 区間の右端
     * @param g 判定関数
     * @return i32 条件を満たす最小の l
     */
    template <class G> i32 min_left(i32 r, G g) const {
        assert(0 <= r && r <= n);
        assert(g(M::e()));
        if (r == 0) return 0;
        r += size;
        S sm = M::e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(M::op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (g(M::op(d[r], sm))) {
                        sm = M::op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = M::op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

    /**
     * @brief 現在の配列要素を std::vector で返す
     * @return std::vector<S> 現在の配列
     */
    std::vector<S> to_vec() const {
        std::vector<S> res(n);
        for (i32 i = 0; i < n; i++) res[i] = get(i);
        return res;
    }

    /**
     * @brief デバッグ用の文字列表現を返す
     * @return std::string 木の状態の文字列表現
     */
    std::string dump() const {
        return std::format("SegmentTree{{\n  N = {},\n  data(restored) = {}\n}}", n, internal::format_range(to_vec()));
    }
};

}  // namespace gwen
