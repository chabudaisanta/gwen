#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <memory>
#include <new>
#include <span>
#include <vector>

#if defined(__AVX2__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#define GWEN_NTT998_USE_AVX2 1
#else
#define GWEN_NTT998_USE_AVX2 0
#endif

#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

namespace gwen::detail {

struct ntt998_access {
    using mint = modint998244353;

    static constexpr u32 raw(const mint& x) { return x.tr; }
};

inline constexpr usize ntt998_max_size = usize{1} << 23;
inline constexpr usize ntt998_naive_cutoff = 60;

template <class T, usize Alignment> class aligned_allocator {
public:
    using value_type = T;

    aligned_allocator() = default;

    template <class U> constexpr aligned_allocator(const aligned_allocator<U, Alignment>&) noexcept {}

    [[nodiscard]] T* allocate(usize n) {
        return static_cast<T*>(::operator new(n * sizeof(T), std::align_val_t{Alignment}));
    }

    void deallocate(T* p, usize) noexcept { ::operator delete(p, std::align_val_t{Alignment}); }

    template <class U> struct rebind {
        using other = aligned_allocator<U, Alignment>;
    };
};

template <class T, class U, usize Alignment>
constexpr bool operator==(const aligned_allocator<T, Alignment>&, const aligned_allocator<U, Alignment>&) noexcept {
    return true;
}

using ntt998_vector = std::vector<modint998244353, aligned_allocator<modint998244353, 32>>;

struct ntt998_workspace {
    ntt998_vector a;
    ntt998_vector b;
};

struct ntt998_info {
    using mint = modint998244353;
    static constexpr i32 rank = 23;

    std::array<mint, rank + 1> root{};
    std::array<mint, rank + 1> iroot{};
    std::array<mint, rank + 1> rate2{};
    std::array<mint, rank + 1> irate2{};
    std::array<mint, rank + 1> rate3{};
    std::array<mint, rank + 1> irate3{};

