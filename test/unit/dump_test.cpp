#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <vector>

// DUMP_SIZE_LIMIT を定義してから dump.hpp を include
#define DUMP_SIZE_LIMIT 3
#define LOCAL // DUMP マクロを有効化
#include "gwen/dump.hpp"
#include "gwen/ds/fenwick_tree.hpp"
#include "gwen/alge/abel.hpp"

using namespace gwen;

TEST(DumpTest, RangeFormatWithLimit) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::string s = internal::format_range(v);
    // limit は 3 なので [1, 2, 3, ... , 5] になるはず
    EXPECT_EQ(s, "[1, 2, 3, ... , 5]");
}

TEST(DumpTest, RangeFormatWithoutLimitExceeded) {
    std::vector<int> v = {1, 2, 3};
    std::string s = internal::format_range(v);
    // limit は 3。サイズ <= 3 + 1 なのでそのまま
    EXPECT_EQ(s, "[1, 2, 3]");
}

TEST(DumpTest, RangeFormatLimitPlusOne) {
    std::vector<int> v = {1, 2, 3, 4};
    std::string s = internal::format_range(v);
    // size = 4, limit + 1 と等しいのでそのまま ( ... , 4 は不要)
    EXPECT_EQ(s, "[1, 2, 3, 4]");
}

TEST(DumpTest, FenwickTreeDump) {
    FenwickTree<sum_abel<int>> ft(5);
    ft.add(0, 10);
    ft.add(2, 20);
    ft.add(4, 30);
    
    std::string ds = ft.dump();
    // ft の復元配列は [10, 0, 20, 0, 30]
    // limit 3 なので [10, 0, 20, ... , 30] が含まれるはず
    EXPECT_NE(ds.find("[10, 0, 20, ... , 30]"), std::string::npos);
}

#include "gwen/bbst/implicit_treap.hpp"
#include "gwen/bbst/sorted_treap.hpp"

TEST(DumpTest, TreapDump) {
    ImplicitTreap<int> it(std::vector<int>{10, 20, 30, 40, 50});
    std::string its = it.dump();
    EXPECT_NE(its.find("[10, 20, 30, ... , 50]"), std::string::npos);

    SortedTreap<int> st;
    st.insert(10);
    st.insert(30);
    st.insert(20);
    st.insert(50);
    st.insert(40);
    std::string sts = st.dump();
    EXPECT_NE(sts.find("[10, 20, 30, ... , 50]"), std::string::npos);
}
