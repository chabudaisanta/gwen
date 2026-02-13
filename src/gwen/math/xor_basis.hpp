#pragma once

#include <bitset>

#include "gwen/types.hpp"

namespace gwen {

template <i32 BitWidth> struct xor_basis {
private:
    using BitSet = typename std::bitset<BitWidth>;
    BitSet basis[BitWidth];
    i32 sz = 0;

public:
    xor_basis() = default;

    i32 size() const { return sz; }

    bool insert(BitSet x) {
        for (i32 i = BitWidth - 1; i >= 0; --i) {
            if (!x[i]) continue;
            if (basis[i].none()) {
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
            if (basis[i].none()) return false;
            x ^= basis[i];
        }
        return x.none();
    }
};
}  // namespace gwen