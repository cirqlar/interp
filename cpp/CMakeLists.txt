cmake_minimum_required(VERSION 3.15)

project(interp)
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

add_library(interp_compiler_flags INTERFACE)
target_compile_features(interp_compiler_flags INTERFACE cxx_std_20)
target_include_directories(interp_compiler_flags INTERFACE "${PROJECT_SOURCE_DIR}")

add_executable(
  interp
  src/main.cpp src/lexer/lexer.cpp src/shared/token.cpp src/shared/ast.cpp src/parser/parser.cpp
)
target_link_libraries(interp PUBLIC interp_compiler_flags)

# ******** TESTING ********

add_subdirectory(vendor/googletest)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(
  lexer_test
  src/lexer/lexer_test.cpp src/lexer/lexer.cpp src/shared/token.cpp
)
target_link_libraries(
  lexer_test
  GTest::gtest_main interp_compiler_flags
)

add_executable(
  ast_test
  src/shared/ast_test.cpp src/shared/ast.cpp src/shared/token.cpp
)
target_link_libraries(
  ast_test
  GTest::gtest_main interp_compiler_flags
)

include(GoogleTest)
gtest_discover_tests(lexer_test)
gtest_discover_tests(ast_test)

