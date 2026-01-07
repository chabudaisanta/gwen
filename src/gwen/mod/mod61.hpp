#include "gwen/types.hpp"

namespace gwen {
namespace mod61 {

constexpr u64 mod61 = (1ull << 61) - 1;
constexpr u64 msk30 = (1ull << 30) - 1;
constexpr u64 msk31 = (1ull << 31) - 1;
constexpr u64 msk61 = (1ull << 61) - 1;

constexpr u64 calc_mod(u64 x) {
    u64 res = (x >> 61) + (x & msk61);
    if (res >= mod61) res -= mod61;
    return res;
}

constexpr u64 mul_mod(u64 a, u64 b) {
    u64 au = a >> 31, ad = a & msk31;
    u64 bu = b >> 31, bd = b & msk31;
    u64 mid = au * bd + ad * bu;
    u64 mu = mid >> 30, md = mid & msk30;
    return calc_mod(au * bu * 2 + mu + (md << 31) + ad * bd);
}

constexpr u64 add_mod(u64 a, u64 b) {
    u64 res = a + b;
    if (res >= mod61) res -= mod61;
    return res;
}

constexpr u64 sub_mod(u64 a, u64 b) {
    if (a < b) return a + mod61 - b;
    return a - b;
}
}  // namespace mod61
}  // namespace gwen