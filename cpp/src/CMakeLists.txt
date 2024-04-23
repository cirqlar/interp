cmake_minimum_required(VERSION 3.28)

project(interp)

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(repl)

add_executable(interp_exe main.cpp)

target_include_directories(interp_parser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(interp_exe PUBLIC interp_lexer interp_parser interp_repl)

set_target_properties(interp_exe
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON	
)