#include <gtest/gtest.h>
#include "testlib.h"

// gtest_discover_tests で --gtest_list_tests が呼ばれた際にも
// testlib の終了時チェックを無効化するため、グローバルで初期化する
int dummy_disable = []() { disableFinalizeGuard(); return 0; }();

TEST(TestLibDummyTest, DummyTest) {
    rnd.setSeed(12345);
}
