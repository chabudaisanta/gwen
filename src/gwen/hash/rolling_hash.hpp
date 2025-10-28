#pragma once

#include <cassert>

#include "gwen/types.hpp"
#include "gwen/mod/mod61.hpp"

namespace gwen {

namespace hash {

struct hash_segment {
    u64 v, p;
    explicit hash_segment() : v(0), p(1) {}
    explicit hash_segment(u64 v_, u64 p_) : v(v_), p(p_) {}

    bool operator==(const hash_segment& other) const {
        return (v == other.v) && (p == other.p);
    }
    bool operator!=(const hash_segment& other) const {
        return (v != other.v) || (p != other.p);
    }
};

}  // namespace hash

}  // namespace gwen