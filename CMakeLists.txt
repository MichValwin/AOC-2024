# Main CMake for revuelto engine
cmake_minimum_required(VERSION 3.5.1)
project(AOC2024 VERSION 0.1 LANGUAGES C CXX)

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
message(STATUS "Build shared?: ${BUILD_SHARED_LIBS}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # It will create compile_commnands.json a file that clangD and other extensions need to know where source files are for code completion
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(CUSTOMIZE_BUILD ON)
# UtilsRaylib
add_subdirectory("${PROJECT_SOURCE_DIR}/utilsraylib")

#--Add executable
function(add_day_solution DAY_NAME)
    add_executable(${PROJECT_NAME} ${DAY_NAME}/main.cpp)
endfunction()

add_day_solution(day0)

target_link_libraries(${PROJECT_NAME} PRIVATE utilsraylib)

if(BUILD_SHARED_LIBS)
	# To link dynamically
	add_compile_definitions(RAYLIB_LIBTYPE=SHARED)
	target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_SHARED_LIBS=ON RAYLIB_LIBTYPE=SHARED)
else()
	# To link statically
	target_compile_definitions(${PROJECT_NAME} PUBLIC)
endif()

