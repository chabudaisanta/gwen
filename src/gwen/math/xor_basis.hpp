#pragma once

#include <algorithm>
#include <bitset>
#include <string>

#include "gwen/types.hpp"

namespace gwen {

template <i32 BitWidth> struct xor_basis {
private:
    using BitSet = typename std::bitset<BitWidth>;
    BitSet basis[BitWidth] = {};
    i32 sz = 0;

public:
    xor_basis() = default;

    i32 size() const noexcept { return sz; }
    bool empty() const noexcept { return !sz; }
    BitSet base(i32 w) const noexcept { return basis[w]; }

    bool insert(BitSet x) {
        for (i32 i = BitWidth - 1; i >= 0; --i) {
            if (!x[i]) continue;
            if (!basis[i][i]) {
                basis[i] = x;
                sz++;
                return true;
            }
            x ^= basis[i];
        }
        return false;
    }

    bool can_form(BitSet x) const {
        for (i32 i = BitWidth - 1; i >= 0; --i) {
            if (!x[i]) continue;
            if (!basis[i][i]) return false;
            x ^= basis[i];
        }
        return x.none();
    }

    using S = xor_basis;
    static S op(S a, S b) {
        S &large = a, small = b;
        if (large.size() < small.size()) std::swap(large, small);
        for (i32 i = BitWidth - 1; i >= 0; --i) large.insert(small.base(i));
        return large;
    }
    static S e() { return S(); }

    std::string dump() const {
        std::string str;
        for (i32 i = BitWidth - 1; i >= 0; --i) str += basis[i].to_string() + "\n";
        return str;
    }
};
}  // namespace gwen