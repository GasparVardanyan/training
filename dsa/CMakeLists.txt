cmake_minimum_required (VERSION 4.1)
project (cpptut)
include (/desktop/nvim/cmake/config.cmake)
add_compile_options (-Wall -Wextra -Werror)


add_executable (1_recursion 1_recursion.cpp)
add_executable (2_class 2_class.cpp)
add_executable (3_class 3_class.cpp 3_IntCell.cpp 3_IntCell.h)
add_executable (4_vector 4_vector.cpp)
add_executable (5_pointers 5_pointers.cpp)
add_executable (6_references 6_references.cpp)
add_executable (7_references 7_references.cpp)
