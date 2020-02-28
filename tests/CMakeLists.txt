cmake_minimum_required(VERSION 3.6)
project(leodb-tests)
file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_library(leodb-tests STATIC ${SOURCE_FILES})
