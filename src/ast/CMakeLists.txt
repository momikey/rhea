set(AST_SOURCES
    identifiers.cpp
)

add_library(rhea_ast STATIC ${AST_SOURCES})
target_include_directories(rhea_ast PUBLIC ${CMAKE_SOURCE_DIR}/include)
