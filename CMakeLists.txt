cmake_minimum_required(VERSION 3.6)
project(leodb)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_SOURCE_DIR .)
add_executable(${PROJECT_NAME} main.cpp)

# Add Loguru
include_directories(${PROJECT_SOURCE_DIR}/external/loguru)

# Add our DBMS directory (i.e. /src)
include_directories(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

# Target our linked Libraries
target_link_libraries(${PROJECT_NAME})

if (${BUILD_TESTS} MATCHES "1")
    # Install Google Tests
    add_subdirectory(external/googletest)
    enable_testing()
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    macro(package_add_test TESTNAME TESTSOURCE)
        add_executable(${TESTNAME} ${TESTSOURCE})
        target_link_libraries(${TESTNAME} gtest gtest_main)
    endmacro()

    package_add_test(test_operators tests/test_operators.cpp)
    package_add_test(test_key tests/test_key.cpp)
    package_add_test(test_value tests/test_value.cpp)

    # Install Google Benchmark
    add_subdirectory(external/googlebenchmark)
    macro(package_add_benchmark TESTNAME TESTSOURCE)
        add_executable(${TESTNAME} ${TESTSOURCE})
        target_link_libraries(${TESTNAME} benchmark::benchmark)
    endmacro()
    package_add_benchmark(test_benchmark tests/benchmark/test_benchmark.cpp)

endif()