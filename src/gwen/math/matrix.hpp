#include <cassert>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {
template <typename T>
std::vector<std::vector<T>> matrix_mul(const std::vector<std::vector<T>>& a, const std::vector<std::vector<T>>& b) {
    i32 H = a.size(), W = b[0].size();
    i32 N = a[0].size();
    std::vector<std::vector<T>> ret(H, std::vector<T>(W));
    for (i32 i = 0; i < H; ++i)
        for (i32 k = 0; k < N; ++k)
            for (i32 j = 0; j < W; ++j) {
                ret[i][j] += a[i][k] * b[k][j];
            }
    return ret;
}
template <typename T> T bin_exp(T x, i64 n, const T& e, auto&& op) {
    T ret = e;
    while (n) {
        if (n & 1) ret = op(x, ret);
        x = op(x, x);
        n >>= 1;
    }
    return ret;
}
}  // namespace gwen