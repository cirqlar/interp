cmake_minimum_required(VERSION 3.28)

add_library(interp_lexer STATIC lexer.cpp token.cpp)
target_include_directories(interp_lexer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(interp_lexer
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON	
)