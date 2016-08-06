# example/test building infrastructure


# basic attributes example
add_executable(attributes_example EXCLUDE_FROM_ALL
    attributes_example.cpp
    person.cpp
)
target_link_libraries(attributes_example cmoh)
add_test(
    NAME attributes
    COMMAND attributes_example --exe $<TARGET_FILE:attributes_example>
)


# provide a convenience target to build all examples
add_custom_target(examples DEPENDS
    attributes_example
)

