set(MAIN_SOURCES
    main.cpp
)

add_subdirectory(ast)
add_subdirectory(codegen)
add_subdirectory(util)

add_executable(rhea ${MAIN_SOURCES})
target_include_directories(rhea PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(rhea rhea_ast rhea_codegen rhea_util ${llvm_libs} ${CONAN_LIBS})
