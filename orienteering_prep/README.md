# Orienteering Prep

Small helper utilities used by the orienteering problem project.

This package provides Python utilities used during development and testing (for example, input preprocessing and matrix construction helpers).

Install (development editable):

python -m pip install -e .

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
```powershell
python -m pip install -r -e .
```

