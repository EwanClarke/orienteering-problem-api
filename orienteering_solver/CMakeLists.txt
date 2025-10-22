cmake_minimum_required(VERSION 3.15...4.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_core src/main.cpp
    src/algorithms/utils.cpp
    src/algorithms/greedy/NearestNeighbour.cpp
    src/algorithms/greedy/InsertionHeuristic.cpp
    src/algorithms/metaheuristic/AntColonyOptimisation.cpp
    src/algorithms/metaheuristic/GeneticAlgorithm.cpp
    src/algorithms/pathImprovement/TwoOpt.cpp
)
target_include_directories(_core PRIVATE src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/algorithms/greedy
    ${CMAKE_CURRENT_SOURCE_DIR}/src/algorithms/metaheuristic
    ${CMAKE_CURRENT_SOURCE_DIR}/src/algorithms/pathImprovement
    ${CMAKE_CURRENT_SOURCE_DIR}/src/algorithms/
)

target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

install(TARGETS _core DESTINATION ${SKBUILD_PROJECT_NAME})
