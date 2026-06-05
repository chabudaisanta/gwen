#include <cassert>
#include <iostream>
#include <vector>

#include "gwen/math/prime.hpp"

using namespace gwen;

static i64 product(const std::vector<i32>& fac) {
    i64 p = 1;
    for (i32 x : fac) p *= x;
    return p;
}

static void check(i32 n, const std::vector<i32>& expected) {
    auto fac = factorize(n);
    assert(fac == expected);
    assert(product(fac) == n);
}

int main() {
    // known 4-digit factorizations
    check(1000, {2, 2, 2, 5, 5, 5});
    check(1234, {2, 617});
    check(2024, {2, 2, 2, 11, 23});
    check(4320, {2, 2, 2, 2, 2, 3, 3, 3, 5});
    check(9999, {3, 3, 11, 101});
    check(8191, {8191});

    // extend() is exercised by growing the table past small limits
    for (i32 n = 1000; n <= 9999; ++n) {
        auto fac = factorize(n);
        assert(product(fac) == n);
        for (i32 p : fac) assert(p >= 2);
        for (std::size_t i = 1; i < fac.size(); ++i) assert(fac[i - 1] <= fac[i]);
    }

    assert(is_prime_small(1009));
    assert(is_prime_small(9973));
    assert(!is_prime_small(9999));
    assert(!is_prime_small(1000));

    std::cout << "ok\n";
    return 0;
}
