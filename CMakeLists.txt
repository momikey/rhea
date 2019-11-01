cmake_minimum_required(VERSION 3.7)
project(rhea VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#### Basic CMake stuff
add_compile_options(-Wall)

include(CTest)
enable_testing()

#### BEGIN LLVM ####
# LLVM is notorious for being nearly impossible to package, so we don't even bother.
# We'll just use what's installed, as long as it's at least version 7.
set(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
set(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)

set(LLVM_ABI_BREAKING_CHECKS FORCE_OFF)
find_package(LLVM 7 REQUIRED CONFIG)
message(STATUS "Building with LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Adding include directories ${LLVM_INCLUDE_DIRS}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
#### END LLVM ####

### Conan stuff
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
#message(STATUS "Conan libs: ${CONAN_LIBS}")

add_subdirectory(src)

add_subdirectory(tests)

#### LLVM linking
llvm_map_components_to_libnames(llvm_libs core mcjit native)
#message(STATUS "LLVM libs: ${llvm_libs}")

#### CPack stuff
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
