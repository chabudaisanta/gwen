---
data:
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    links: []
  dependencies:
  - files: []
    type: Depends on
  - files: []
    type: Required by
  - files: []
    type: Verified with
  dependsOn: []
  embedded:
  - code: "#pragma once\n\n#include <cstddef>\n#include <cstdint>\n\nnamespace gwen\
      \ {\n\n// \u7B26\u53F7\u3042\u308A\u6574\u6570\u578B\nusing i32 = std::int32_t;\n\
      using i64 = std::int64_t;\nusing i128 = __int128_t;\n\n// \u7B26\u53F7\u306A\
      \u3057\u6574\u6570\u578B\nusing u32 = std::uint32_t;\nusing u64 = std::uint64_t;\n\
      using u128 = __uint128_t;\n\n// \u30DD\u30A4\u30F3\u30BF\u30B5\u30A4\u30BA\u30FB\
      \u30B3\u30F3\u30C6\u30CA\u30B5\u30A4\u30BA\u578B\nusing usize = std::size_t;\n\
      using isize = std::ptrdiff_t;\n\n// \u6D6E\u52D5\u5C0F\u6570\u70B9\u578B\nusing\
      \ f32 = float;\nusing f64 = double;\nusing f80 = long double;\n\n// \u30E6\u30FC\
      \u30B6\u30FC\u5B9A\u7FA9\u30EA\u30C6\u30E9\u30EB\nnamespace literals {\n\nconstexpr\
      \ i64 operator\"\"_i64(unsigned long long n) { return static_cast<i64>(n); }\n\
      constexpr u64 operator\"\"_u64(unsigned long long n) { return static_cast<u64>(n);\
      \ }\nconstexpr usize operator\"\"_zu(unsigned long long n) { return static_cast<usize>(n);\
      \ }\n\n}  // namespace literals\n\n}  // namespace gwen\n"
    name: default
  - code: "#line 2 \"include/gwen/types.hpp\"\n\n#include <cstddef>\n#include <cstdint>\n\
      \nnamespace gwen {\n\n// \u7B26\u53F7\u3042\u308A\u6574\u6570\u578B\nusing i32\
      \ = std::int32_t;\nusing i64 = std::int64_t;\nusing i128 = __int128_t;\n\n//\
      \ \u7B26\u53F7\u306A\u3057\u6574\u6570\u578B\nusing u32 = std::uint32_t;\nusing\
      \ u64 = std::uint64_t;\nusing u128 = __uint128_t;\n\n// \u30DD\u30A4\u30F3\u30BF\
      \u30B5\u30A4\u30BA\u30FB\u30B3\u30F3\u30C6\u30CA\u30B5\u30A4\u30BA\u578B\nusing\
      \ usize = std::size_t;\nusing isize = std::ptrdiff_t;\n\n// \u6D6E\u52D5\u5C0F\
      \u6570\u70B9\u578B\nusing f32 = float;\nusing f64 = double;\nusing f80 = long\
      \ double;\n\n// \u30E6\u30FC\u30B6\u30FC\u5B9A\u7FA9\u30EA\u30C6\u30E9\u30EB\
      \nnamespace literals {\n\nconstexpr i64 operator\"\"_i64(unsigned long long\
      \ n) { return static_cast<i64>(n); }\nconstexpr u64 operator\"\"_u64(unsigned\
      \ long long n) { return static_cast<u64>(n); }\nconstexpr usize operator\"\"\
      _zu(unsigned long long n) { return static_cast<usize>(n); }\n\n}  // namespace\
      \ literals\n\n}  // namespace gwen\n"
    name: bundled
  isFailed: false
  isVerificationFile: false
  path: include/gwen/types.hpp
  pathExtension: hpp
  requiredBy: []
  timestamp: '2026-07-13 21:57:44+09:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: include/gwen/types.hpp
layout: document
---
