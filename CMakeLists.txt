cmake_minimum_required(VERSION 3.5)
project(cmoh CXX)

# we require C++11
set(CMAKE_CXX_STANDARD 11)


# some formal declaration of the library
add_library(cmoh INTERFACE)
target_include_directories(cmoh INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
install(
    DIRECTORY include/cmoh/
    DESTINATION include/cmoh
    FILES_MATCHING PATTERN "*.hpp"
)


# examples/tests
enable_testing()
add_subdirectory(examples)


