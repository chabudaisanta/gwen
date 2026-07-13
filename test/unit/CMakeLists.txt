# test/unit/CMakeLists.txt

# GoogleTest用のマクロ(gtest_discover_tests)を使えるようにする
include(GoogleTest)

# --- テスト1: test_types ---

# "テストスイート" と呼ばれる、テストケースの集まりを作成します。
# ここでは、 test_types.cpp に書かれたテストを "TypesTest" という名前のスイートにまとめ、
# test_types という実行ファイル（ターゲット）にリンクしています。
add_executable(test_types test_types.cpp)

# 【target_link_libraries】
# 実行ファイルに必要な「ライブラリ（他のプログラムの塊）」を結合（リンク）するコマンドです。
target_link_libraries(test_types PRIVATE gwen gtest_main)

# 【gtest_discover_tests】
# 生成された実行ファイルの中身をCMakeが自動で解析（スキャン）し、
# コード内に書かれた `TEST()` や `TEST_F()` などの各テストケースを ctest に登録するコマンドです。
# これにより、テストを実行した際に「どのテストケースが成功/失敗したか」を個別に見分けられるようになります。
gtest_discover_tests(test_types)


# --- テスト2: test_timer ---
add_executable(test_timer test_timer.cpp)
target_link_libraries(test_timer PRIVATE gwen gtest_main)
gtest_discover_tests(test_timer)
