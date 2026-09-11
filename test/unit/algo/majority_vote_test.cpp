#include <gtest/gtest.h>

#include "gwen/algo/majority_vote.hpp"

using namespace gwen;

namespace {

struct NoDefault {
    i32 value;

    NoDefault() = delete;
    explicit NoDefault(i32 value) : value(value) {}
    bool operator==(const NoDefault&) const = default;
};

}  // namespace

TEST(MajorityVoteTest, FindsMajority) {
    const std::vector<i32> values = {1, 2, 2, 2, 3};
    const auto [exists, value] = majority_vote(values);
    EXPECT_TRUE(exists);
    EXPECT_EQ(value, 2);
}

TEST(MajorityVoteTest, RejectsAbsentMajority) {
    const std::vector<i32> values = {1, 2, 3, 2, 4};
    const auto [exists, value] = majority_vote(values);
    EXPECT_FALSE(exists);
    (void)value;
}

TEST(MajorityVoteTest, HandlesZeroRemainingCandidateCount) {
    const std::vector<i32> values = {1, 2};
    EXPECT_FALSE(majority_vote(values).first);
}

TEST(MajorityVoteTest, DoesNotRequireDefaultConstruction) {
    const std::vector<NoDefault> values = {NoDefault(1), NoDefault(2), NoDefault(2)};
    const auto [exists, value] = majority_vote(values);
    EXPECT_TRUE(exists);
    EXPECT_EQ(value.value, 2);
}

#ifndef NDEBUG
TEST(MajorityVoteTest, RejectsEmptyInput) {
    const std::vector<i32> values;
    ASSERT_DEATH((void)majority_vote(values), "");
}
#endif
