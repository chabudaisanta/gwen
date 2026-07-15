// clang-format off
#include "gwen/utils/timer.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

namespace gwen {

TEST(TimerTest, MethodCallTest) {
    EXPECT_LE(timer.elapsed<Timer::ms>(), Timer::ms(20));
    EXPECT_LE(timer.delta<Timer::ms>(), Timer::ms(20));
    EXPECT_LE(timer.lap<Timer::ms>(), Timer::ms(20));

    timer.set_limit(Timer::s(1));
    EXPECT_FALSE(timer.expired());

    timer.set_limit(Timer::ms(1));
    std::this_thread::sleep_for(Timer::ms(20));
    EXPECT_TRUE(timer.expired());
    timer.restart(Timer::ms(5));
    EXPECT_FALSE(timer.expired());
    EXPECT_TRUE(timer.expired_elapsed());

    std::this_thread::sleep_for(Timer::ms(20));
    EXPECT_TRUE(timer.expired());
    timer.reset();
    EXPECT_FALSE(timer.expired());

    EXPECT_TRUE(timer.dump().size());
}

TEST(TimerTest, DumpTest) {
    Timer timer;
    timer.restart(Timer::s(2));
    std::string d = timer.dump();

    // 標準エラー出力に出してログに残す
    std::cerr << "[DEBUG OUTPUT]\n" << d << std::endl;

    // limit が正しくフォーマットされているかテスト
    EXPECT_TRUE(d.find("Timer{") != std::string::npos);
    EXPECT_TRUE(d.find("limit: 2000ms") != std::string::npos);
}

}  // namespace gwen