file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)
add_library(leodb-src STATIC ${SOURCE_FILES} ${HEADER_FILES})
