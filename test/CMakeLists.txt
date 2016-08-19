# Self sufficiency tests
#
# This tests whether the CMOH header files include everything they need by
# compiling minimalistic executables which include only one header of the
# header files.
add_custom_target(selfsufficiency)

# Collect all the header files
file(GLOB HEADER_FILES
    LIST_DIRECTORIES false
    RELATIVE ${PROJECT_SOURCE_DIR}/include/cmoh
    ${PROJECT_SOURCE_DIR}/include/cmoh/*.hpp
)

foreach(HEADER_FILE ${HEADER_FILES})
    string(REGEX REPLACE "(.*)\\.hpp" "\\1" HEADER_BASE ${HEADER_FILE})

    # Make the executables
    configure_file(self_sufficiency.cpp.in ${HEADER_BASE}_selfsufficiency.cpp)
    add_executable(${HEADER_BASE}_selfsufficiency EXCLUDE_FROM_ALL
        ${HEADER_BASE}_selfsufficiency.cpp
    )
    target_link_libraries(${HEADER_BASE}_selfsufficiency cmoh)

    # Handle depdendencies
    add_test(
        NAME ${HEADER_BASE}_selfsufficiency_test
        COMMAND ${HEADER_BASE}_selfsufficiency
        --exe $<TARGET_FILE:${HEADER_BASE}_selfsufficiency>
    )
    add_dependencies(selfsufficiency ${HEADER_BASE}_selfsufficiency)
endforeach(HEADER_FILE)
