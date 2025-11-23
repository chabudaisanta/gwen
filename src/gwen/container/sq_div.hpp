#include <vector>
#include "gwen/types.hpp"

namespace gwen {

template<typename S, S (*op)(S, S), S (*e)()>
struct sq_div {
    static constexpr i32 bsize = 512;
    i32 n;
    i32 m;
    std::vector<S> raw;
    std::vector<S> d;

    sq_div(const vector<S>& x) : n(x.size()), m(n / bsize + (n % bsize > 0)), raw(x) {
        d.resize(m);
        for(i32 p = 0; p < m; ++p) update(p);
    }

    void set(i32 p, S x) {
        raw[p] = x;
        i32 q = p / bsize;
        update(q);
    }

    S prod(i32 l, i32 r) {
        S ret = e();
        while(l % bsize != 0 && l < r) {
            ret = op(ret, raw[l]);
            l++;
        }
        while(l + bsize <= r) {
            ret = op(ret, d[l / bsize]);
            l += bsize;
        }
        while(l < r) {
            ret = op(ret, raw[l]);
            l++;
        }
        return ret;
    }

    void update(i32 q) {
        d[q] = e();
        i32 offset = q * bsize;
        for(i32 i = 0; i < bsize && i + offset < n; i++) {
            d[q] = op(d[q], raw[offset + i]);
        }
    }
};

} // namespace gwen