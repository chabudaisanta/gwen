cmake_minimum_required(VERSION 3.14)
project(gwen CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ライブラリ本体 (ヘッダーオンリー)
add_library(gwen INTERFACE)
target_include_directories(gwen INTERFACE include)

# テスト
enable_testing()

# CMP0135 警告の抑制 (アーカイブ抽出時のタイムスタンプの扱い)
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(test/unit)
