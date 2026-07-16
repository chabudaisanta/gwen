// clang-format off
#include "gwen/graph/weighted_dsu.hpp"
// clang-format on

#include <gtest/gtest.h>

namespace gwen {

struct IntAddAbel {
    using S = int;
    static S op(S a, S b) { return a + b; }
    static S e() { return 0; }
    static S inv(S a) { return -a; }
};

TEST(GraphWeightedDsuTest, BasicOperations) {
    WeightedDsu<IntAddAbel> dsu(5);

    EXPECT_EQ(dsu.size(0), 1);
    EXPECT_TRUE(dsu.same(0, 0));
    EXPECT_FALSE(dsu.same(0, 1));

    // P(1) - P(0) = 5
    EXPECT_EQ(dsu.merge(0, 1, 5), 0); // Assuming it returns the leader which is likely 0

    EXPECT_TRUE(dsu.same(0, 1));
    EXPECT_EQ(dsu.diff(1, 0), -5);
    // Let's verify diff definition. diff(a,b) = P(a) - P(b).
    // merge(a,b,w) => P(a) - P(b) = w
    // Oh, diff(1,0) should be -5 because P(1) - P(0) = -w?
    // Wait, merge(0,1,5) means P(0) - P(1) = 5. So P(1) - P(0) = -5.
    EXPECT_EQ(dsu.diff(0, 1), 5);

    // P(2) - P(1) = 10
    EXPECT_EQ(dsu.merge(1, 2, 10), 0);
    EXPECT_TRUE(dsu.same(0, 2));
    
    // P(0) - P(1) = 5, P(1) - P(2) = 10 => P(0) - P(2) = 15
    EXPECT_EQ(dsu.diff(0, 2), 15);

    auto groups = dsu.groups();
    EXPECT_EQ(groups.size(), 3); // {0,1,2}, {3}, {4}
}

} // namespace gwen
