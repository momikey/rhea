set(CODEGEN_SOURCES
    generator.cpp
    code_visitor.cpp
    allocation_manager.cpp
    type_convert.cpp
    function_visitor.cpp
)

add_library(rhea_codegen STATIC ${CODEGEN_SOURCES})
target_include_directories(rhea_codegen PUBLIC ${CMAKE_SOURCE_DIR}/include)
