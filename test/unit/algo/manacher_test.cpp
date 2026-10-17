#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "gwen/algo/manacher.hpp"
#include "gwen/types.hpp"

using namespace gwen;

TEST(ManacherTest, EmptyAndSingleElement) {
    EXPECT_EQ(manacher(std::string{}), (std::pair{std::vector<i32>{}, std::vector<i32>{}}));
    EXPECT_EQ(manacher(std::string{"x"}), (std::pair{std::vector<i32>{1}, std::vector<i32>{0}}));
}

TEST(ManacherTest, ChecksEveryCenterAgainstNaiveExpansion) {
    for (i32 length = 0; length <= 7; ++length) {
        i32 count = 1;
        for (i32 j = 0; j < length; ++j) count *= 3;
        for (i32 mask = 0; mask < count; ++mask) {
            std::vector<i32> values(length);
            i32 code = mask;
            for (i32& value : values) {
                value = code % 3;
                code /= 3;
            }
            const auto [odd, even] = manacher(values);
            for (i32 i = 0; i < length; ++i) {
                i32 expected_odd = 1;
                while (expected_odd <= i && expected_odd < length - i &&
                       values[i - expected_odd] == values[i + expected_odd]) {
                    ++expected_odd;
                }
                i32 expected_even = 0;
                while (expected_even < i && expected_even < length - i &&
                       values[i - expected_even - 1] == values[i + expected_even]) {
                    ++expected_even;
                }
                EXPECT_EQ(odd[i], expected_odd) << "mask=" << mask << " i=" << i;
                EXPECT_EQ(even[i], expected_even) << "mask=" << mask << " i=" << i;
            }
        }
    }
}
