cmake_minimum_required(VERSION 3.14)
project(leodb)
set(CMAKE_CXX_STANDARD 14)

set(leodb_lib
        src/leodb/main.cpp)

add_library(${PROJECT_NAME} ${leodb_lib})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

include_directories(external)

configure_file(external/CMakeLists.txt.in external/googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/external/googletest-download"
        )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/external/googletest-download"
        )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/external/googletest-src"
        "${CMAKE_BINARY_DIR}/external/googletest-build"
        )

add_subdirectory(misc)
add_subdirectory(tests)