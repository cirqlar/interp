cmake_minimum_required(VERSION 3.15)

project(interp)
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

add_library(interp_compiler_flags INTERFACE)
target_compile_features(interp_compiler_flags INTERFACE cxx_std_20)

add_executable(interp src/main.cpp src/lexer/lexer.cpp src/shared/token.cpp)
target_include_directories(interp PUBLIC "${PROJECT_SOURCE_DIR}")
target_link_libraries(interp PUBLIC interp_compiler_flags)