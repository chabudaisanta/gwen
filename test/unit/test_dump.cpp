// clang-format off
#define LOCAL
#include "gwen/dump.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <set>
#include <utility>
#include <tuple>

namespace gwen {

// ダミーのダンプ可能な型
struct MyDumpable {
    std::string dump() const { return "dumpable_value"; }
};

// ダミーの値フォーマット可能な型 (modintなどを想定)
struct MyFormattable {
    int val() const { return 42; }
};

// フォーマット条件を一切満たさない型
struct MyUnformattable {};

TEST(DumpTest, BasicTypes) {
    testing::internal::CaptureStderr();
    dump(1, 2.5, "hello");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "1, 2.5, hello\n");
}

TEST(DumpTest, CustomTypes) {
    MyDumpable d;
    MyFormattable f;

    testing::internal::CaptureStderr();
    dump(d, f);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "dumpable_value, 42\n");
}

TEST(DumpTest, MacroOutput) {
    testing::internal::CaptureStderr();
    int x = 10;
    std::string str = "test";
    DUMP(x, str);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "x, str: 10, test\n");
}

TEST(DumpTest, MacroLongOutput) {
    testing::internal::CaptureStderr();
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    // 変数名リストが15文字以上になるようにする
    DUMP(a, b, c, d, e, a, b, c, d, e);
    std::string output = testing::internal::GetCapturedStderr();
    // 15文字以上の場合は改行が入るか確認
    EXPECT_EQ(output, "a, b, c, d, e, a, b, c, d, e: \n    1, 2, 3, 4, 5, 1, 2, 3, 4, 5\n");
}

TEST(DumpTest, ContainerOutput) {
    testing::internal::CaptureStderr();
    std::vector<int> v = {1, 2, 3};
    std::set<std::string> s = {"a", "b"};
    std::pair<int, double> p = {1, 2.5};
    std::tuple<int, std::string, double> t = {1, "test", 3.14};
    
    DUMP(v, s, p, t);
    std::string output = testing::internal::GetCapturedStderr();
    
    // C++23 の std::format の仕様に基づく出力形式
    EXPECT_TRUE(output.find("[1, 2, 3]") != std::string::npos);
    EXPECT_TRUE(output.find("\"a\"") != std::string::npos);
    EXPECT_TRUE(output.find("\"b\"") != std::string::npos);
    EXPECT_TRUE(output.find("(1, 2.5)") != std::string::npos);
    EXPECT_TRUE(output.find("(1, \"test\", 3.14)") != std::string::npos);
}

TEST(DumpTest, EmptyMacro) {
    testing::internal::CaptureStderr();
    DUMP();
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "empty dump called\n");
}

TEST(DumpTest, UnformattableOutput) {
    MyUnformattable u;
    testing::internal::CaptureStderr();
    DUMP(u);
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(output.find("[unformattable token]") != std::string::npos);
}

} // namespace gwen