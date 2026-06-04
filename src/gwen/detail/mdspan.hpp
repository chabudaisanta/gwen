#pragma once

#include <array>
#include <cstddef>
#include <utility>

#include "gwen/types.hpp"

// std::mdspan when the toolchain ships <mdspan>; otherwise a minimal polyfill
// with the same interface used by gwen::mdarray.

#if defined(__has_include) && __has_include(<mdspan>)
#include <mdspan>
#define GWEN_HAS_STD_MDSPAN 1
#endif

namespace gwen::detail {

#ifdef GWEN_HAS_STD_MDSPAN

template <i32 Rank> using mdspan_extents = std::dextents<i32, Rank>;

template <typename T, i32 Rank> using mdspan_type = std::mdspan<T, mdspan_extents<Rank>>;

template <i32 Rank> constexpr mdspan_extents<Rank> make_extents(const std::array<i32, Rank>& shape) {
    return [&]<size_t... Is>(std::index_sequence<Is...>) {
        return mdspan_extents<Rank>{shape[Is]...};
    }(std::make_index_sequence<Rank>{});
}

#else

template <typename IndexType, i32 Rank> struct mdspan_extents_impl {
    std::array<IndexType, Rank> sizes{};

    static constexpr i32 rank() { return Rank; }

    constexpr IndexType extent(i32 r) const { return sizes[r]; }

    template <typename... Is> constexpr size_t offset(Is... indices) const {
        static_assert(sizeof...(Is) == Rank);
        std::array<IndexType, Rank> idx{IndexType(indices)...};
        size_t off = 0, stride = 1;
        for (i32 r = Rank - 1; r >= 0; --r) {
            off += size_t(idx[r]) * stride;
            stride *= size_t(sizes[r]);
        }
        return off;
    }
};

template <typename T, typename Extents> struct mdspan_impl {
    T* ptr = nullptr;
    Extents ext{};

    constexpr mdspan_impl() = default;
    constexpr mdspan_impl(T* p, Extents e) : ptr(p), ext(e) {}

    template <typename... Is> constexpr T& operator()(Is... indices) const { return ptr[ext.offset(indices...)]; }

    constexpr T* data_handle() const { return ptr; }
    constexpr i32 rank() const { return Extents::rank(); }
    constexpr i32 extent(i32 r) const { return ext.extent(r); }
};

template <i32 Rank> using mdspan_extents = mdspan_extents_impl<i32, Rank>;

template <typename T, i32 Rank> using mdspan_type = mdspan_impl<T, mdspan_extents<Rank>>;

template <i32 Rank> constexpr mdspan_extents<Rank> make_extents(const std::array<i32, Rank>& shape) {
    mdspan_extents<Rank> ext{};
    ext.sizes = shape;
    return ext;
}

#endif

}  // namespace gwen::detail
