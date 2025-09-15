#pragma once

#include "gwen/types.hpp"
#include "gwen/mod/modint.hpp"

namespace gwen {

bool miller32(u32 n) {
    static u64 a[3] = {2,7,61};
    using mint = dynamic_modint64;
    mint::set_mod(n);
    u64 s = 0, d = n - 1;
    while(!(d & 1)) {
        ++s;
        d >>= 1;
    }
    for(int i = 0; i < 3; ++i) {
        if(n <= a[i]) return true;
        mint x{a[i]};
        x = x.pow(d);
        if(x.val() != 1u) {
            u64 t;
            for(t = 0; t < s; ++t) {
                if(x.val() == n-1) break;
                x *= x;
            }
            if(t == s) return false;
        }
    }
    return true;
}

bool miller64(u64 n) {
    static u64 a[7] = {2,325,9375,28178,450775,9780504,1795265022};
    using mint = dynamic_modint64;
    mint::set_mod(n);
    u64 s = 0, d = n - 1;
    while(!(d & 1)) {
        ++s;
        d >>= 1;
    }
    for(int i = 0; i < 7; ++i) {
        if(n <= a[i]) return true;
        mint x{a[i]};
        x = x.pow(d);
        u64 t;
        if(x.val() != 1u) {
            for(t = 0; t < s; ++t) {
                if(x.val() == n-1) break;
                x *= x;
            }
            if(t == s) return false;
        }
    }
    return true;
}

bool miller(u64 n) {
    if(n <= 1) return false;
    else if(n == 2) return true;
    else if(!(n & 1)) return false;
    else if(n < 4759123141u) return miller32(n);
    else return miller64(n);
}

} // namespace gwen