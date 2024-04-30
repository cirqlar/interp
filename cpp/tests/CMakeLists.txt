cmake_minimum_required(VERSION 3.28)

add_subdirectory(../vendor/googletest tests/googletest)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(
  lexer_test
  lexer/lexer_test.cpp
)
target_link_libraries(
  lexer_test
  GTest::gtest_main interp_lexer
)

add_executable(
  ast_test
  parser/ast_test.cpp
)
target_link_libraries(
  ast_test
  GTest::gtest_main interp_parser
)

add_executable(
  parser_test
  parser/parser_test.cpp
)
target_link_libraries(
  parser_test
  GTest::gtest_main interp_parser
)

add_executable(
  eval_test
  parser/eval_test.cpp
)
target_link_libraries(
  eval_test
  GTest::gtest_main interp_parser
)

set_target_properties(lexer_test ast_test parser_test eval_test
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON	
)

include(GoogleTest)
gtest_discover_tests(lexer_test)
gtest_discover_tests(ast_test)
gtest_discover_tests(parser_test)
gtest_discover_tests(eval_test)

# add_library(interp_parser STATIC parser.cpp ast.cpp)
# target_include_directories(interp_parser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
# target_link_libraries(interp_parser PUBLIC interp_lexer)
