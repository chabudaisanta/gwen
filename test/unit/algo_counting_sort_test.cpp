#include "gwen/algo/counting_sort.hpp"
#include <gtest/gtest.h>
#include <vector>

using namespace gwen;

TEST(CountingSortTest, Stability) {
    // Pairs of (value, id) to check stability
    std::vector<std::pair<i32, i32>> a = {
        {3, 0}, {1, 1}, {2, 2}, {3, 3}, {1, 4}, {5, 5}, {2, 6}
    };
    
    // We only pass the values to counting_sort
    std::vector<i32> values(a.size());
    i32 max_val = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        values[i] = a[i].first;
        if (a[i].first > max_val) {
            max_val = a[i].first;
        }
    }

    auto p = counting_sort(values, max_val);
    
    ASSERT_EQ(p.size(), a.size());

    // Check sorted order and stability
    for (size_t i = 1; i < p.size(); ++i) {
        auto prev = a[p[i - 1]];
        auto curr = a[p[i]];
        
        EXPECT_TRUE(prev.first <= curr.first);
        if (prev.first == curr.first) {
            EXPECT_TRUE(prev.second < curr.second); // Stability check
        }
    }
}
