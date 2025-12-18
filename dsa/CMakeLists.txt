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
add_executable (7_references 7_references.cpp verbose_class.h)
target_compile_options (7_references PRIVATE -fno-elide-constructors)
add_executable (8_big_five 8_big_five.cpp verbose_class.h)
target_compile_options (8_big_five PRIVATE -fno-elide-constructors)
add_executable (9_function_templates 9_function_templates.cpp)
add_executable (10_class_templates 10_class_templates.cpp)
add_executable (11_object_and_comparable 11_object_and_comparable.cpp)
add_executable (12_function_object 12_function_object.cpp)
add_executable ( 13_separate_compilation_of_class_templates
	13_separate_compilation_of_class_templates.cpp
	13_MemoryCell1.h
	13_MemoryCell2.h
	13_MemoryCell2.cpp
)




find_package(GTest REQUIRED)

enable_testing ()

add_executable (hello_test hello_test.cpp)

target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
