set(TESTS_CODEGEN_SOURCES
    test_setup.cpp
    startup.cpp
    literals.cpp
)

add_library(tests_codegen OBJECT ${TESTS_CODEGEN_SOURCES})
target_link_libraries(tests_codegen rhea_ast rhea_codegen rhea_util ${llvm_libs})