set(TESTS_TYPES_SOURCES
    test_setup.cpp
    to_string.cpp
    mapper.cpp
)

add_library(tests_types OBJECT ${TESTS_TYPES_SOURCES})
target_link_libraries(tests_types rhea_ast rhea_util rhea_types)