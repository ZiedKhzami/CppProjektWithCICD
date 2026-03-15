cmake_minimum_required(VERSION 3.20)

project(FirstProject
    VERSION 1.0.0
    DESCRIPTION "Modern C++20 Project with Tests, Benchmarks and Tooling"
    LANGUAGES CXX
)

# =====================================================
# C++ Standard (MSVC-compatible)
# =====================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
include(CTest)
enable_testing()

# =====================================================
# Compiler Warnings (No /std overrides here!)
# =====================================================
if (MSVC)
    add_compile_options(
        /W4
        /permissive-
        /EHsc
        /utf-8
        /Zc:preprocessor
    )
else()
    add_compile_options(
        -Wall -Wextra -Wpedantic
        -Wshadow -Wconversion -Wsign-conversion
    )
endif()

# --- Remove unwanted -std:* from CMake cache (critical MSVC fix)

if (MSVC)
  foreach(VAR
      CMAKE_CXX_FLAGS
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS_MINSIZEREL)
    if (DEFINED ${VAR})
      # Entferne -std=c++NN / -std:c++NN (alles mit -std... bis zum nächsten Space)
      string(REGEX REPLACE "-std[^ ]+" "" CLEANED "${${VAR}}")
      # Optional: Falls im Cache Schmutz hängt, leeren wir die Variablen lieber ganz
      set(${VAR} "${CLEANED}" CACHE STRING "" FORCE)
    endif()
  endforeach()
endif()


# =====================================================
# clang-tidy (target-based, MSVC-safe)
# =====================================================
option(ENABLE_CLANG_TIDY "Enable clang-tidy" ON)

if (ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE
        NAMES clang-tidy clang-tidy-19 clang-tidy-18 clang-tidy-17 clang-tidy-16)

    if (CLANG_TIDY_EXE)
        if (MSVC)
            set(_CLANG_TIDY_ARGS
                "${CLANG_TIDY_EXE};
                 -warnings-as-errors=*;
                 --extra-arg=--driver-mode=cl"
            )
        else()
            set(_CLANG_TIDY_ARGS
                "${CLANG_TIDY_EXE};
                 -warnings-as-errors=*"
            )
        endif()
    endif()
endif()

# =====================================================
# clang-format (format / format-check)
# =====================================================
file(GLOB_RECURSE FORMAT_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/app/*.cpp"
    "${CMAKE_SOURCE_DIR}/tests/*.cpp"
    "${CMAKE_SOURCE_DIR}/bench/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/**/*.hpp"
    "${CMAKE_SOURCE_DIR}/include/**/*.h"
)

find_program(CLANG_FORMAT_EXE NAMES clang-format clang-format-19 clang-format-18)

if (CLANG_FORMAT_EXE)
    add_custom_target(format
        COMMAND "${CLANG_FORMAT_EXE}" -i -style=file ${FORMAT_SOURCES}
    )
    add_custom_target(format-check
        COMMAND "${CLANG_FORMAT_EXE}" --dry-run --Werror -style=file ${FORMAT_SOURCES}
    )
endif()

# =====================================================
# core_lib
# =====================================================
add_library(core_lib
    src/math.cpp
    include/project/math.hpp
)

target_include_directories(core_lib PUBLIC include)
target_compile_features(core_lib PUBLIC cxx_std_20)

if (ENABLE_CLANG_TIDY AND CLANG_TIDY_EXE)
    set_property(TARGET core_lib PROPERTY CXX_CLANG_TIDY "${_CLANG_TIDY_ARGS}")
endif()

# =====================================================
# app
# =====================================================
add_executable(app app/main.cpp)
target_link_libraries(app PRIVATE core_lib)

if (ENABLE_CLANG_TIDY AND CLANG_TIDY_EXE)
    set_property(TARGET app PROPERTY CXX_CLANG_TIDY "${_CLANG_TIDY_ARGS}")
endif()

# =====================================================
# GoogleTest
# =====================================================
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    tests/test_main.cpp
    tests/test_math.cpp
)

target_link_libraries(unit_tests PRIVATE
    core_lib
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME unit_tests COMMAND unit_tests)

# =====================================================
# Google Benchmark (MSVC clean)
# =====================================================
option(ENABLE_BENCHMARKS "Enable Benchmarks" ON)

if (ENABLE_BENCHMARKS)
    FetchContent_Declare(
        benchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(benchmark)

    add_executable(benchmarks
        bench/bench_math.cpp
    )

    target_link_libraries(benchmarks PRIVATE
        core_lib
        benchmark::benchmark
    )
endif()

# =====================================================
# Doxygen
# =====================================================
option(ENABLE_DOXYGEN "Enable Doxygen" ON)

if (ENABLE_DOXYGEN AND EXISTS "${CMAKE_SOURCE_DIR}/docs/Doxyfile.in")
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        configure_file(
            ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in
            ${CMAKE_BINARY_DIR}/Doxyfile
            @ONLY
        )
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif()
endif()

message(STATUS "====================================")
message(STATUS " FirstProject configured successfully ")
message(STATUS "====================================")