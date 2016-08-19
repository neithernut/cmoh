cmake_minimum_required(VERSION 3.5)
project(cmoh CXX)

# we require C++14
set(CMAKE_CXX_STANDARD 14)


# some formal declaration of the library
add_library(cmoh INTERFACE)
target_include_directories(cmoh INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


# examples/tests
enable_testing()
add_subdirectory(examples)
add_subdirectory(test)


# installation
install(
    DIRECTORY include/cmoh/
    DESTINATION include/cmoh
    FILES_MATCHING PATTERN "*.hpp"
)

# CMake specific export
#
# Because of CMake being a bitch when it comes to non-stndart use-cases, we
# have to have this dummy target which we can't even use to install the
# headers
install(TARGETS cmoh EXPORT cmoh PUBLIC_HEADER)
install(EXPORT cmoh DESTINATION lib)

# Pkg-config export
configure_file(cmoh.pc.in cmoh.pc @ONLY)
install(FILES cmoh.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)

