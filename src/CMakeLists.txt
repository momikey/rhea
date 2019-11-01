set(MAIN_SOURCES
    main.cpp
)

add_executable(rhea ${MAIN_SOURCES})

target_link_libraries(rhea ${llvm_libs} ${CONAN_LIBS})
