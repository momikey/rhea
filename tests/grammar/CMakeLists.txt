set(TESTS_GRAMMAR_SOURCES
    tokens.cpp
    operators.cpp
    statement.cpp
    strings.cpp
)

add_library(tests_grammar OBJECT ${TESTS_GRAMMAR_SOURCES})
