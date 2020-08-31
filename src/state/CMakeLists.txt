set(STATE_SOURCES
    symbol.cpp
    module_tree.cpp
    module_node.cpp
)

add_library(rhea_state STATIC ${STATE_SOURCES})
target_include_directories(rhea_state PUBLIC ${CMAKE_SOURCE_DIR}/include)
