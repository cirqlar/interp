cmake_minimum_required(VERSION 3.15)

project(interp)
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

add_library(interp_compiler_flags INTERFACE)
target_compile_features(interp_compiler_flags INTERFACE cxx_std_20)
target_include_directories(interp_compiler_flags INTERFACE "${PROJECT_SOURCE_DIR}")
target_compile_options(interp_compiler_flags INTERFACE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wextra -Wpedantic>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)

add_executable(
  interp
  src/main.cpp src/lexer/lexer.cpp src/shared/token.cpp src/shared/ast.cpp src/parser/parser.cpp src/repl/repl.cpp
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

add_executable(
  parser_test
  src/parser/parser_test.cpp src/parser/parser.cpp src/shared/token.cpp src/lexer/lexer.cpp src/shared/ast.cpp
)
target_link_libraries(
  parser_test
  GTest::gtest_main interp_compiler_flags
)

include(GoogleTest)
gtest_discover_tests(lexer_test)
gtest_discover_tests(ast_test)
gtest_discover_tests(parser_test)

