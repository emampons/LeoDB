#!/bin/bash
# Helper scirpt to clean up our enviroment

for file in 'test_value' \
            'test_key' \
            'test_operators' \
            'Makefile' \
            'leodb.cbp' \
            'leodb' \
            'CMakeCache.txt' \
            'cmake_install.cmake' \
            'tests/CMakeFiles' \
            'tests/cmake_install.cmake' \
            'tests/libleodb-tests.a' \
            'tests/Makefile' \
            'lib' \
            'tests/CMakeFiles' \
            'external/googletest-build' \
            'external/googletest-download' \
            'external/clipp.h' \
            'external/CMakeList.txt.in' \
            'CMakeFiles' \
            'cmake-build-normal' \
            'src/CMakeFiles' \
            'cmake_install.cmake' \
            'src/libleodb-src.a' \
            'src/Makefile' \
            'src/libleodb-src.a' \
            'src/cmake_install.cmake' \
            'src/CmakeFiles' \
            'CTestTestfile.cmake' \
            'external/CMakeListsTests.txt.in'
  do
    rm -r ${file}
  done

# Clean up bin
rm -r bin
mkdir -p bin
