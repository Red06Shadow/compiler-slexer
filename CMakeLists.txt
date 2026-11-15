cmake_minimum_required(VERSION 3.10)

project(compiler-slexer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/debug/bin)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/release/bin)
endif()


include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(yaml-cpp CONFIG REQUIRED)

find_library(LIBGIT2_LIBRARY
    NAMES git2 libgit2
    HINTS
        "$ENV{MSYSTEM_PREFIX}/lib"
        "C:/msys64/mingw64/lib"
        "C:/msys64/ucrt64/lib"
        "C:/msys64/usr/lib"
)

find_path(LIBGIT2_INCLUDE_DIR
    NAMES git2.h
    HINTS
        "$ENV{MSYSTEM_PREFIX}/include"
        "C:/msys64/mingw64/include"
        "C:/msys64/ucrt64/include"
        "C:/msys64/usr/include"
)

if(LIBGIT2_LIBRARY AND LIBGIT2_INCLUDE_DIR)
    add_library(LibGit2::LibGit2 UNKNOWN IMPORTED)
    set_target_properties(LibGit2::LibGit2 PROPERTIES
        IMPORTED_LOCATION "${LIBGIT2_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LIBGIT2_INCLUDE_DIR}"
    )
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBGIT2 REQUIRED IMPORTED_TARGET libgit2)
endif()

add_executable(compiler-slexer
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/include/core/parser/parser.cpp
    ${CMAKE_SOURCE_DIR}/include/core/clone/clone.cpp
    ${CMAKE_SOURCE_DIR}/include/core/expresions/expresions.cpp
    ${CMAKE_SOURCE_DIR}/include/core/build/build.cpp
    ${CMAKE_SOURCE_DIR}/include/core/clean/clean.cpp
)

target_link_libraries(compiler-slexer PRIVATE yaml-cpp::yaml-cpp)

if(TARGET LibGit2::LibGit2)
    target_link_libraries(compiler-slexer PRIVATE LibGit2::LibGit2)
else()
    target_link_libraries(compiler-slexer PRIVATE PkgConfig::LIBGIT2)
endif()
