cmake_minimum_required(VERSION 3.14)

add_executable(test_operators test_operators.cpp)
add_executable(test_key test_key.cpp)
add_executable(test_value test_value.cpp)

target_link_libraries(test_operators PUBLIC leodb gtest)
target_link_libraries(test_key PUBLIC leodb gtest)
target_link_libraries(test_value PUBLIC leodb gtest)