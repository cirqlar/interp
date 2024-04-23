cmake_minimum_required(VERSION 3.28)

project(interp-dev)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_library(interp_compiler_flags INTERFACE)
add_subdirectory(src)

target_compile_options(interp_compiler_flags INTERFACE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wextra -Wpedantic>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)
target_link_libraries(interp_exe PRIVATE interp_compiler_flags)
target_link_libraries(interp_parser PUBLIC interp_compiler_flags)
target_link_libraries(interp_lexer PUBLIC interp_compiler_flags)
target_link_libraries(interp_repl PUBLIC interp_compiler_flags)


# ******** TESTING ********
enable_testing()
add_subdirectory(tests)
