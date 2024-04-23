cmake_minimum_required(VERSION 3.28)

add_library(interp_repl STATIC repl.cpp)
target_include_directories(interp_repl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(interp_repl PUBLIC interp_lexer interp_parser)

set_target_properties(interp_repl
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON	
)
