set(AST_SOURCES
    binary_operator.cpp
    unary_operator.cpp
    function.cpp
    identifiers.cpp
    simple_statement.cpp
    control_statement.cpp
    structure_type.cpp
    exception.cpp
    typenames.cpp
    module.cpp
    builder.cpp
    visitor_impl.cpp
    concept.cpp
)

add_library(rhea_ast STATIC ${AST_SOURCES})
target_include_directories(rhea_ast PUBLIC ${CMAKE_SOURCE_DIR}/include)
