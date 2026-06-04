#pragma once

#include <cassert>
#include <concepts>
#include <span>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {
template <typename T> struct matrix {
private:
    i32 N, M;
    std::vector<T> dat;

public:
    using value_type = T;

    matrix() : matrix(0) {}
    matrix(i32 N_) : matrix(N_, N_) {}
    matrix(i32 N_, i32 M_, T val = T{}) : N(N_), M(M_), dat(N * M, val) {}

    std::span<T> operator[](i32 i) { return std::span<T>(&dat[i * M], M); }
    std::span<const T> operator[](i32 i) const { return std::span<const T>(&dat[i * M], M); }

    matrix operator+(const matrix& other) const { return add(*this, other); }
    matrix operator-(const matrix& other) const { return sub(*this, other); }
    matrix operator*(const matrix& other) const { return mul(*this, other); }
    template <typename U> matrix operator*(const U& scalar) const {
        matrix ret = *this;
        for (T& t : ret.dat) t *= scalar;
        return ret;
    }

    matrix& operator+=(const matrix& other) {
        *this = *this + other;
        return this;
    }
    matrix& operator-=(const matrix& other) {
        *this = *this - other;
        return this;
    }
    matrix& operator*=(const matrix& other) {
        *this = *this * other;
        return this;
    }
    template <typename U> matrix& operator*=(const U& scalar) {
        *this = *this * scalar;
        return this;
    }

    static matrix add(const matrix& A, const matrix& B) {
        assert(A.N == B.N && A.M == B.M);
        matrix ret(A.N, A.M);
        for (i32 i = 0; i < A.N; ++i)
            for (i32 j = 0; j < A.M; ++j) {
                ret[i][j] = A[i][j] + B[i][j];
            }
        return ret;
    }
    static matrix sub(const matrix& A, const matrix& B) {
        assert(A.N == B.N && A.M == B.M);
        matrix ret(A.N, A.M);
        for (i32 i = 0; i < A.N; ++i)
            for (i32 j = 0; j < A.M; ++j) {
                ret[i][j] = A[i][j] - B[i][j];
            }
        return ret;
    }
    static matrix mul(const matrix& A, const matrix& B) {
        assert(A.M == B.N);
        matrix ret(A.N, B.M);
        for (i32 i = 0; i < A.N; ++i)
            for (i32 k = 0; k < A.M; ++k)
                for (i32 j = 0; j < B.M; ++j) {
                    ret[i][j] += A[i][k] * B[k][j];
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
        assert(A.N == A.M);
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