cmake_minimum_required(VERSION 3.15...4.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_core src/main.cpp
    src/algorithms/greedy/NearestNeighbour.cpp
    src/algorithms/greedy/InsertionHeuristic.cpp
)
target_include_directories(_core PRIVATE src)

target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

install(TARGETS _core DESTINATION ${SKBUILD_PROJECT_NAME})
