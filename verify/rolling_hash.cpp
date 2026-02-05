#include <cassert>
#include <iostream>
#include <string>

#include "gwen/hash/rolling_hash.hpp"

using namespace gwen;

int main() {
    std::string s = "otonarinototoro";
    rolling_hash<0> rh(s);

    // "to" appears at (1,3), (9,11), (11,13)
    auto to_1_3 = rh.get(1, 3);
    auto to_9_11 = rh.get(9, 11);
    auto to_11_13 = rh.get(11, 13);
    assert(to_1_3.v == to_9_11.v && to_1_3.p == to_9_11.p);
    assert(to_1_3.v == to_11_13.v && to_1_3.p == to_11_13.p);

    // "ototo" at (8,13) = "oto" (8,11) + "to" (11,13)
    auto ototo = rh.get(8, 13);
    auto oto = rh.get(8, 11);
    auto to = rh.get(11, 13);
    auto oto_to = rhash::rolling_hash_monoid<0>::op(oto, to);
    assert(ototo.v == oto_to.v && ototo.p == oto_to.p);

    // size, equal, lcp
    assert(rh.size() == 15);
    assert(rh.equal(1, 3, 9, 11));
    assert(rh.equal(1, 3, 11, 13));
    assert(!rh.equal(1, 3, 2, 4));  // "to" vs "on"
    assert(rh.lcp(0, 8) == 3);      // "oto" vs "oto..."
    assert(rh.lcp(1, 9) == 2);      // "to..." vs "to..."
    assert(rh.lcp(0, 14) == 1);     // "o" vs "o"

    std::cout << "rolling_hash: all tests passed\n";
    return 0;
}
