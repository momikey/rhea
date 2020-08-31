set(UTIL_SOURCES
    symbol_hash.cpp
)

add_library(rhea_util STATIC ${UTIL_SOURCES})
target_include_directories(rhea_util PUBLIC ${CMAKE_SOURCE_DIR}/include)
