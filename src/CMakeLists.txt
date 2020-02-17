set(MAIN_SOURCES
    main.cpp
)

add_subdirectory(ast)
add_subdirectory(codegen)
add_subdirectory(inference)
add_subdirectory(state)
add_subdirectory(types)
add_subdirectory(util)

set(RHEA_LIBS
    rhea_ast
    rhea_codegen
    rhea_inference
    rhea_state
    rhea_types
    rhea_util
    ${llvm_libs}
    ${CONAN_LIBS}
)

add_executable(rhea ${MAIN_SOURCES})
target_include_directories(rhea PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(rhea ${RHEA_LIBS})

add_executable(rhea_debug_parse debug/parse.cpp)
target_include_directories(rhea_debug_parse PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(rhea_debug_parse ${RHEA_LIBS})

add_executable(rhea_debug_ast debug/ast.cpp)
target_include_directories(rhea_debug_ast PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(rhea_debug_ast ${RHEA_LIBS})

add_executable(rhea_debug_ir debug/ir.cpp)
target_include_directories(rhea_debug_ir PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(rhea_debug_ir ${RHEA_LIBS})

add_executable(rhea_debug_asm debug/asm.cpp)
target_include_directories(rhea_debug_asm PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(rhea_debug_asm ${RHEA_LIBS})