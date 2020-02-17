set(INFERENCE_SOURCES
    engine.cpp
    visitor.cpp
)

add_library(rhea_inference STATIC ${INFERENCE_SOURCES})
target_include_directories(rhea_inference PUBLIC ${CMAKE_SOURCE_DIR}/include)
