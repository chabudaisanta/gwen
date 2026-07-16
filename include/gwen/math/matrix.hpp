#pragma once

#include <cassert>
#include <concepts>
#include <span>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 行列の要素として必要な演算要件を定義するコンセプト
 */
template <typename T>
concept matrix_field = requires(T a, T b) {
    { T(0) } -> std::same_as<T>;
    { T(1) } -> std::same_as<T>;
    { -a } -> std::convertible_to<T>;
    { a + b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a * b } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<T>;
    { a == b } -> std::convertible_to<bool>;
    { a != T(0) } -> std::convertible_to<bool>;
};

/**
 * @brief 行列 (Matrix)
 * @details 任意の型 `T` の要素を持つ行列クラスです。行列の加減乗除や行列式、階数、逆行列の計算をサポートします。
 *
 * @tparam T 行列の要素の型
 */
template <typename T>
class Matrix {
private:
    i32 n, m;
    std::vector<T> dat;

    /**
     * @brief 掃き出し法（ガウスの消去法）
     * @param a 対象の行列
     * @param pivot_end ピボット探索の終了列
     * @param diagonalize 対角化するかどうか
     * @return std::pair<i32, T> 階数(rank)と行列式の値
     */
    static std::pair<i32, T> gauss(Matrix& a, i32 pivot_end, bool diagonalize) {
        assert(0 <= pivot_end && pivot_end <= a.m);
        i32 rank = 0;
        T det = T(1);
        for (i32 j = 0; j < pivot_end; ++j) {
            i32 idx = -1;
            for (i32 i = rank; i < a.n; ++i) {
                if (a.dat[i * a.m + j] != T(0)) {
                    idx = i;
                    break;
                }
            }
            if (idx == -1) {
                det = T(0);
                continue;
            }
            if (rank != idx) {
                det = -det;
                for (i32 k = 0; k < a.m; ++k) {
                    std::swap(a.dat[rank * a.m + k], a.dat[idx * a.m + k]);
                }
            }
            det *= a.dat[rank * a.m + j];
            if (diagonalize && a.dat[rank * a.m + j] != T(1)) {
                const T inv = T(1) / a.dat[rank * a.m + j];
                for (i32 k = j; k < a.m; ++k) {
                    a.dat[rank * a.m + k] *= inv;
                }
            }
            const i32 start = diagonalize ? 0 : rank + 1;
            for (i32 i = start; i < a.n; ++i) {
                if (i == rank) continue;
                if (a.dat[i * a.m + j] == T(0)) continue;
                const T coeff = a.dat[i * a.m + j] / a.dat[rank * a.m + j];
                for (i32 k = j; k < a.m; ++k) {
                    a.dat[i * a.m + k] -= a.dat[rank * a.m + k] * coeff;
                }
            }
            ++rank;
        }
        return {rank, det};
    }

public:
    using value_type = T;

    /**
     * @brief デフォルトコンストラクタ
     */
    Matrix() : Matrix(0) {}

    /**
     * @brief 正方行列を作成するコンストラクタ
     * @param n_ 行数および列数
     */
    explicit Matrix(i32 n_) : Matrix(n_, n_) {}

    /**
     * @brief 任意のサイズの行列を作成するコンストラクタ
     * @param n_ 行数
     * @param m_ 列数
     * @param val 初期値
     */
    Matrix(i32 n_, i32 m_, T val = T{}) : n(n_), m(m_), dat(static_cast<size_t>(n_) * m_, val) {}

    /**
     * @brief 行数を取得する
     */
    i32 rows() const { return n; }

    /**
     * @brief 列数を取得する
     */
    i32 cols() const { return m; }

    /**
     * @brief 正方行列かどうかを判定する
     */
    bool is_square() const { return n == m; }

    /**
     * @brief 指定した行へのアクセスを提供する
     * @param i 行インデックス (0-indexed)
     */
    std::span<T> operator[](i32 i) { return std::span<T>(&dat[static_cast<size_t>(i) * m], m); }

    /**
     * @brief 指定した行への読み取り専用アクセスを提供する
     * @param i 行インデックス (0-indexed)
     */
    std::span<const T> operator[](i32 i) const { return std::span<const T>(&dat[static_cast<size_t>(i) * m], m); }

    /** @brief 行列の加算 */
    Matrix operator+(const Matrix& other) const { return add(*this, other); }
    /** @brief 行列の減算 */
    Matrix operator-(const Matrix& other) const { return sub(*this, other); }
    /** @brief 行列の乗算 */
    Matrix operator*(const Matrix& other) const { return mul(*this, other); }
    /** @brief 行列のスカラー倍 */
    template <typename U> Matrix operator*(const U& scalar) const {
        Matrix ret = *this;
        for (T& t : ret.dat) t *= scalar;
        return ret;
    }

