set(TESTS_GRAMMAR_SOURCES
    tokens.cpp
    expression.cpp
    statement.cpp
    strings.cpp
    typenames.cpp
    concepts.cpp
    module.cpp
)

add_library(tests_grammar OBJECT ${TESTS_GRAMMAR_SOURCES})
