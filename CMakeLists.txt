cmake_minimum_required(VERSION 3.27)
project(BdfCpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "build/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "build/lib")

set(DOXYGEN_READY OFF)

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

add_executable(helloworld examples/helloworld.cpp)
add_dependencies(helloworld bdf)
target_link_libraries(helloworld -lbdf)

if(BUILD_DOC) 
	find_package(Doxygen)
	if(Doxygen_FOUND)
		doxygen_add_docs(
			doxygen
			${PROJECT_SOURCE_DIR}
			CONFIG_FILE ${PROJECT_SOURCE_DIR}/Doxyfile
		)
		set(DOXYGEN_READY ON)
	else(Doxygen_FOUND)
		message(WARNING "Building of documentation was requested but Doxygen was not found. Please install Doxygen and ensure it is accessible to CMake. To suppress this warning, disable building documentation.")
	endif(Doxygen_FOUND)
endif()

add_custom_target(
    copy_include
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/build/include
)

add_library(bdf src/BdfError.cpp src/BdfHelpers.cpp src/BdfIndent.cpp src/BdfList.cpp src/BdfLookupTable.cpp src/BdfNamedList.cpp src/BdfObject.cpp src/BdfReader.cpp src/BdfReaderHuman.cpp src/BdfStringReader.cpp src/version.cpp)
add_dependencies(bdf timestamp)
add_dependencies(bdf copy_include)
if (DOXYGEN_READY)
	add_dependencies(bdf doxygen)
endif(DOXYGEN_READY)
# target_link_libraries(bdf )
install(TARGETS bdf)