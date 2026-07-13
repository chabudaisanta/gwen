# test/unit/CMakeLists.txt

# GoogleTest用のマクロ(gtest_discover_tests)を使えるようにする
include(GoogleTest)

# test/unit/ ディレクトリ内の .cpp ファイルをすべて取得
file(GLOB TEST_SOURCES "*.cpp")

# template.cpp.sample などを誤って .cpp 拡張子で保存した場合などに備え、template という名前のファイルは除外する
list(FILTER TEST_SOURCES EXCLUDE REGEX "template.*\\.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})
    # ファイル名から拡張子を除いた部分（例: test_types.cpp -> test_types）を取得
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # 実行ファイルの追加
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    # ライブラリのリンク
    target_link_libraries(${TEST_NAME} PRIVATE gwen gtest_main)

    # CTest への登録
    gtest_discover_tests(${TEST_NAME})
endforeach()
