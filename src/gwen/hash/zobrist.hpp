#pragma once

#include <functional>

#include "gwen/hash/to_hash.hpp"
#include "gwen/types.hpp"

namespace gwen {
namespace hash {

template <typename T, bool isMultiset = false>
struct zobrist {
private:
    static u64 elem_hash(const T& x) {
        return to_hash(static_cast<u64>(std::hash<T>{}(x)));
    }

    i32 sz = 0;
    u64 val = 0;

public:
    zobrist() = default;

    inline i32 size() const { return sz; }
    inline bool empty() const { return !sz; }
    inline u64 get() const { return val; }
    inline bool operator==(const zobrist& other) const { return (sz == other.sz) && (val == other.val); }
    inline bool operator!=(const zobrist& other) const { return !(*this == other); }

    inline void insert(const T& x) {
        u64 h = elem_hash(x);
        if constexpr (isMultiset) {
            val += h;
        } else {
            val ^= h;
        }
        sz++;
    }

    inline void erase(const T& x) {
        u64 h = elem_hash(x);
        if constexpr (isMultiset) {
            val -= h;
        } else {
            val ^= h;
        }
        sz--;
    }
};

}  // namespace hash

}  // namespace gwen