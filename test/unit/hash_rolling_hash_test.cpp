// clang-format off
#include "gwen/hash/rolling_hash.hpp"
// clang-format on

#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace gwen;

TEST(RollingHashTest, StringMatching) {
    std::string s = "abracadabra";
    rolling_hash<0> rh(s);
    
    // "abra" == "abra"
    EXPECT_TRUE(rh.equal(0, 4, 7, 11));
    // "abr" == "abr"
    EXPECT_TRUE(rh.equal(0, 3, 7, 10));
    // "abrac" != "dabra"
    EXPECT_FALSE(rh.equal(0, 5, 6, 11));
}

TEST(RollingHashTest, VectorMatching) {
    std::vector<int> v = {1, 2, 3, 1, 2, 3};
    rolling_hash<1> rh(v);
    
    EXPECT_TRUE(rh.equal(0, 3, 3, 6));
    EXPECT_FALSE(rh.equal(0, 2, 3, 6));
}

TEST(RollingHashTest, LCP) {
    std::string s = "abracadabra";
    rolling_hash<2> rh(s);
    
    // "abracadabra" vs "adabra" -> a matches, then b!=d -> LCP is 1
    EXPECT_EQ(rh.lcp(0, 5), 1);
    
    // "abracadabra" vs "abra" -> "abra" matches completely -> LCP is 4
    EXPECT_EQ(rh.lcp(0, 7), 4);
    
    // "abracadabra" vs "abracadabra" -> LCP is 11
    EXPECT_EQ(rh.lcp(0, 0), 11);
}

TEST(RollingHashTest, Rotations) {
    std::string s = "abcde";
    rolling_hash<3> rh(s);
    
    // rotl left shift by 2 -> "cdeab"
    auto h_rotl = rh.rotl(0, 5, 2);
    
    // rotr right shift by 3 -> "cdeab"
    auto h_rotr = rh.rotr(0, 5, 3);
    
    EXPECT_EQ(h_rotl, h_rotr);
    
    // Test negative shift
    auto h_rotl_neg = rh.rotl(0, 5, -3); // equivalent to right shift 3
    EXPECT_EQ(h_rotl, h_rotl_neg);
    
    // Manual construction of "cdeab"
    rolling_hash<3> rh_manual(std::string("cdeab"));
    EXPECT_EQ(h_rotl, rh_manual.get(0, 5));
}

TEST(RollingHashTest, Monoid) {
    using Monoid = rhash::rolling_hash_monoid<4>;
    
    std::string s1 = "hello";
    std::string s2 = "world";
    
    rolling_hash<4> rh1(s1);
    rolling_hash<4> rh2(s2);
    
    auto h1 = rh1.get(0, 5);
    auto h2 = rh2.get(0, 5);
    
    auto h_concat = Monoid::op(h1, h2);
    
    rolling_hash<4> rh_both(s1 + s2);
    EXPECT_EQ(h_concat, rh_both.get(0, 10));
    
    // Unit test
    auto e = Monoid::e();
    EXPECT_EQ(Monoid::op(h1, e), h1);
    EXPECT_EQ(Monoid::op(e, h2), h2);
}
