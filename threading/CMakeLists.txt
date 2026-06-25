cmake_minimum_required(VERSION 3.5.0)

set(VERSION "0.0.1")
set(PROJECT_NAME "training")

include(/desktop/nvim/cmake/config.cmake)

project(${PROJECT_NAME} VERSION ${VERSION} LANGUAGES CXX)

add_executable(1_hello 1_hello.cpp)
add_executable(2_reference_wrapper 2_reference_wrapper.cpp)
add_executable(3_passing_references 3_passing_references.cpp)
add_executable(4_async_calls 4_async_calls.cpp)
add_executable(5_basic_synchronization 5_basic_synchronization.cpp)
add_executable(6_always_joined 6_always_joined.cpp)
add_executable(7_current_thread_operations 7_current_thread_operations.cpp)
add_executable(8_condition_variables 8_condition_variables.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(CTest)
enable_testing()
