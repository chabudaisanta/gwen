#include <gtest/gtest.h>
#include <string>
#include "gwen/hash/rolling_hash.hpp"

using namespace gwen;

TEST(RollingHashTest, BasicOps) {
    std::string s = "otonarinototoro";
    rolling_hash<0> rh(s);

    // "to" appears at (1,3), (9,11), (11,13)
    auto to_1_3 = rh.get(1, 3);
    auto to_9_11 = rh.get(9, 11);
    auto to_11_13 = rh.get(11, 13);
    EXPECT_EQ(to_1_3.v, to_9_11.v);
    EXPECT_EQ(to_1_3.p, to_9_11.p);
    EXPECT_EQ(to_1_3.v, to_11_13.v);
    EXPECT_EQ(to_1_3.p, to_11_13.p);

    // "ototo" at (8,13) = "oto" (8,11) + "to" (11,13)
    auto ototo = rh.get(8, 13);
    auto oto = rh.get(8, 11);
    auto to = rh.get(11, 13);
    auto oto_to = rhash::rolling_hash_monoid<0>::op(oto, to);
    EXPECT_EQ(ototo.v, oto_to.v);
    EXPECT_EQ(ototo.p, oto_to.p);

    // size, equal, lcp
    EXPECT_EQ(rh.size(), 15);
    EXPECT_TRUE(rh.equal(1, 3, 9, 11));
    EXPECT_TRUE(rh.equal(1, 3, 11, 13));
    EXPECT_FALSE(rh.equal(1, 3, 2, 4));  // "to" vs "on"
    EXPECT_EQ(rh.lcp(0, 8), 3);      // "oto" vs "oto..."
    EXPECT_EQ(rh.lcp(1, 9), 2);      // "to..." vs "to..."
    EXPECT_EQ(rh.lcp(0, 14), 1);     // "o" vs "o"

    // rotl / rotr on [8,13) "ototo"
    EXPECT_EQ(rh.rotl(8, 13, 0).v, rh.get(8, 13).v);
    EXPECT_EQ(rh.rotl(8, 13, 2).v, rhash::rolling_hash_monoid<0>::op(rh.get(10, 13), rh.get(8, 10)).v);
    EXPECT_EQ(rh.rotr(8, 13, 2).v, rhash::rolling_hash_monoid<0>::op(rh.get(11, 13), rh.get(8, 11)).v);
    EXPECT_EQ(rh.rotr(8, 13, 3).v, rh.rotl(8, 13, 2).v);  // rotr k = rotl (len-k)
}
