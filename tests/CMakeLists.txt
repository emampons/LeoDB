cmake_minimum_required(VERSION 3.14)

add_executable(test_operators test_operators.cpp)

target_link_libraries(test_operators PUBLIC leodb gtest)
