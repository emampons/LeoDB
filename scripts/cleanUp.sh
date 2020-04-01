# Clean up our enviroment
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
            'test_benchmark'
  do
    rm -r ${file}
  done
