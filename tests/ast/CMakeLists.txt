set(TESTS_AST_SOURCES
    test_setup.cpp
    literals.cpp
    identifiers.cpp
    operators.cpp
    simple_statement.cpp
    structure_type.cpp
    control_statement.cpp
    typenames.cpp
    function.cpp
    module.cpp
    builder.cpp
    visitor.cpp
)

add_library(tests_ast OBJECT ${TESTS_AST_SOURCES})
target_link_libraries(tests_ast rhea_ast)