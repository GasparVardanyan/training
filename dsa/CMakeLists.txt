cmake_minimum_required (VERSION 4.1)
project (cpptut)
include (/desktop/nvim/cmake/config.cmake)
add_compile_options (-Wall -Wextra -Werror)


add_executable (1_class 1_class.cpp)
add_executable (2_class 2_class.cpp 2_IntCell.cpp 2_IntCell.h)
add_executable (3_vector 3_vector.cpp)
add_executable (4_pointers 4_pointers.cpp)
add_executable (5_references 5_references.cpp)
add_executable (6_references 6_references.cpp)
