set(TESTS_AST_SOURCES
    test_setup.cpp
    literals.cpp
    identifiers.cpp
    operators.cpp
)

add_library(tests_ast OBJECT ${TESTS_AST_SOURCES})
target_link_libraries(tests_ast rhea_ast)