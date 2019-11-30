add_subdirectory(grammar)
add_subdirectory(ast)

add_executable(tests main.cpp)
target_link_libraries(tests PUBLIC tests_grammar tests_ast)
target_link_libraries(tests PUBLIC ${CONAN_LIBS})
add_test(NAME test_all COMMAND tests)
