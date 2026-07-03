# Compile each problem solver as an executable
# <<< GENERATED SOLUTIONS START >>>
add_executable(problem_factorize problems/factorize/solution.cpp)
target_link_libraries(problem_factorize PRIVATE gwen)

add_executable(problem_lca problems/lca/solution.cpp)
target_link_libraries(problem_lca PRIVATE gwen)

# <<< GENERATED SOLUTIONS END >>>

# Compile Google Test executable
add_executable(gwen_tests
  unit/test_factorize.cpp
  unit/test_mdarray.cpp
  unit/test_rolling_hash.cpp
  unit/test_polynomial_difference_array.cpp
  problem_runner/test_problems.cpp
)

target_link_libraries(gwen_tests PRIVATE
  gwen
  gtest_main
)

# Define PROJECT_ROOT and BUILD_DIR macros
target_compile_definitions(gwen_tests PRIVATE
  PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
  BUILD_DIR="${CMAKE_BINARY_DIR}"
)

# Enable CTest integration
include(GoogleTest)
gtest_discover_tests(gwen_tests)
