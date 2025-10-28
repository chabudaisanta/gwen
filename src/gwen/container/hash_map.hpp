#pragma once

#include <algorithm>
#include "gwen/types.hpp"
#include "gwen/hash/to_hash.hpp"

namespace gwen {

template<typename Key, typename Val, u64(*to_hash)(Key)= gwen::to_hash, i32 W = 22>
class fixed_hash_map {
private:
    static constexpr i32 sz = 1 << W;
    Val dt[sz];
    Key ky[sz];
    i16 st[sz];
    static constexpr i16 ST_EMPTY = 0, ST_EXIST = 1, ST_DELETED = 2;

    i32 search(const Key& k) {
        i32 p = to_hash(k) & (sz - 1);
        while(st[p] != ST_EMPTY && ky[p] != k) p = (p + 1) & (sz - 1);
        return p;
    }

public:
    explicit fixed_hash_map() {
        std::fill(st, st + sz, ST_EMPTY);
    }

    void insert(const Key& k, const Val& v) {
        i32 p = search(k);
        st[p] = ST_EXIST;
        ky[p] = k;
        dt[p] = v;
    }
    void erase(const Key& k) {
        i32 p = search(k);
        if(st[p] != ST_EMPTY) st[p] = ST_DELETED;
    }

    Key& at(const Key& k) {
        i32 p = search(k);
        if(st[p] != ST_EXIST) {
            st[p] = ST_EXIST;
            ky[p] = k;
            dt[p] = Val();
        }
        return dt[p];
    }

    Key& operator[](const Key& k) {
        return at(k);
    }
};
}