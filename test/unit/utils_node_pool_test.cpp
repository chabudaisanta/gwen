// clang-format off
#include "gwen/utils/node_pool.hpp"
// clang-format on

#include <gtest/gtest.h>

using namespace gwen;

struct MyNode {
    int val = 0;
    int left = 0;
    int right = 0;
};

TEST(NodePoolTest, BasicAndFree) {
    NodePool<MyNode> pool;
    
    EXPECT_EQ(pool.size(), 1); // dummy node
    
    MyNode n1{10, 0, 0};
    i32 id1 = pool.new_node(n1);
    EXPECT_EQ(id1, 1);
    EXPECT_EQ(pool.size(), 2);
    
    MyNode n2{20, 0, 0};
    i32 id2 = pool.new_node(n2);
    EXPECT_EQ(id2, 2);
    EXPECT_EQ(pool.size(), 3);
    
    pool.free_node(id1);
    EXPECT_EQ(pool.size(), 2);
    
    // The next new_node should reuse id1
    MyNode n3{30, 0, 0};
    i32 id3 = pool.new_node(n3);
    EXPECT_EQ(id3, 1);
    EXPECT_EQ(pool.size(), 3);
    
    // Check values
    EXPECT_EQ(pool[1].val, 30);
    EXPECT_EQ(pool[2].val, 20);
}
