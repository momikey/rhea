add_subdirectory(grammar)
add_subdirectory(ast)
add_subdirectory(codegen)
add_subdirectory(types)
add_subdirectory(inference)

set(TEST_LIBS
    tests_grammar
    tests_ast
    tests_codegen
    tests_types
    tests_inference
    ${CONAN_LIBS}
)

add_executable(tests main.cpp)
target_link_libraries(tests PUBLIC ${TEST_LIBS})
add_test(NAME test_all COMMAND tests)
