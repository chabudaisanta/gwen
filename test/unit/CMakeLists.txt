# test/unit/CMakeLists.txt
# ここに単体テストのコードを追加していきます

add_executable(test_types test_types.cpp)
target_link_libraries(test_types gwen gtest_main)
include(GoogleTest)
gtest_discover_tests(test_types)
