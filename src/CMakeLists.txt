set(MAIN_SOURCES
    main.cpp
)

add_executable(rhea ${MAIN_SOURCES})
target_include_directories(rhea PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(rhea ${llvm_libs} ${CONAN_LIBS})
