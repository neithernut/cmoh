cmake_minimum_required(VERSION 3.5)
project(cmoh CXX)


add_library(cmoh INTERFACE)
target_include_directories(cmoh INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