    ntt998_info() {
        root[rank] = mint(3).pow((mint::mod() - 1) >> rank);
        iroot[rank] = root[rank].inv();
        for (i32 i = rank - 1; i >= 0; --i) {
            root[i] = root[i + 1] * root[i + 1];
            iroot[i] = iroot[i + 1] * iroot[i + 1];
        }
        mint prod(1), iprod(1);
        for (i32 i = 0; i <= rank - 2; ++i) {
            rate2[i] = root[i + 2] * prod;
            irate2[i] = iroot[i + 2] * iprod;
            prod *= iroot[i + 2];
            iprod *= root[i + 2];
        }
        prod = iprod = mint(1);
        for (i32 i = 0; i <= rank - 3; ++i) {
            rate3[i] = root[i + 3] * prod;
            irate3[i] = iroot[i + 3] * iprod;
            prod *= iroot[i + 3];
            iprod *= root[i + 3];
        }
    }
};

inline const ntt998_info& get_ntt998_info() {
    static const ntt998_info info;
    return info;
}

#if GWEN_NTT998_USE_AVX2

using ntt998_vec = __m256i;

inline ntt998_vec ntt998_load(const modint998244353* p) {
    return _mm256_loadu_si256(reinterpret_cast<const ntt998_vec*>(p));
}

inline void ntt998_store(modint998244353* p, ntt998_vec x) { _mm256_storeu_si256(reinterpret_cast<ntt998_vec*>(p), x); }

inline ntt998_vec ntt998_add(ntt998_vec a, ntt998_vec b) {
    const ntt998_vec mod = _mm256_set1_epi32(static_cast<i32>(modint998244353::mod()));
    const ntt998_vec sum = _mm256_add_epi32(a, b);
    return _mm256_min_epu32(sum, _mm256_sub_epi32(sum, mod));
}

inline ntt998_vec ntt998_sub(ntt998_vec a, ntt998_vec b) {
    const ntt998_vec mod = _mm256_set1_epi32(static_cast<i32>(modint998244353::mod()));
    const ntt998_vec diff = _mm256_sub_epi32(a, b);
    return _mm256_min_epu32(diff, _mm256_add_epi32(diff, mod));
}

inline ntt998_vec ntt998_mul(ntt998_vec a, ntt998_vec b) {
    constexpr u32 ninv = 998244351;
    const ntt998_vec vninv = _mm256_set1_epi32(static_cast<i32>(ninv));
    const ntt998_vec vmod = _mm256_set1_epi32(static_cast<i32>(modint998244353::mod()));

    const ntt998_vec even = _mm256_mul_epu32(a, b);
    const ntt998_vec odd = _mm256_mul_epu32(_mm256_srli_epi64(a, 32), _mm256_srli_epi64(b, 32));
    const ntt998_vec qe = _mm256_mul_epu32(even, vninv);
    const ntt998_vec qo = _mm256_mul_epu32(odd, vninv);
    const ntt998_vec re = _mm256_srli_epi64(_mm256_add_epi64(even, _mm256_mul_epu32(qe, vmod)), 32);
    const ntt998_vec ro = _mm256_srli_epi64(_mm256_add_epi64(odd, _mm256_mul_epu32(qo, vmod)), 32);
    const ntt998_vec res = _mm256_or_si256(re, _mm256_slli_epi64(ro, 32));
    return _mm256_min_epu32(res, _mm256_sub_epi32(res, vmod));
}

inline ntt998_vec ntt998_mul(ntt998_vec a, u32 b) { return ntt998_mul(a, _mm256_set1_epi32(static_cast<i32>(b))); }

inline void ntt998_forward8(modint998244353* a0,
                            modint998244353* a1,
                            modint998244353* a2,
                            modint998244353* a3,
                            u32 rot,
                            u32 rot2,
                            u32 rot3,
                            u32 imag) {
    const ntt998_vec x0 = ntt998_load(a0);
    const ntt998_vec x1 = ntt998_mul(ntt998_load(a1), rot);
    const ntt998_vec x2 = ntt998_mul(ntt998_load(a2), rot2);
    const ntt998_vec x3 = ntt998_mul(ntt998_load(a3), rot3);
    const ntt998_vec s02 = ntt998_add(x0, x2);
    const ntt998_vec d02 = ntt998_sub(x0, x2);
    const ntt998_vec s13 = ntt998_add(x1, x3);
    const ntt998_vec d13 = ntt998_mul(ntt998_sub(x1, x3), imag);
    ntt998_store(a0, ntt998_add(s02, s13));
    ntt998_store(a1, ntt998_sub(s02, s13));
    ntt998_store(a2, ntt998_add(d02, d13));
    ntt998_store(a3, ntt998_sub(d02, d13));
}

inline void ntt998_inverse8(modint998244353* a0,
                            modint998244353* a1,
                            modint998244353* a2,
                            modint998244353* a3,
                            u32 irot,
                            u32 irot2,
                            u32 irot3,
                            u32 iimag) {
    const ntt998_vec x0 = ntt998_load(a0);
    const ntt998_vec x1 = ntt998_load(a1);
    const ntt998_vec x2 = ntt998_load(a2);
    const ntt998_vec x3 = ntt998_load(a3);
    const ntt998_vec s01 = ntt998_add(x0, x1);
    const ntt998_vec d01 = ntt998_sub(x0, x1);
    const ntt998_vec s23 = ntt998_add(x2, x3);
    const ntt998_vec d23 = ntt998_mul(ntt998_sub(x2, x3), iimag);
    ntt998_store(a0, ntt998_add(s01, s23));
    ntt998_store(a1, ntt998_mul(ntt998_add(d01, d23), irot));
    ntt998_store(a2, ntt998_mul(ntt998_sub(s01, s23), irot2));
    ntt998_store(a3, ntt998_mul(ntt998_sub(d01, d23), irot3));
}

#endif

inline void ntt998_forward(std::span<modint998244353> a) {
    const usize n = a.size();
    assert(n != 0 && std::has_single_bit(n) && n <= ntt998_max_size);
    const i32 h = static_cast<i32>(std::countr_zero(n));
    const auto& info = get_ntt998_info();
    i32 len = 0;
    while (len < h) {
        if (h - len == 1) {
            const usize p = usize{1} << (h - len - 1);
            modint998244353 rot(1);
            for (usize s = 0; s < (usize{1} << len); ++s) {
                const usize offset = s << (h - len);
                for (usize i = 0; i < p; ++i) {
                    const auto l = a[offset + i];
                    const auto r = a[offset + i + p] * rot;
                    a[offset + i] = l + r;
                    a[offset + i + p] = l - r;
                }
                if (s + 1 != (usize{1} << len)) {
                    rot *= info.rate2[std::countr_zero(~static_cast<u32>(s))];
                }
            }
            ++len;
            continue;
        }

        const usize p = usize{1} << (h - len - 2);
        modint998244353 rot(1);
        const modint998244353 imag = info.root[2];
        for (usize s = 0; s < (usize{1} << len); ++s) {
            const modint998244353 rot2 = rot * rot;
            const modint998244353 rot3 = rot2 * rot;
            const usize offset = s << (h - len);
            usize i = 0;
#if GWEN_NTT998_USE_AVX2
            for (; i + 8 <= p; i += 8) {
                ntt998_forward8(a.data() + offset + i, a.data() + offset + p + i, a.data() + offset + 2 * p + i,
                                a.data() + offset + 3 * p + i, ntt998_access::raw(rot), ntt998_access::raw(rot2),
                                ntt998_access::raw(rot3), ntt998_access::raw(imag));
            }
#endif
            for (; i < p; ++i) {
                const auto x0 = a[offset + i];
                const auto x1 = a[offset + p + i] * rot;
                const auto x2 = a[offset + 2 * p + i] * rot2;
                const auto x3 = a[offset + 3 * p + i] * rot3;
                const auto s02 = x0 + x2;
                const auto d02 = x0 - x2;
                const auto s13 = x1 + x3;
                const auto d13 = (x1 - x3) * imag;
                a[offset + i] = s02 + s13;
                a[offset + p + i] = s02 - s13;
                a[offset + 2 * p + i] = d02 + d13;
                a[offset + 3 * p + i] = d02 - d13;
            }
            if (s + 1 != (usize{1} << len)) {
                rot *= info.rate3[std::countr_zero(~static_cast<u32>(s))];
            }
        }
        len += 2;
    }
}

inline void ntt998_inverse(std::span<modint998244353> a) {
    const usize n = a.size();
    assert(n != 0 && std::has_single_bit(n) && n <= ntt998_max_size);
    const i32 h = static_cast<i32>(std::countr_zero(n));
    const auto& info = get_ntt998_info();
    i32 len = h;
    while (len > 0) {
        if (len == 1) {
            const usize p = usize{1} << (h - len);
            modint998244353 irot(1);
            for (usize s = 0; s < (usize{1} << (len - 1)); ++s) {
                const usize offset = s << (h - len + 1);
                for (usize i = 0; i < p; ++i) {
                    const auto l = a[offset + i];
                    const auto r = a[offset + i + p];
                    a[offset + i] = l + r;
                    a[offset + i + p] = (l - r) * irot;
                }
                if (s + 1 != (usize{1} << (len - 1))) {
                    irot *= info.irate2[std::countr_zero(~static_cast<u32>(s))];
                }
            }
            --len;
            continue;
        }

        const usize p = usize{1} << (h - len);
        modint998244353 irot(1);
        const modint998244353 iimag = info.iroot[2];
        for (usize s = 0; s < (usize{1} << (len - 2)); ++s) {
            const modint998244353 irot2 = irot * irot;
            const modint998244353 irot3 = irot2 * irot;
            const usize offset = s << (h - len + 2);
            usize i = 0;
#if GWEN_NTT998_USE_AVX2
            for (; i + 8 <= p; i += 8) {
                ntt998_inverse8(a.data() + offset + i, a.data() + offset + p + i, a.data() + offset + 2 * p + i,
                                a.data() + offset + 3 * p + i, ntt998_access::raw(irot), ntt998_access::raw(irot2),
                                ntt998_access::raw(irot3), ntt998_access::raw(iimag));
            }
#endif
            for (; i < p; ++i) {
                const auto x0 = a[offset + i];
                const auto x1 = a[offset + p + i];
                const auto x2 = a[offset + 2 * p + i];
                const auto x3 = a[offset + 3 * p + i];
                const auto s01 = x0 + x1;
                const auto d01 = x0 - x1;
                const auto s23 = x2 + x3;
                const auto d23 = (x2 - x3) * iimag;
                a[offset + i] = s01 + s23;
                a[offset + p + i] = (d01 + d23) * irot;
                a[offset + 2 * p + i] = (s01 - s23) * irot2;
                a[offset + 3 * p + i] = (d01 - d23) * irot3;
            }
            if (s + 1 != (usize{1} << (len - 2))) {
                irot *= info.irate3[std::countr_zero(~static_cast<u32>(s))];
            }
        }
        len -= 2;
    }

    const modint998244353 inv_n = modint998244353(n).inv();
    usize i = 0;
#if GWEN_NTT998_USE_AVX2
    for (; i + 8 <= n; i += 8) {
        ntt998_store(a.data() + i, ntt998_mul(ntt998_load(a.data() + i), ntt998_access::raw(inv_n)));
    }
#endif
    for (; i < n; ++i) a[i] *= inv_n;
}

inline usize ntt998_transform_size(usize n, usize m) {
    if (n == 0 || m == 0) return 0;
    if (n > ntt998_max_size || m > ntt998_max_size || n > ntt998_max_size - m + 1) {
        assert(false && "ntt998_transform_size(): result is too large");
        return 0;
    }
    return std::bit_ceil(n + m - 1);
}

inline std::vector<modint998244353> ntt998_convolution_naive(std::span<const modint998244353> a,
                                                             std::span<const modint998244353> b) {
    if (a.empty() || b.empty()) return {};
    std::vector<modint998244353> res(a.size() + b.size() - 1);
    for (usize i = 0; i < a.size(); ++i) {
        for (usize j = 0; j < b.size(); ++j) res[i + j] += a[i] * b[j];
    }
    return res;
}

inline std::vector<modint998244353> ntt998_convolution(std::span<const modint998244353> a,
                                                       std::span<const modint998244353> b,
                                                       ntt998_workspace& workspace) {
    if (a.empty() || b.empty()) return {};
    if (std::min(a.size(), b.size()) <= ntt998_naive_cutoff) return ntt998_convolution_naive(a, b);

    const usize z = ntt998_transform_size(a.size(), b.size());
    if (z == 0) return {};
    const usize result_size = a.size() + b.size() - 1;
    workspace.a.assign(z, modint998244353{});
    std::copy(a.begin(), a.end(), workspace.a.begin());

    const bool square = a.data() == b.data() && a.size() == b.size();
    ntt998_forward(workspace.a);
    if (square) {
        usize i = 0;
#if GWEN_NTT998_USE_AVX2
        for (; i + 8 <= z; i += 8) {
            const ntt998_vec x = ntt998_load(workspace.a.data() + i);
            ntt998_store(workspace.a.data() + i, ntt998_mul(x, x));
        }
#endif
        for (; i < z; ++i) workspace.a[i] *= workspace.a[i];
    }
    else {
        workspace.b.assign(z, modint998244353{});
        std::copy(b.begin(), b.end(), workspace.b.begin());
        ntt998_forward(workspace.b);
        usize i = 0;
#if GWEN_NTT998_USE_AVX2
        for (; i + 8 <= z; i += 8) {
            ntt998_store(workspace.a.data() + i,
                         ntt998_mul(ntt998_load(workspace.a.data() + i), ntt998_load(workspace.b.data() + i)));
        }
#endif
        for (; i < z; ++i) workspace.a[i] *= workspace.b[i];
    }
    ntt998_inverse(workspace.a);

    std::vector<modint998244353> res(result_size);
    std::copy_n(workspace.a.begin(), result_size, res.begin());
    return res;
}

}  // namespace gwen::detail

#undef GWEN_NTT998_USE_AVX2
