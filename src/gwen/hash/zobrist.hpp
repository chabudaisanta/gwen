#pragma once
#include <unordered_map>

#include "gwen/misc/xorshift.hpp"
namespace gwen {
namespace hash {

template <typename T, bool isMultiset = false, i32 Reserve_size = 200200> struct zobrist {
private:
    static inline std::unordered_map<T, u64> table;
    static inline bool reserved = false;
    i32 sz = 0;
    u64 val = 0;

public:
    explicit zobrist() {
        if (!reserved) {
            table.reserve(Reserve_size);
            reserved = true;
        }
    }

    inline i32 size() const { return sz; }
    inline bool empty() const { return !sz; }
    inline u64 get() const { return val; }
    inline bool operator==(const zobrist& other) const { return (sz == other.sz) && (val == other.val); }
    inline bool operator!=(const zobrist& other) const { return !(*this == other); }

    inline void insert(const T& x) {
        if (!table.contains(x)) {
            table[x] = rand64();
        }
        if constexpr (isMultiset) {
            val += table[x];
        }
        else {
            // この構造体内で重複判定はしない.
            val ^= table[x];
        }
        sz++;
    }

    inline void erase(const T& x) {
        assert(table.contains(x));
        if constexpr (isMultiset) {
            val -= table[x];
        }
        else {
            val ^= table[x];
        }
        sz--;
    }
};

}  // namespace hash

}  // namespace gwen