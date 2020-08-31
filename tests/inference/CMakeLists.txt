set(TESTS_INFERENCE_SOURCES
    engine.cpp
)

add_library(tests_inference OBJECT ${TESTS_INFERENCE_SOURCES})
target_link_libraries(tests_inference rhea_ast rhea_util rhea_types rhea_inference)