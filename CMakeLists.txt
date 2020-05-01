cmake_minimum_required(VERSION 3.6)
project(leodb)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_SOURCE_DIR .)

# Set our output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add main.cpp as leoDB
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
#
#    # Add unit test files
#    package_add_test(test_operators tests/test_operators.cpp)
#    package_add_test(test_key tests/test_key.cpp)
#    package_add_test(test_value tests/test_value.cpp)

    # Install Google BenchmarkTests
    add_subdirectory(external/googlebenchmark)
    macro(package_add_benchmark TESTNAME TESTSOURCE)
        add_executable(${TESTNAME} ${TESTSOURCE})
        target_link_libraries(${TESTNAME} benchmark::benchmark)
    endmacro()

    # Add benchmark files
    # level
    # Put tests
    package_add_benchmark(benchmark_level_put_1000 tests/benchmark/level/benchmark_level_put_1000.cpp)
    package_add_benchmark(benchmark_level_put_5000 tests/benchmark/level/benchmark_level_put_5000.cpp)
    package_add_benchmark(benchmark_level_put_10000 tests/benchmark/level/benchmark_level_put_10000.cpp)
    package_add_benchmark(benchmark_level_put_25000 tests/benchmark/level/benchmark_level_put_25000.cpp)
    package_add_benchmark(benchmark_level_put_50000 tests/benchmark/level/benchmark_level_put_50000.cpp)
    package_add_benchmark(benchmark_level_put_100000 tests/benchmark/level/benchmark_level_put_100000.cpp)
    package_add_benchmark(benchmark_level_put_250000 tests/benchmark/level/benchmark_level_put_250000.cpp)

    # Put and get tests
    package_add_benchmark(benchmark_level_put_get_500 tests/benchmark/level/benchmark_level_put_get_500.cpp)
    package_add_benchmark(benchmark_level_put_get_1000 tests/benchmark/level/benchmark_level_put_get_1000.cpp)
    package_add_benchmark(benchmark_level_put_get_5000 tests/benchmark/level/benchmark_level_put_get_5000.cpp)
    package_add_benchmark(benchmark_level_put_get_10000 tests/benchmark/level/benchmark_level_put_get_10000.cpp)
    package_add_benchmark(benchmark_level_put_get_25000 tests/benchmark/level/benchmark_level_put_get_25000.cpp)

    # Put, get, put tests
    package_add_benchmark(benchmark_level_put_get_put_1000 tests/benchmark/level/benchmark_level_put_get_put_1000.cpp)
    package_add_benchmark(benchmark_level_put_get_put_5000 tests/benchmark/level/benchmark_level_put_get_put_5000.cpp)
    package_add_benchmark(benchmark_level_put_get_put_10000 tests/benchmark/level/benchmark_level_put_get_put_10000.cpp)

    # Put, get, put new tests
    package_add_benchmark(benchmark_level_put_get_put_new_1000 tests/benchmark/level/benchmark_level_put_get_put_new_1000.cpp)
    package_add_benchmark(benchmark_level_put_get_put_new_5000 tests/benchmark/level/benchmark_level_put_get_put_new_5000.cpp)
    package_add_benchmark(benchmark_level_put_get_put_new_10000 tests/benchmark/level/benchmark_level_put_get_put_new_10000.cpp)

    # tier
    # Put tests
    package_add_benchmark(benchmark_tier_put_1000 tests/benchmark/tier/benchmark_tier_put_1000.cpp)
    package_add_benchmark(benchmark_tier_put_5000 tests/benchmark/tier/benchmark_tier_put_5000.cpp)
    package_add_benchmark(benchmark_tier_put_10000 tests/benchmark/tier/benchmark_tier_put_10000.cpp)
    package_add_benchmark(benchmark_tier_put_25000 tests/benchmark/tier/benchmark_tier_put_25000.cpp)
    package_add_benchmark(benchmark_tier_put_50000 tests/benchmark/tier/benchmark_tier_put_50000.cpp)
    package_add_benchmark(benchmark_tier_put_100000 tests/benchmark/tier/benchmark_tier_put_100000.cpp)
    package_add_benchmark(benchmark_tier_put_250000 tests/benchmark/tier/benchmark_tier_put_250000.cpp)

    # Put and get tests
    package_add_benchmark(benchmark_tier_put_get_500 tests/benchmark/tier/benchmark_tier_put_get_500.cpp)
    package_add_benchmark(benchmark_tier_put_get_1000 tests/benchmark/tier/benchmark_tier_put_get_1000.cpp)
    package_add_benchmark(benchmark_tier_put_get_5000 tests/benchmark/tier/benchmark_tier_put_get_5000.cpp)
    package_add_benchmark(benchmark_tier_put_get_10000 tests/benchmark/tier/benchmark_tier_put_get_10000.cpp)
    package_add_benchmark(benchmark_tier_put_get_25000 tests/benchmark/tier/benchmark_tier_put_get_25000.cpp)

    # Put, get, put tests
    package_add_benchmark(benchmark_tier_put_get_put_1000 tests/benchmark/tier/benchmark_tier_put_get_put_1000.cpp)
    package_add_benchmark(benchmark_tier_put_get_put_5000 tests/benchmark/tier/benchmark_tier_put_get_put_5000.cpp)
    package_add_benchmark(benchmark_tier_put_get_put_10000 tests/benchmark/tier/benchmark_tier_put_get_put_10000.cpp)

    # Put, get, put new tests
    package_add_benchmark(benchmark_tier_put_get_put_new_1000 tests/benchmark/tier/benchmark_tier_put_get_put_new_1000.cpp)
    package_add_benchmark(benchmark_tier_put_get_put_new_5000 tests/benchmark/tier/benchmark_tier_put_get_put_new_5000.cpp)
    package_add_benchmark(benchmark_tier_put_get_put_new_10000 tests/benchmark/tier/benchmark_tier_put_get_put_new_10000.cpp)

    # any_level
    # Put tests
    package_add_benchmark(benchmark_any_level_put_1000 tests/benchmark/any_level/benchmark_any_level_put_1000.cpp)
    package_add_benchmark(benchmark_any_level_put_5000 tests/benchmark/any_level/benchmark_any_level_put_5000.cpp)
    package_add_benchmark(benchmark_any_level_put_10000 tests/benchmark/any_level/benchmark_any_level_put_10000.cpp)
    package_add_benchmark(benchmark_any_level_put_25000 tests/benchmark/any_level/benchmark_any_level_put_25000.cpp)
    package_add_benchmark(benchmark_any_level_put_50000 tests/benchmark/any_level/benchmark_any_level_put_50000.cpp)
    package_add_benchmark(benchmark_any_level_put_100000 tests/benchmark/any_level/benchmark_any_level_put_100000.cpp)
    package_add_benchmark(benchmark_any_level_put_250000 tests/benchmark/any_level/benchmark_any_level_put_250000.cpp)

    # Put and get tests
    package_add_benchmark(benchmark_any_level_put_get_500 tests/benchmark/any_level/benchmark_any_level_put_get_500.cpp)
    package_add_benchmark(benchmark_any_level_put_get_1000 tests/benchmark/any_level/benchmark_any_level_put_get_1000.cpp)
    package_add_benchmark(benchmark_any_level_put_get_5000 tests/benchmark/any_level/benchmark_any_level_put_get_5000.cpp)
    package_add_benchmark(benchmark_any_level_put_get_10000 tests/benchmark/any_level/benchmark_any_level_put_get_10000.cpp)
    package_add_benchmark(benchmark_any_level_put_get_25000 tests/benchmark/any_level/benchmark_any_level_put_get_25000.cpp)

    # Put, get, put tests
    package_add_benchmark(benchmark_any_level_put_get_put_1000 tests/benchmark/any_level/benchmark_any_level_put_get_put_1000.cpp)
    package_add_benchmark(benchmark_any_level_put_get_put_5000 tests/benchmark/any_level/benchmark_any_level_put_get_put_5000.cpp)
    package_add_benchmark(benchmark_any_level_put_get_put_10000 tests/benchmark/any_level/benchmark_any_level_put_get_put_10000.cpp)

    # Put, get, put new tests
    package_add_benchmark(benchmark_any_level_put_get_put_new_1000 tests/benchmark/any_level/benchmark_any_level_put_get_put_new_1000.cpp)
    package_add_benchmark(benchmark_any_level_put_get_put_new_5000 tests/benchmark/any_level/benchmark_any_level_put_get_put_new_5000.cpp)
    package_add_benchmark(benchmark_any_level_put_get_put_new_10000 tests/benchmark/any_level/benchmark_any_level_put_get_put_new_10000.cpp)
endif()