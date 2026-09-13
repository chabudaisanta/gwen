#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <limits>
#include <span>
#include <type_traits>
#include <utility>
#include <vector>

#include "gwen/alge/field.hpp"
#include "gwen/math/internal/ntt998.hpp"
#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 形式的冪級数
 * @tparam T 係数体
 */
template <field T> class FormalPowerSeries {
private:
    static constexpr usize sparse_cutoff = 60;

    std::vector<T> coeffs_;

    static std::vector<T> multiply(std::span<const T> lhs,
                                   std::span<const T> rhs,
                                   detail::ntt998_workspace& workspace) {
        if constexpr (std::same_as<T, modint998244353>) {
            return detail::ntt998_convolution(lhs, rhs, workspace);
        }
        else {
            if (lhs.empty() || rhs.empty()) return {};
            std::vector<T> res(lhs.size() + rhs.size() - 1, T(0));
            for (usize i = 0; i < lhs.size(); ++i) {
                for (usize j = 0; j < rhs.size(); ++j) res[i + j] += lhs[i] * rhs[j];
            }
            return res;
        }
    }

    static std::vector<std::pair<usize, T>> collect_nonzero_terms(std::span<const T> f, usize n) {
        std::vector<std::pair<usize, T>> terms;
        const usize size = std::min(f.size(), n);
        for (usize i = 0; i < size; ++i) {
            if (f[i] == T(0)) continue;
            terms.emplace_back(i, f[i]);
            if constexpr (std::same_as<T, modint998244353>) {
                if (terms.size() > sparse_cutoff) break;
            }
        }
        return terms;
    }

    static std::vector<modint998244353> inverse_table(usize n) {
        assert(n < modint998244353::mod());
        std::vector<modint998244353> inverse(n + 1);
        if (n == 0) return inverse;
        inverse[1] = modint998244353(1);
        for (usize i = 2; i <= n; ++i) {
            inverse[i] = -inverse[modint998244353::mod() % i] * modint998244353(modint998244353::mod() / i);
        }
        return inverse;
    }

    static std::vector<T> inverse_sparse(std::span<const T> f, usize n, const std::vector<std::pair<usize, T>>& terms) {
        std::vector<T> res(n, T(0));
        res[0] = T(1) / f[0];
        for (usize i = 1; i < n; ++i) {
            T value(0);
            for (const auto& [degree, coefficient] : terms) {
                if (degree == 0) continue;
                if (degree > i) break;
                value -= coefficient * res[i - degree];
            }
            res[i] = value * res[0];
        }
        return res;
    }

    static std::vector<modint998244353> inverse_dense(std::span<const modint998244353> f,
                                                      usize n,
                                                      detail::ntt998_workspace& workspace) {
        std::vector<modint998244353> res(n);
        res[0] = f[0].inv();
        for (usize d = 1; d < n; d <<= 1) {
            const usize z = d << 1;
            workspace.a.assign(z, modint998244353(0));
            workspace.b.assign(z, modint998244353(0));
            std::copy_n(f.begin(), std::min(f.size(), z), workspace.a.begin());
            std::copy_n(res.begin(), d, workspace.b.begin());
            detail::ntt998_forward(workspace.a);
            detail::ntt998_forward(workspace.b);
            for (usize i = 0; i < z; ++i) workspace.a[i] *= workspace.b[i];
            detail::ntt998_inverse(workspace.a);
            std::fill_n(workspace.a.begin(), d, modint998244353(0));
            detail::ntt998_forward(workspace.a);
            for (usize i = 0; i < z; ++i) workspace.a[i] *= workspace.b[i];
            detail::ntt998_inverse(workspace.a);
            for (usize i = d; i < std::min(n, z); ++i) res[i] = -workspace.a[i];
        }
        return res;
    }

    static std::vector<T> logarithm_sparse(std::span<const T> f,
                                           usize n,
                                           const std::vector<std::pair<usize, T>>& terms) {
        std::vector<T> res(n, T(0));
        std::vector<T> quotient(n == 0 ? 0 : n - 1, T(0));
        T denominator(0);
        for (usize i = 0; i + 1 < n; ++i) {
            denominator += T(1);
            assert(denominator != T(0));
            T value = (i + 1 < f.size() ? f[i + 1] : T(0)) * denominator;
            for (const auto& [degree, coefficient] : terms) {
                if (degree == 0) continue;
                if (degree > i) break;
                value -= coefficient * quotient[i - degree];
            }
            quotient[i] = value;
            res[i + 1] = value / denominator;
        }
        return res;
    }

    static std::vector<T> exponential_sparse(usize n, const std::vector<std::pair<usize, T>>& terms) {
        std::vector<std::pair<usize, T>> weighted;
        weighted.reserve(terms.size());
        T degree(0);
        usize previous = 0;
        for (const auto& [index, coefficient] : terms) {
            if (index == 0) continue;
            while (previous < index) {
                degree += T(1);
                ++previous;
            }
            weighted.emplace_back(index, coefficient * degree);
        }

        std::vector<T> res(n, T(0));
        res[0] = T(1);
        T denominator(0);
        for (usize i = 1; i < n; ++i) {
            denominator += T(1);
            assert(denominator != T(0));
            T value(0);
            for (const auto& [degree_index, coefficient] : weighted) {
                if (degree_index > i) break;
                value += coefficient * res[i - degree_index];
            }
            res[i] = value / denominator;
        }
        return res;
    }

    template <class Vector> static void differentiate_in_place(Vector& f) {
        if (f.empty()) return;
        f.erase(f.begin());
        modint998244353 degree(1);
        for (auto& coefficient : f) {
            coefficient *= degree;
            degree += modint998244353(1);
        }
    }

    template <class Vector> static void integrate_in_place(Vector& f) {
        const auto inverse = inverse_table(f.size());
        f.insert(f.begin(), modint998244353(0));
        for (usize i = 1; i < f.size(); ++i) f[i] *= inverse[i];
    }

    static std::vector<modint998244353> exponential_dense(std::span<const modint998244353> f,
                                                          usize n,
                                                          detail::ntt998_workspace& workspace) {
        if (n == 1) return {modint998244353(1)};
        std::vector<modint998244353> b{modint998244353(1), f.size() > 1 ? f[1] : modint998244353(0)};
        std::vector<modint998244353> c{modint998244353(1)};
        std::vector<modint998244353> z1;
        std::vector<modint998244353> z2{modint998244353(1), modint998244353(1)};
        b.reserve(n);
        c.reserve(n);
        z1.reserve(n);
        z2.reserve(n);

        for (usize m = 2; m < n; m <<= 1) {
            workspace.a.assign(b.begin(), b.end());
            auto& y = workspace.a;
            y.resize(m << 1);
            detail::ntt998_forward(y);

            z1 = z2;
            workspace.b.assign(m, modint998244353(0));
            auto& z = workspace.b;
            for (usize i = 0; i < m; ++i) z[i] = y[i] * z1[i];
            detail::ntt998_inverse(z);
            std::fill_n(z.begin(), m >> 1, modint998244353(0));
            detail::ntt998_forward(z);
            for (usize i = 0; i < m; ++i) z[i] *= -z1[i];
            detail::ntt998_inverse(z);
            c.insert(c.end(), z.begin() + static_cast<isize>(m >> 1), z.end());

            z2 = c;
            z2.resize(m << 1);
            detail::ntt998_forward(z2);

            workspace.b.assign(m, modint998244353(0));
            auto& x = workspace.b;
            std::copy_n(f.begin(), std::min(f.size(), m), x.begin());
            differentiate_in_place(x);
            x.push_back(modint998244353(0));
            detail::ntt998_forward(x);
            for (usize i = 0; i < m; ++i) x[i] *= y[i];
            detail::ntt998_inverse(x);

            modint998244353 degree(1);
            for (usize i = 0; i + 1 < b.size(); ++i) {
                x[i] -= b[i + 1] * degree;
                degree += modint998244353(1);
            }
            x.resize(m << 1);
            for (usize i = 0; i + 1 < m; ++i) {
                x[m + i] = x[i];
                x[i] = modint998244353(0);
            }
            detail::ntt998_forward(x);
            for (usize i = 0; i < (m << 1); ++i) x[i] *= z2[i];
            detail::ntt998_inverse(x);
            x.pop_back();
            integrate_in_place(x);
            for (usize i = m; i < std::min(f.size(), m << 1); ++i) x[i] += f[i];
            std::fill_n(x.begin(), m, modint998244353(0));
            detail::ntt998_forward(x);
            for (usize i = 0; i < (m << 1); ++i) x[i] *= y[i];
            detail::ntt998_inverse(x);
            b.insert(b.end(), x.begin() + static_cast<isize>(m), x.end());
        }
        b.resize(n);
        return b;
    }

public:
    /**
     * @brief 係数列から形式的冪級数を構築する
     * @param coeffs 低次から並べた係数列
     */
    explicit FormalPowerSeries(std::vector<T> coeffs = {}) : coeffs_(std::move(coeffs)) {}

    /**
     * @brief 初期化子リストから形式的冪級数を構築する
     * @param coeffs 低次から並べた係数列
     */
    FormalPowerSeries(std::initializer_list<T> coeffs) : coeffs_(coeffs) {}

    /** @brief 係数列の長さを返す */
    i32 size() const {
        assert(coeffs_.size() <= static_cast<usize>(std::numeric_limits<i32>::max()));
        return static_cast<i32>(coeffs_.size());
    }

    /** @brief 係数列が空か判定する */
    bool empty() const { return coeffs_.empty(); }

    /** @brief 指定次数の係数への参照を返す */
    T& operator[](i32 i) {
        assert(0 <= i && i < size());
        return coeffs_[static_cast<usize>(i)];
    }

    /** @brief 指定次数の係数への定数参照を返す */
    const T& operator[](i32 i) const {
        assert(0 <= i && i < size());
        return coeffs_[static_cast<usize>(i)];
    }

    /** @brief 先頭から最大 n 項を返す */
    FormalPowerSeries prefix(i32 n) const {
        assert(n >= 0);
        const usize length = std::min(coeffs_.size(), static_cast<usize>(n));
        return FormalPowerSeries(std::vector<T>(coeffs_.begin(), coeffs_.begin() + static_cast<isize>(length)));
    }

    /** @brief 各係数の符号を反転する */
    FormalPowerSeries operator-() const {
        auto res = *this;
        for (auto& coefficient : res.coeffs_) coefficient = -coefficient;
        return res;
    }

    /** @brief 形式的冪級数を加算する */
    FormalPowerSeries& operator+=(const FormalPowerSeries& rhs) {
        coeffs_.resize(std::max(coeffs_.size(), rhs.coeffs_.size()), T(0));
        for (usize i = 0; i < rhs.coeffs_.size(); ++i) coeffs_[i] += rhs.coeffs_[i];
        return *this;
    }

    /** @brief 形式的冪級数を減算する */
    FormalPowerSeries& operator-=(const FormalPowerSeries& rhs) {
        coeffs_.resize(std::max(coeffs_.size(), rhs.coeffs_.size()), T(0));
        for (usize i = 0; i < rhs.coeffs_.size(); ++i) coeffs_[i] -= rhs.coeffs_[i];
        return *this;
    }

    /** @brief 形式的冪級数を乗算する */
    FormalPowerSeries& operator*=(const FormalPowerSeries& rhs) {
        detail::ntt998_workspace workspace;
        coeffs_ = multiply(coeffs_, rhs.coeffs_, workspace);
        return *this;
    }

    /** @brief 各係数をスカラー倍する */
    FormalPowerSeries& operator*=(const T& scalar) {
        for (auto& coefficient : coeffs_) coefficient *= scalar;
        return *this;
    }

    /** @brief 各係数をスカラーで割る */
    FormalPowerSeries& operator/=(const T& scalar) {
        assert(scalar != T(0));
        for (auto& coefficient : coeffs_) coefficient /= scalar;
        return *this;
    }

    /** @brief 微分を返す */
    FormalPowerSeries derivative() const {
        if (coeffs_.empty()) return FormalPowerSeries();
        std::vector<T> res(coeffs_.size() - 1);
        T degree(0);
        for (usize i = 1; i < coeffs_.size(); ++i) {
            degree += T(1);
            res[i - 1] = coeffs_[i] * degree;
        }
        return FormalPowerSeries(std::move(res));
    }

    /** @brief 積分定数を 0 とした積分を返す */
    FormalPowerSeries integral() const {
        std::vector<T> res(coeffs_.size() + 1, T(0));
        if constexpr (std::same_as<T, modint998244353>) {
            const auto inverse = inverse_table(coeffs_.size());
            for (usize i = 0; i < coeffs_.size(); ++i) res[i + 1] = coeffs_[i] * inverse[i + 1];
        }
        else {
            T denominator(0);
            for (usize i = 0; i < coeffs_.size(); ++i) {
                denominator += T(1);
                assert(denominator != T(0));
                res[i + 1] = coeffs_[i] / denominator;
            }
        }
        return FormalPowerSeries(std::move(res));
    }

    /** @brief x^n 未満の乗法逆数を返す */
    FormalPowerSeries inv(i32 n) const {
        assert(n >= 0);
        if (n == 0) return FormalPowerSeries();
        assert(!coeffs_.empty() && coeffs_[0] != T(0));
        const usize length = static_cast<usize>(n);
        const auto terms = collect_nonzero_terms(coeffs_, length);
        if constexpr (std::same_as<T, modint998244353>) {
            if (terms.size() > sparse_cutoff) {
                assert(length <= detail::ntt998_max_size);
                detail::ntt998_workspace workspace;
                return FormalPowerSeries(inverse_dense(coeffs_, length, workspace));
            }
        }
        return FormalPowerSeries(inverse_sparse(coeffs_, length, terms));
    }

    /** @brief x^n 未満の対数を返す */
    FormalPowerSeries log(i32 n) const {
        assert(n >= 0);
        if (n == 0) return FormalPowerSeries();
        assert(!coeffs_.empty() && coeffs_[0] == T(1));
        const usize length = static_cast<usize>(n);
        const auto terms = collect_nonzero_terms(coeffs_, length);
        if constexpr (std::same_as<T, modint998244353>) {
            if (terms.size() > sparse_cutoff) {
                assert(length <= detail::ntt998_max_size);
                detail::ntt998_workspace workspace;
                auto inverse = inverse_dense(coeffs_, length, workspace);
                std::vector<T> derivative_coeffs(length - 1, T(0));
                T degree(0);
                for (usize i = 1; i < std::min(coeffs_.size(), length); ++i) {
                    degree += T(1);
                    derivative_coeffs[i - 1] = coeffs_[i] * degree;
                }
                auto product = multiply(derivative_coeffs, inverse, workspace);
                product.resize(length - 1);
                return FormalPowerSeries(std::move(product)).integral();
            }
        }
        return FormalPowerSeries(logarithm_sparse(coeffs_, length, terms));
    }

    /** @brief x^n 未満の指数を返す */
    FormalPowerSeries exp(i32 n) const {
        assert(n >= 0);
        if (n == 0) return FormalPowerSeries();
        assert(coeffs_.empty() || coeffs_[0] == T(0));
        const usize length = static_cast<usize>(n);
        const auto terms = collect_nonzero_terms(coeffs_, length);
        if constexpr (std::same_as<T, modint998244353>) {
            if (terms.size() > sparse_cutoff) {
                assert(length <= detail::ntt998_max_size);
                detail::ntt998_workspace workspace;
                return FormalPowerSeries(exponential_dense(coeffs_, length, workspace));
            }
        }
        return FormalPowerSeries(exponential_sparse(length, terms));
    }

    /** @brief 形式的冪級数同士の和を返す */
    friend FormalPowerSeries operator+(FormalPowerSeries lhs, const FormalPowerSeries& rhs) { return lhs += rhs; }

    /** @brief 形式的冪級数同士の差を返す */
    friend FormalPowerSeries operator-(FormalPowerSeries lhs, const FormalPowerSeries& rhs) { return lhs -= rhs; }

    /** @brief 形式的冪級数同士の積を返す */
    friend FormalPowerSeries operator*(FormalPowerSeries lhs, const FormalPowerSeries& rhs) { return lhs *= rhs; }

    /** @brief 形式的冪級数のスカラー倍を返す */
    friend FormalPowerSeries operator*(FormalPowerSeries lhs, const T& scalar) { return lhs *= scalar; }

    /** @brief 形式的冪級数のスカラー倍を返す */
    friend FormalPowerSeries operator*(const T& scalar, FormalPowerSeries rhs) { return rhs *= scalar; }

    /** @brief 形式的冪級数をスカラーで割った結果を返す */
    friend FormalPowerSeries operator/(FormalPowerSeries lhs, const T& scalar) { return lhs /= scalar; }
};

}  // namespace gwen
