set(AST_SOURCES
    function.cpp
    identifiers.cpp
    simple_statement.cpp
    structure_type.cpp
    typenames.cpp
    module.cpp
    builder.cpp
)

add_library(rhea_ast STATIC ${AST_SOURCES})
target_include_directories(rhea_ast PUBLIC ${CMAKE_SOURCE_DIR}/include)
