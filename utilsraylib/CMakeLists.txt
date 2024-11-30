# Main CMake for revuelto engine
cmake_minimum_required(VERSION 3.5.1)
project(utilsraylib VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Verbose
set(CMAKE_VERBOSE_MAKEFILE ON)

# Debug cmake
message(STATUS "CMake source directory is ${CMAKE_SOURCE_DIR}")
message(STATUS "System is ${CMAKE_SYSTEM_NAME}")
message(STATUS "Arquitecture is ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE CXX Flags set: ${MAKE_CXX_FLAGS}")
message(STATUS "CMAKE C Flags set: ${CMAKE_C_FLAGS}")

# -- Libraries
# Includes
include_directories("${PROJECT_SOURCE_DIR}/includes")

# Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

# Raylib
add_subdirectory("${PROJECT_SOURCE_DIR}/raylib")

# Add library
add_library(utilsraylib
	src/utils.cpp
	src/VirtualResWindow.cpp
)

# Link Raylib to the library
target_link_libraries(utilsraylib PUBLIC raylib)

target_include_directories(utilsraylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

# Install rules
install(TARGETS utilsraylib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY src/ DESTINATION include)