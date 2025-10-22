# Orienteering API / Solver

## Overview

This repository contains a Python FastAPI application that exposes an orienteering solver endpoint (/solve). The heavy lifting solver is implemented in C++ and exposed to Python through pybind11 (package: `orienteering_solver`). The Python side converts JSON -> Pydantic models -> a small pybind11 DTO and calls the native solver. The api accepts both complete adjacency matrices and lists of locations (pairs of latitude and longitude values) which are used to construct a complete geodesic adjacency matrix, with plans to allow matrix construction based on transportation networks.

## Installation

Prerequisites

- Python 3.8+ (this workspace was tested with Python 3.13 on Windows).
- A C/C++ toolchain for building the native solver:
    - On Windows: Visual Studio (MSVC) with "Desktop development with C++" and CMake.
    - CMake and a working build environment must be available on PATH.
- pip, wheel

Developer / local editable install (recommended)

1. From the repository root, install the local packages in editable mode. This will build the native solver and install the small helper package used by the API:

```powershell
python -m pip install -r .\dev-requirements.txt
```

This installs the C++ extension (`orienteering_solver`) and the Python helper package (`orienteering_prep`) in editable mode so changes in the source tree are immediately visible.

If you see errors when building the solver, ensure your C++ toolchain and CMake are installed and that you opened the terminal from a developer command prompt (or have MSVC and CMake on PATH).

## Local running of the API

To run the API from the repository root using the included app in the `orienteering-api` directory, point uvicorn at the module and use `--app-dir` because the directory name contains a hyphen (not a valid Python package identifier):

```powershell
uvicorn orienteering-api.main:app --reload --port 8080
```

### Requests

A simple request from the terminal will look like the following (PowerShell example):

```powershell
curl -X POST -H "Content-Type: application/json" -d "{\"locations\": [{\"lat\": 51.5074, \"lon\": 0.1278}, {\"lat\": 48.8566, \"lon\": 2.3522}, {\"lat\": 52.5200, \"lon\": 13.4050}, {\"lat\": 40.7128, \"lon\": -74.0060}, {\"lat\": 55.9533, \"lon\": -3.1883}], \"profits\": [0.0, 50.0, 40.0, 60.0, 0.0], \"budget\": 5000.0, \"start_node\": 0, \"end_node\": 4, \"algorithm\": \"nearest_neighbour\"}" "http://127.0.0.1:8080/solve"
```
Adjacency matrix input will involve swapping out locations for matrix followed by nested arrays for the adjacency matrix.
## Adding algorithms

### Implement the algorithm:
The algorithm must be exposed through a function, accepting standard OrienteeringProblemInputData struct as input and return the list of locations visited (vector<int>) including the start and end nodes.
e.g. std::vector<int> nearestNeighbour(OrienteeringProblemInputData input);

```
| orienteering-solver
|-> src
| |-> algorithms
| | |-> greedy
| | |-> metaheuristic
| | |-> exact
```

### Algorithm build and bindings:
1. Add .cpp file to CMakeLists.txt

```
pybind11_add_module(_core src/main.cpp
    src/algorithms/greedy/NearestNeighbour.cpp
    ...  <-- here
)
```

2. Add pybind11 binding for the function in main.cpp
    - Include the header file
    - Add function binding

```
PYBIND11_MODULE(_core, m) {
    ...
    m.def("nearest_neighbour", &nearestNeighbour, 
          "A function to find the nearest neighbour path given OP constraints.",
          py::arg("input_data") <--any additional parameters--> );
}
```
3. Expose the function
    - (optional) Create a wrapper function, required if there are additional parameters
    - Add the function to the FUNCTION_DICT in solve_orienteering_problem
4. Reinstall packages
    - pip install .
