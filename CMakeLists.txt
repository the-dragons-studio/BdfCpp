cmake_minimum_required(VERSION 3.27)
project(BdfCpp)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# build time in UTC ISO 8601
FILE (WRITE ${CMAKE_BINARY_DIR}/timestamp.cmake "STRING(TIMESTAMP TIMEZ UTC)\n")
FILE (APPEND ${CMAKE_BINARY_DIR}/timestamp.cmake "FILE(WRITE include/timestamp.h \"#ifndef TIMESTAMP_H\\n\")\n")
FILE (APPEND ${CMAKE_BINARY_DIR}/timestamp.cmake "FILE(APPEND include/timestamp.h \"#define TIMESTAMP_H\\n\\n\")\n")
FILE (APPEND ${CMAKE_BINARY_DIR}/timestamp.cmake "FILE(APPEND include/timestamp.h \"#define _TIMEZ_ \\\"\${TIMEZ}\\\"\\n\\n\")\n")
FILE (APPEND ${CMAKE_BINARY_DIR}/timestamp.cmake "FILE(APPEND include/timestamp.h \"#endif // TIMESTAMP_H\\n\")\n")
add_custom_target(
    timestamp
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/timestamp.cmake
    add_dependencies ${CMAKE_BINARY_DIR}/timestamp.cmake)

# Try to build Doxygen.
option(BUILD_DOC "Build documentation" ON)

add_executable(fibonacci examples/fibonacci.cpp)
add_dependencies(fibonacci bdf)
target_link_libraries(helloworld -lbdf)

add_executable(helloworld examples/helloworld.cpp)
add_dependencies(helloworld bdf)
target_link_libraries(helloworld -lbdf)

add_executable(tests examples/tests.cpp)
add_dependencies(tests bdf)
target_link_libraries(tests -lbdf)

add_executable(bdfconvert tools/bdfconvert.cpp)
add_dependencies(bdfconvert tools/bdfconvert)
target_link_libraries(bdfconvert -lbdf)

add_executable(bdfedit tools/bdfedit.cpp)
add_dependencies(bdfedit bdf)
target_link_libraries(bdfedit -lbdf -lboost)

add_library(bdf src/BdfError.cpp src/BdfHelpers.cpp src/BdfIndent.cpp src/BdfList.cpp src/BdfLookupTable.cpp src/BdfNamedList.cpp src/BdfObject.cpp src/BdfReader.cpp src/BdfReaderHuman.cpp src/BdfStringReader.cpp src/version.cpp)
add_dependencies(bdf timestamp)
# target_link_libraries(bdf )
install(TARGETS bdf)
