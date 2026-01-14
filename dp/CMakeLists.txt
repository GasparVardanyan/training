cmake_minimum_required (VERSION 4.1)
project (training)
include (/desktop/nvim/cmake/config.cmake)
add_compile_options (-Wall -Wextra -Werror -Wno-unused-parameter)

add_executable (1_crtp 1_crtp.cpp)
add_executable (2_mixin_inheritance 2_mixin_inheritance.cpp)
add_executable (3_static_polymorphism 3_static_polymorphism.cpp)
