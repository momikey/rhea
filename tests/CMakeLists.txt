add_subdirectory(grammar)

add_executable(tests main.cpp)
target_link_libraries(tests PUBLIC tests_grammar)
target_link_libraries(tests PUBLIC ${CONAN_LIBS})
add_test(NAME test_all COMMAND tests)
