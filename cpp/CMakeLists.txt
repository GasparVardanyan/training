cmake_minimum_required (VERSION 4.1)
project (training)
include (/desktop/nvim/cmake/config.cmake)
add_compile_options (-Wall -Wextra -Werror -Wno-unused-parameter)

add_executable (1_hello_world 1_hello_world.cpp)
add_executable (2_function 2_function.cpp)
add_executable (3_literals 3_literals.cpp)
add_executable (4_operator_precedence 4_operator_precedence.cpp)
add_executable (5_unary_operators 5_unary_operators.cpp)
add_executable (6_floating_point_arithmetic 6_floating_point_arithmetic.cpp)
add_executable (7_bit_operators 7_bit_operators.cpp)
add_executable (8_bit_manipulation 8_bit_manipulation.cpp)
add_executable (9_popcount 9_popcount.cpp)
add_executable (10_bit_fields 10_bit_fields.cpp)
add_executable (11_arrays 11_arrays.cpp)
add_executable (12_iterators 12_iterators.cpp)
add_executable (13_iterators 13_iterators.cpp)
add_executable (14_loops 14_loops.cpp)
add_executable (15_loops 15_loops.cpp)
add_executable (16_file_io 16_file_io.cpp)
add_executable (17_file_io 17_file_io.cpp)
add_executable (18_streams 18_streams.cpp)
add_executable (19_stream_manipulators 19_stream_manipulators.cpp)
add_executable (20_flow_control 20_flow_control.cpp)

add_executable (
	21_metaprogramming_parameter_pack
	21_metaprogramming_parameter_pack.cpp
)
target_compile_options (
	21_metaprogramming_parameter_pack
	PRIVATE
	-Wno-c99-extensions
)

add_executable (22_metaprogramming_recursion 22_metaprogramming_recursion.cpp)

add_executable (
	23_metaprogramming_template_specialization
	23_metaprogramming_template_specialization.cpp
)
