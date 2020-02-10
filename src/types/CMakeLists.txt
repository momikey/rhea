set(TYPES_SOURCES
    mapper.cpp
)

add_library(rhea_types STATIC ${TYPES_SOURCES})
target_include_directories(rhea_types PUBLIC ${CMAKE_SOURCE_DIR}/include)
