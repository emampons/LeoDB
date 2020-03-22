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
    ## Install Google Tests
    configure_file(external/CMakeLists.txt.in ${CMAKE_BINARY_DIR}/external/googletest-download/CMakeLists.txt)
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/external/googletest-download"
            )
    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/external/googletest-download"
            )

    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    #set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # Add googletest directly to our build. This adds the following targets:
    # gtest, gtest_main, gmock and gmock_main
    #set(GTEST_INCLUDE ${PROJECT_SOURCE_DIR}/external/googletest-src/include)
    #add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/external/googletest-src
    #        ${CMAKE_CURRENT_BINARY_DIR}/external/googletest-build
    #        EXCLUDE_FROM_ALL)

    #Add the google test subdirectory
    set(GOOGLETEST_DIR, ${CMAKE_CURRENT_BINARY_DIR}/external/googletest-src)
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/external/googletest-src)

    #include googletest/include dir
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/external/googletest-src/googletest/include)

    #include the googlemock/include dir
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/external/googletest-src/googlemock/include)

    #target_link_libraries(${PROJECT_NAME} )
    #target_link_libraries(${PROJECT_NAME} gtest)

    add_subdirectory(tests)
    add_executable(test_operators tests/test_operators.cpp)
    add_executable(test_key tests/test_key.cpp)
    add_executable(test_value tests/test_value.cpp)

    target_link_libraries(test_operators PUBLIC leodb-tests gtest gtest_main)
    target_link_libraries(test_key PUBLIC leodb-tests gtest gtest_main)
    target_link_libraries(test_value PUBLIC leodb-tests gtest gtest_main)
endif()