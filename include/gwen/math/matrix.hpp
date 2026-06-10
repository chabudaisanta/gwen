#pragma once

#include <cassert>
#include <concepts>
#include <span>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

template <typename T>
concept matrix_field = requires(T a, T b, T c) {
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

template <typename T> struct matrix {
private:
    i32 N, M;
    std::vector<T> dat;

    static std::pair<i32, T> gauss(matrix& A, i32 pivot_end, bool diagonalize) {
        assert(0 <= pivot_end && pivot_end <= A.M);
        i32 rank = 0;
        T det = T(1);
        for (i32 j = 0; j < pivot_end; ++j) {
            i32 idx = -1;
            for (i32 i = rank; i < A.N; ++i) {
                if (A.dat[i * A.M + j] != T(0)) {
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
                for (i32 k = 0; k < A.M; ++k) std::swap(A.dat[rank * A.M + k], A.dat[idx * A.M + k]);
            }
            det *= A.dat[rank * A.M + j];
            if (diagonalize && A.dat[rank * A.M + j] != T(1)) {
                const T inv = T(1) / A.dat[rank * A.M + j];
                for (i32 k = j; k < A.M; ++k) A.dat[rank * A.M + k] *= inv;
            }
            const i32 start = diagonalize ? 0 : rank + 1;
            for (i32 i = start; i < A.N; ++i) {
                if (i == rank) continue;
                if (A.dat[i * A.M + j] == T(0)) continue;
                const T coeff = A.dat[i * A.M + j] / A.dat[rank * A.M + j];
                for (i32 k = j; k < A.M; ++k) A.dat[i * A.M + k] -= A.dat[rank * A.M + k] * coeff;
            }
            ++rank;
        }
        return {rank, det};
    }

public:
    using value_type = T;

    matrix() : matrix(0) {}
    matrix(i32 N_) : matrix(N_, N_) {}
    matrix(i32 N_, i32 M_, T val = T{}) : N(N_), M(M_), dat(static_cast<size_t>(N_) * M_, val) {}

    i32 rows() const { return N; }
    i32 cols() const { return M; }
    bool is_square() const { return N == M; }

    std::span<T> operator[](i32 i) { return std::span<T>(&dat[static_cast<size_t>(i) * M], M); }
    std::span<const T> operator[](i32 i) const { return std::span<const T>(&dat[static_cast<size_t>(i) * M], M); }

    matrix operator+(const matrix& other) const { return add(*this, other); }
    matrix operator-(const matrix& other) const { return sub(*this, other); }
    matrix operator*(const matrix& other) const { return mul(*this, other); }
    template <typename U> matrix operator*(const U& scalar) const {
        matrix ret = *this;
        for (T& t : ret.dat) t *= scalar;
        return ret;
    }

    matrix& operator+=(const matrix& other) {
        assert(N == other.N && M == other.M);
        for (i32 i = 0; i < static_cast<i32>(dat.size()); ++i) dat[i] += other.dat[i];
        return *this;
    }
    matrix& operator-=(const matrix& other) {
        assert(N == other.N && M == other.M);
        for (i32 i = 0; i < static_cast<i32>(dat.size()); ++i) dat[i] -= other.dat[i];
        return *this;
    }
    matrix& operator*=(const matrix& other) {
        *this = mul(*this, other);
        return *this;
    }
    template <typename U> matrix& operator*=(const U& scalar) {
        for (T& t : dat) t *= scalar;
        return *this;
    }

    matrix transpose() const {
        matrix ret(M, N);
        for (i32 i = 0; i < N; ++i)
            for (i32 j = 0; j < M; ++j) ret.dat[static_cast<size_t>(j) * N + i] = dat[static_cast<size_t>(i) * M + j];
        return ret;
    }

    T det() const
        requires matrix_field<T>
    {
        assert(is_square());
        matrix B = *this;
        return gauss(B, N, false).second;
    }

    i32 rank() const
        requires matrix_field<T>
    {
        matrix B = *this;
        return gauss(B, M, false).first;
    }

    T trace() const {
        assert(is_square());
        T s = T(0);
        for (i32 i = 0; i < N; ++i) s += dat[static_cast<size_t>(i) * M + i];
        return s;
    }

    matrix inverse() const
        requires matrix_field<T>
    {
        assert(is_square());
        matrix aug(N, 2 * N);
        for (i32 i = 0; i < N; ++i) {
            for (i32 j = 0; j < N; ++j)
                aug.dat[static_cast<size_t>(i) * (2 * N) + j] = dat[static_cast<size_t>(i) * N + j];
            aug.dat[static_cast<size_t>(i) * (2 * N) + N + i] = T(1);
        }
        if (gauss(aug, N, true).first != N) return matrix();
        matrix ret(N);
        for (i32 i = 0; i < N; ++i)
            for (i32 j = 0; j < N; ++j)
                ret.dat[static_cast<size_t>(i) * N + j] = aug.dat[static_cast<size_t>(i) * (2 * N) + N + j];
        return ret;
    }

    static matrix add(const matrix& A, const matrix& B) {
        assert(A.N == B.N && A.M == B.M);
        matrix ret(A.N, A.M);
        for (i32 i = 0; i < static_cast<i32>(ret.dat.size()); ++i) ret.dat[i] = A.dat[i] + B.dat[i];
        return ret;
    }
    static matrix sub(const matrix& A, const matrix& B) {
        assert(A.N == B.N && A.M == B.M);
        matrix ret(A.N, A.M);
        for (i32 i = 0; i < static_cast<i32>(ret.dat.size()); ++i) ret.dat[i] = A.dat[i] - B.dat[i];
        return ret;
    }
    static matrix mul(const matrix& A, const matrix& B) {
        assert(A.M == B.N);
        matrix ret(A.N, B.M);
        for (i32 i = 0; i < A.N; ++i) {
            const i32 row = i * B.M;
            for (i32 k = 0; k < A.M; ++k) {
                const T aik = A.dat[static_cast<size_t>(i) * A.M + k];
                const i32 krow = k * B.M;
                for (i32 j = 0; j < B.M; ++j)
                    ret.dat[static_cast<size_t>(row) + j] += aik * B.dat[static_cast<size_t>(krow) + j];
            }
        }
        return ret;
    }
    static matrix eye(i32 N_) {
        assert(0 <= N_);
        matrix ret(N_);
        for (i32 i = 0; i < N_; ++i) ret[i][i] = T{1};
        return ret;
    }
    template <std::integral U> static matrix pow(matrix A, U n) {
        assert(A.is_square());
        matrix ret = eye(A.N);
        while (n) {
            if (n & 1) ret = ret * A;
            A = A * A;
            n >>= 1;
        }
        return ret;
    }
};

}  // namespace gwen
