#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "gwen/automaton/aho_corasick.hpp"
#include "gwen/types.hpp"

using namespace gwen;

TEST(AhoCorasickTest, EmptyDictionary) {
    const std::vector<std::vector<i32>> patterns;
    const auto a = automaton::aho_corasick<3>(patterns);
    ASSERT_TRUE(a.valid());
    EXPECT_EQ(a.n, 1);
    EXPECT_EQ(a.init, std::vector<i32>{0});
    EXPECT_TRUE(a.accept.empty());
    for (i32 c = 0; c < 3; ++c) EXPECT_EQ(a.edge(0, c), 0);
}

TEST(AhoCorasickTest, MatchesEveryPrefixAgainstNaiveSearch) {
    const std::vector<std::vector<i32>> patterns = {{0, 1}, {1}, {1, 0, 1}, {0, 1}};
    const auto a = automaton::aho_corasick<3>(patterns);
    ASSERT_TRUE(a.valid());
    ASSERT_EQ(a.init, std::vector<i32>{0});
    std::vector<bool> accepting(a.n);
    for (i32 state : a.accept) accepting[state] = true;

    for (i32 length = 0; length <= 6; ++length) {
        i32 count = 1;
        for (i32 j = 0; j < length; ++j) count *= 3;
        for (i32 mask = 0; mask < count; ++mask) {
            std::vector<i32> text(length);
            i32 value = mask;
            for (i32& c : text) {
                c = value % 3;
                value /= 3;
            }
            i32 state = 0;
            for (i32 end = 1; end <= length; ++end) {
                state = a.edge(state, text[end - 1]);
                ASSERT_GE(state, 0);
                bool expected = false;
                for (const auto& pattern : patterns) {
                    if (static_cast<i32>(pattern.size()) <= end &&
                        std::equal(pattern.begin(), pattern.end(), text.begin() + end - pattern.size())) {
                        expected = true;
                    }
                }
                EXPECT_EQ(accepting[state], expected) << "mask=" << mask << " end=" << end;
            }
        }
    }
}

#ifndef NDEBUG
TEST(AhoCorasickTest, RejectsEmptyPattern) {
    const std::vector<std::vector<i32>> patterns = {{}};
    ASSERT_DEATH((void)automaton::aho_corasick<2>(patterns), "");
}
#endif
