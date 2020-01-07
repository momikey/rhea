set(CODEGEN_SOURCES
    code_visitor.cpp
)

add_library(rhea_codegen STATIC ${CODEGEN_SOURCES})
target_include_directories(rhea_codegen PUBLIC ${CMAKE_SOURCE_DIR}/include)