    /** @brief 行列の加算代入 */
    Matrix& operator+=(const Matrix& other) {
        assert(n == other.n && m == other.m);
        for (i32 i = 0; i < static_cast<i32>(dat.size()); ++i) dat[i] += other.dat[i];
        return *this;
    }
    /** @brief 行列の減算代入 */
    Matrix& operator-=(const Matrix& other) {
        assert(n == other.n && m == other.m);
        for (i32 i = 0; i < static_cast<i32>(dat.size()); ++i) dat[i] -= other.dat[i];
        return *this;
    }
    /** @brief 行列の乗算代入 */
    Matrix& operator*=(const Matrix& other) {
        *this = mul(*this, other);
        return *this;
    }
    /** @brief 行列のスカラー倍代入 */
    template <typename U> Matrix& operator*=(const U& scalar) {
        for (T& t : dat) t *= scalar;
        return *this;
    }

    /**
     * @brief 転置行列を取得する
     */
    Matrix transpose() const {
        Matrix ret(m, n);
        for (i32 i = 0; i < n; ++i) {
            for (i32 j = 0; j < m; ++j) {
                ret.dat[static_cast<size_t>(j) * n + i] = dat[static_cast<size_t>(i) * m + j];
            }
        }
        return ret;
    }

    /**
     * @brief 行列式を取得する
     */
    T det() const
        requires matrix_field<T>
    {
        assert(is_square());
        Matrix b = *this;
        return gauss(b, n, false).second;
    }

    /**
     * @brief 階数 (rank) を取得する
     */
    i32 rank() const
        requires matrix_field<T>
    {
        Matrix b = *this;
        return gauss(b, m, false).first;
    }

    /**
     * @brief トレース (対角成分の和) を取得する
     */
    T trace() const {
        assert(is_square());
        T s = T(0);
        for (i32 i = 0; i < n; ++i) s += dat[static_cast<size_t>(i) * m + i];
        return s;
    }

    /**
     * @brief 逆行列を取得する
     * @return Matrix 逆行列が存在しない場合は空の行列を返す
     */
    Matrix inverse() const
        requires matrix_field<T>
    {
        assert(is_square());
        Matrix aug(n, 2 * n);
        for (i32 i = 0; i < n; ++i) {
            for (i32 j = 0; j < n; ++j) {
                aug.dat[static_cast<size_t>(i) * (2 * n) + j] = dat[static_cast<size_t>(i) * n + j];
            }
            aug.dat[static_cast<size_t>(i) * (2 * n) + n + i] = T(1);
        }
        if (gauss(aug, n, true).first != n) return Matrix();
        Matrix ret(n);
        for (i32 i = 0; i < n; ++i) {
            for (i32 j = 0; j < n; ++j) {
                ret.dat[static_cast<size_t>(i) * n + j] = aug.dat[static_cast<size_t>(i) * (2 * n) + n + j];
            }
        }
        return ret;
    }

    /**
     * @brief 2つの行列を加算する
     * @param a 左辺の行列
     * @param b 右辺の行列
     * @return Matrix 加算結果
     */
    static Matrix add(const Matrix& a, const Matrix& b) {
        assert(a.n == b.n && a.m == b.m);
        Matrix ret(a.n, a.m);
        for (i32 i = 0; i < static_cast<i32>(ret.dat.size()); ++i) ret.dat[i] = a.dat[i] + b.dat[i];
        return ret;
    }

    /**
     * @brief 2つの行列を減算する
     * @param a 左辺の行列
     * @param b 右辺の行列
     * @return Matrix 減算結果
     */
    static Matrix sub(const Matrix& a, const Matrix& b) {
        assert(a.n == b.n && a.m == b.m);
        Matrix ret(a.n, a.m);
        for (i32 i = 0; i < static_cast<i32>(ret.dat.size()); ++i) ret.dat[i] = a.dat[i] - b.dat[i];
        return ret;
    }

    /**
     * @brief 2つの行列を乗算する
     * @param a 左辺の行列
     * @param b 右辺の行列
     * @return Matrix 乗算結果
     */
    static Matrix mul(const Matrix& a, const Matrix& b) {
        assert(a.m == b.n);
        Matrix ret(a.n, b.m);
        for (i32 i = 0; i < a.n; ++i) {
            const i32 row = i * b.m;
            for (i32 k = 0; k < a.m; ++k) {
                const T aik = a.dat[static_cast<size_t>(i) * a.m + k];
                const i32 krow = k * b.m;
                for (i32 j = 0; j < b.m; ++j) {
                    ret.dat[static_cast<size_t>(row) + j] += aik * b.dat[static_cast<size_t>(krow) + j];
                }
            }
        }
        return ret;
    }

    /**
     * @brief 単位行列を取得する
     * @param n_ 行列のサイズ
     */
    static Matrix eye(i32 n_) {
        assert(0 <= n_);
        Matrix ret(n_);
        for (i32 i = 0; i < n_; ++i) ret[i][i] = T{1};
        return ret;
    }

    /**
     * @brief 行列の累乗を計算する
     * @param a 底となる正方行列
     * @param k 指数
     */
    template <std::integral U> static Matrix pow(Matrix a, U k) {
        assert(a.is_square());
        Matrix ret = eye(a.n);
        while (k) {
            if (k & 1) ret = ret * a;
            a = a * a;
            k >>= 1;
        }
        return ret;
    }
};

}  // namespace gwen
