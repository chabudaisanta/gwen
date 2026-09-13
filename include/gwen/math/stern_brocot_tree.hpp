#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <numeric>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief Stern-Brocot 木上の正の既約分数を扱う
 * @tparam T bool 以外の整数型
 *
 * 分子・分母が正で互いに素な分数だけを受け付ける。
 */
template <std::integral T>
    requires(!std::same_as<std::remove_cv_t<T>, bool>)
class SternBrocotTree {
  public:
    /** @brief インスタンス化を禁止する */
    SternBrocotTree() = delete;

    /**
     * @brief 根から p / q への run-length 圧縮パスを返す
     * @param p 分子
     * @param q 分母
     * @return `L` または `R` と正の run 長の列
     * @pre p, q は正で互いに素
     * @complexity O(log(max(p, q)))
     */
    static std::vector<std::pair<char, T>> encode_path(T p, T q) {
        assert(p > 0 && q > 0);
        assert(std::gcd(p, q) == 1);

        std::vector<std::pair<char, T>> path;
        while (p != q) {
            if (p < q) {
                const T count = (q - 1) / p;
                path.emplace_back('L', count);
                q -= count * p;
            } else {
                const T count = (p - 1) / q;
                path.emplace_back('R', count);
                p -= count * q;
            }
        }
        return path;
    }

    /**
     * @brief run-length 圧縮パスの表す分数を返す
     * @param path `L` または `R` と正の run 長の列
     * @return 既約分数 {分子, 分母}
     * @pre path の各方向は `L` または `R`、run 長は正
     * @complexity O(K) (K = path.size())
     */
    static std::pair<T, T> decode_path(std::span<const std::pair<char, T>> path) {
        T left_numerator = 0;
        T left_denominator = 1;
        T numerator = 1;
        T denominator = 1;
        T right_numerator = 1;
        T right_denominator = 0;
        for (const auto& [direction, count] : path) {
            assert((direction == 'L' || direction == 'R') && count > 0);
            if (direction == 'L') {
                right_numerator = numerator + (count - 1) * left_numerator;
                right_denominator = denominator + (count - 1) * left_denominator;
                numerator += count * left_numerator;
                denominator += count * left_denominator;
            } else {
                left_numerator = numerator + (count - 1) * right_numerator;
                left_denominator = denominator + (count - 1) * right_denominator;
                numerator += count * right_numerator;
                denominator += count * right_denominator;
            }
        }
        return {numerator, denominator};
    }

    /**
     * @brief 二つの分数の最小共通祖先を返す
     * @param p 一つ目の分子
     * @param q 一つ目の分母
     * @param r 二つ目の分子
     * @param s 二つ目の分母
     * @return 最小共通祖先の {分子, 分母}
     * @pre p, q, r, s は正で、各分数は既約
     * @complexity O(log(max(p, q, r, s)))
     */
    static std::pair<T, T> lca(T p, T q, T r, T s) {
        const auto first = encode_path(p, q);
        const auto second = encode_path(r, s);
        std::vector<std::pair<char, T>> common;
        for (usize i = 0, j = 0; i < first.size() && j < second.size(); ++i, ++j) {
            const auto [first_direction, first_count] = first[i];
            const auto [second_direction, second_count] = second[j];
            if (first_direction != second_direction) break;
            common.emplace_back(first_direction, std::min(first_count, second_count));
            if (first_count != second_count) break;
        }
        return decode_path(common);
    }

    /**
     * @brief 深さ k の祖先を返す
     * @param k 根を深さ 0 とした祖先の深さ
     * @param p 分子
     * @param q 分母
     * @return 祖先の {分子, 分母}。存在しない場合は {0, 0}
     * @pre k は非負、p, q は正で互いに素
     * @complexity O(log(max(p, q)))
     */
    static std::pair<T, T> ancestor(T k, T p, T q) {
        if constexpr (std::signed_integral<T>) assert(k >= 0);
        auto path = encode_path(p, q);
        std::vector<std::pair<char, T>> result;
        for (const auto& [direction, count] : path) {
            const T used = std::min(k, count);
            if (used > 0) result.emplace_back(direction, used);
            k -= used;
            if (k == 0) return decode_path(result);
        }
        return {0, 0};
    }

    /**
     * @brief 分数を挟む Stern-Brocot 区間の左右端を返す
     * @param p 分子
     * @param q 分母
     * @return {左の分子, 左の分母, 右の分子, 右の分母}
     * @pre p, q は正で互いに素
     * @details 左右端 a / b, c / d は a / b < p / q < c / d を満たす。0 / 1, 1 / 0 を含み得る。
     * @complexity O(log(max(p, q)))
     */
    static std::tuple<T, T, T, T> range(T p, T q) {
        const auto path = encode_path(p, q);
        T left_numerator = 0;
        T left_denominator = 1;
        T numerator = 1;
        T denominator = 1;
        T right_numerator = 1;
        T right_denominator = 0;
        for (const auto& [direction, count] : path) {
            if (direction == 'L') {
                right_numerator = numerator + (count - 1) * left_numerator;
                right_denominator = denominator + (count - 1) * left_denominator;
                numerator += count * left_numerator;
                denominator += count * left_denominator;
            } else {
                left_numerator = numerator + (count - 1) * right_numerator;
                left_denominator = denominator + (count - 1) * right_denominator;
                numerator += count * right_numerator;
                denominator += count * right_denominator;
            }
        }
        return {left_numerator, left_denominator, right_numerator, right_denominator};
    }
};

}  // namespace gwen
