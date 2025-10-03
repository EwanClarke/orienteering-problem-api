# Orienteering API / Solver

## Overview

This repository contains a Python FastAPI application that exposes an orienteering solver endpoint (/solve). The heavy lifting solver is implemented in C++ and exposed to Python through pybind11 (package: `orienteering_solver`). The Python side converts JSON -> Pydantic models -> a small pybind11 DTO and calls the native solver. The api accepts both complete adjacency matrices and lists of locations (pairs of latitude and longitude values) which are used to construct a complete geodesic adjacency matrix, with plans to allow matrix construction based on transportation networks.

## Installation

- clone the repository
- install packages
  - pip install .

## Local running of api & inputs
To run the api, run the following in a terminal from the root directory:
```
uvicorn orienteering-api.main:app --port 8080
```
### Requests
A simple request from the terminal will look like the following:
```
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
