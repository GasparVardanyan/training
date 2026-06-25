cmake_minimum_required (VERSION 4.1)
project (training)
include (/desktop/nvim/cmake/config.cmake)
add_compile_options (-Wall -Wextra -Werror)

find_package (TBB REQUIRED)

add_executable (pre_1_barrier pre_1_barrier.cpp)
add_executable (pre_2_dead_lock pre_2_dead_lock.cpp)

add_executable (pre_3_execution_policies pre_3_execution_policies.cpp)
target_link_libraries (pre_3_execution_policies PRIVATE TBB::tbb)

add_executable (pre_4_projections pre_4_projections.cpp)

add_executable (1_batch_operations 1_batch_operations.cpp)
target_link_libraries (1_batch_operations PRIVATE TBB::tbb)

add_executable (1_batch_operations_for_each 1_batch_operations_for_each.cpp)

add_executable (2_search_operations 2_search_operations.cpp)
target_link_libraries (2_search_operations PRIVATE TBB::tbb)
