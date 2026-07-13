#include <gtest/gtest.h>

#include <type_traits>

#include "gwen/types.hpp"

using namespace gwen;
using namespace gwen::literals;

TEST(TypesTest, Aliases) {
    EXPECT_TRUE((std::is_same_v<i32, std::int32_t>));
    EXPECT_TRUE((std::is_same_v<i64, std::int64_t>));
    EXPECT_TRUE((std::is_same_v<u32, std::uint32_t>));
    EXPECT_TRUE((std::is_same_v<u64, std::uint64_t>));
    EXPECT_TRUE((std::is_same_v<usize, std::size_t>));
    EXPECT_TRUE((std::is_same_v<isize, std::ptrdiff_t>));
}

TEST(TypesTest, Literals) {
    auto val_i64 = 42_i64;
    EXPECT_TRUE((std::is_same_v<decltype(val_i64), i64>));
    EXPECT_EQ(val_i64, 42);

    auto val_u64 = 100_u64;
    EXPECT_TRUE((std::is_same_v<decltype(val_u64), u64>));
    EXPECT_EQ(val_u64, 100);

    auto val_zu = 12345_zu;
    EXPECT_TRUE((std::is_same_v<decltype(val_zu), usize>));
    EXPECT_EQ(val_zu, 12345);
}
