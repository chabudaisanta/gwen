// clang-format off
#include "gwen/math/prime.hpp"
// clang-format on

#include <gtest/gtest.h>

using namespace gwen;

TEST(PrimeTest, LpfTableEratosthenes) {
    LpfTable table(100);
    EXPECT_FALSE(table.is_prime(0));
    EXPECT_FALSE(table.is_prime(1));
    EXPECT_TRUE(table.is_prime(2));
    EXPECT_TRUE(table.is_prime(3));
    EXPECT_FALSE(table.is_prime(4));
    EXPECT_TRUE(table.is_prime(5));
    EXPECT_FALSE(table.is_prime(9));
    EXPECT_TRUE(table.is_prime(97));
    EXPECT_FALSE(table.is_prime(100));

    EXPECT_EQ(table.factorize(12), (std::vector<i32>{2, 2, 3}));
    EXPECT_EQ(table.factorize(97), (std::vector<i32>{97}));
    EXPECT_EQ(table.factorize(100), (std::vector<i32>{2, 2, 5, 5}));
}

TEST(PrimeTest, LinearLpfTable) {
    LinearLpfTable table(100);
    EXPECT_FALSE(table.is_prime(0));
    EXPECT_FALSE(table.is_prime(1));
    EXPECT_TRUE(table.is_prime(2));
    EXPECT_TRUE(table.is_prime(3));
    EXPECT_FALSE(table.is_prime(4));
    EXPECT_TRUE(table.is_prime(5));
    EXPECT_FALSE(table.is_prime(9));
    EXPECT_TRUE(table.is_prime(97));
    EXPECT_FALSE(table.is_prime(100));

    EXPECT_EQ(table.factorize(12), (std::vector<i32>{2, 2, 3}));
    EXPECT_EQ(table.factorize(97), (std::vector<i32>{97}));
    EXPECT_EQ(table.factorize(100), (std::vector<i32>{2, 2, 5, 5}));
}

TEST(PrimeTest, IsPrimeLarge) {
    EXPECT_FALSE(is_prime(0));
    EXPECT_FALSE(is_prime(1));
    EXPECT_TRUE(is_prime(2));
    EXPECT_TRUE(is_prime(3));
    EXPECT_FALSE(is_prime(4));
    
    // Mersenne prime 2^31 - 1
    EXPECT_TRUE(is_prime(2147483647));
    
    // Mersenne prime 2^61 - 1
    EXPECT_TRUE(is_prime(2305843009213693951ull));
    
    // Composite
    EXPECT_FALSE(is_prime(2147483647ull * 3));
    
    // Carmichael numbers (pseudo-primes)
    EXPECT_FALSE(is_prime(561));
    EXPECT_FALSE(is_prime(1105));
    EXPECT_FALSE(is_prime(1729));
    EXPECT_FALSE(is_prime(2465));
    EXPECT_FALSE(is_prime(2821));
    EXPECT_FALSE(is_prime(6601));
    EXPECT_FALSE(is_prime(8911));
}

TEST(PrimeTest, FactorizeLarge) {
    EXPECT_EQ(factorize(12), (std::vector<u64>{2, 2, 3}));
    EXPECT_EQ(factorize(2147483647), (std::vector<u64>{2147483647}));
    
    // 2^61 - 1 is prime
    EXPECT_EQ(factorize(2305843009213693951ull), (std::vector<u64>{2305843009213693951ull}));
    
    // 10^18
    std::vector<u64> f10_18;
    for (int i = 0; i < 18; ++i) f10_18.push_back(2);
    for (int i = 0; i < 18; ++i) f10_18.push_back(5);
    EXPECT_EQ(factorize(1000000000000000000ull), f10_18);
    
    // Product of two large primes
    // 1000000007 and 1000000009 (twin primes)
    u64 p1 = 1000000007;
    u64 p2 = 1000000009;
    EXPECT_EQ(factorize(p1 * p2), (std::vector<u64>{p1, p2}));
}

TEST(PrimeTest, ExhaustiveCheckSmall) {
    auto naive_is_prime = [](i32 x) {
        if (x <= 1) return false;
        for (i32 i = 2; i * i <= x; ++i) {
            if (x % i == 0) return false;
        }
        return true;
    };
    
    auto naive_factorize = [](i32 x) {
        std::vector<i32> res;
        for (i32 i = 2; i * i <= x; ++i) {
            while (x % i == 0) {
                res.push_back(i);
                x /= i;
            }
        }
        if (x > 1) res.push_back(x);
        return res;
    };

    LpfTable lpf(1000);
    LinearLpfTable linear_lpf(1000);
    
    for (i32 i = 0; i <= 1000; ++i) {
        bool expected = naive_is_prime(i);
        EXPECT_EQ(lpf.is_prime(i), expected) << "LpfTable failed at " << i;
        EXPECT_EQ(linear_lpf.is_prime(i), expected) << "LinearLpfTable failed at " << i;
        EXPECT_EQ(is_prime(i), expected) << "is_prime failed at " << i;
        
        if (i > 0) {
            auto expected_factors = naive_factorize(i);
            EXPECT_EQ(lpf.factorize(i), expected_factors) << "LpfTable factorize failed at " << i;
            EXPECT_EQ(linear_lpf.factorize(i), expected_factors) << "LinearLpfTable factorize failed at " << i;
            
            std::vector<u64> expected_factors_u64(expected_factors.begin(), expected_factors.end());
            EXPECT_EQ(factorize(i), expected_factors_u64) << "factorize failed at " << i;
        }
    }
}
