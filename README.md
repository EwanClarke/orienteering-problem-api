````markdown
Orienteering API / Solver

Overview

This repository contains a Python FastAPI application that exposes an orienteering solver endpoint (/solve). The heavy lifting solver is implemented in C++ and exposed to Python through pybind11 (package: `orienteering_solver`). The Python side converts JSON -> Pydantic models -> a small pybind11 DTO and calls the native solver.

This README documents how to build the C++ extension, how to add algorithms to the native module, how to wire new algorithms into the Python API, and developer tooling (tests, linter, mypy).

Table of contents
- Build the native extension (Windows / PowerShell)
- Add a C++ algorithm
- Expose the algorithm via pybind11
- Wire the algorithm into the Python API
- Tests and linting
- Troubleshooting


Build the native extension (Windows, PowerShell)

Prerequisites
- Visual Studio with C++ build tools (MSVC) and CMake in PATH
- Python 3.11+ (the project logs show CPython 3.13 but any compatible 3.x is expected)
- pip, virtualenv or other environment manager
- scikit-build-core and scikit-build/CMake toolchain (provided via pyproject.toml build-system)

Quick build and install (from repository root)

1. Create and activate a virtual environment (optional but recommended):

```powershell
python -m venv .venv
.\.venv\Scripts\Activate.ps1
```

2. Install build requirements and build+install the wheel:

```powershell
cd orienteering-solver
pip install --upgrade pip
pip install .
```

`pip install .` will use the pyproject.toml backend (scikit-build-core) and invoke CMake to build the C++ extension. If it succeeds you should see a wheel created and installed (e.g., `orienteering_solver-0.0.1-cpXXX-win_amd64.whl`).

If you need more control (build only):

```powershell
pip install build
python -m build -w
```

This will create a wheel in `dist/` which you can then install with `pip install dist\orienteering_solver-*.whl`.

Add a new C++ algorithm

Project layout (relevant parts)
- orienteering-solver/
  - src/
    - OrienteeringProblemInputData.h  # DTO struct used to pass data from Python
    - main.cpp  # pybind11 module registration
    - algorithms/
      - greedy/
        - NearestNeighbour.cpp/h

Steps to add an algorithm (C++)
1. Add a new header and source file in `src/algorithms/<category>/`, e.g. `MyAlgorithm.h` / `MyAlgorithm.cpp`.
2. Implement a function with a stable C++-side signature that accepts an `OrienteeringProblemInputData` (or appropriate fields) and returns a `std::vector<int>` representing the node path (indices).

Example signature (follow existing patterns):

```cpp
#include "OrienteeringProblemInputData.h"
#include <vector>

std::vector<int> my_algorithm(const OrienteeringProblemInputData &input) {
    // compute path
}
```

3. Add the new source file to the `CMakeLists.txt` (or the appropriate CMake target). There are two common approaches:
- Add the new files to the library target under `src/`.
- If using `pybind11_add_module`, include the algorithm .cpp in the target's sources.

4. Fix compilation errors locally (use Visual Studio or `cmake`/`ninja`) and run `pip install .` from the `orienteering-solver` directory to build and install the updated wheel.

Expose the algorithm via pybind11

1. Open `src/main.cpp` (or where pybind11 bindings are created).
2. Add a function binding for your algorithm (choose a stable name like `my_algorithm`). Example:

```cpp
m.def("my_algorithm", &my_algorithm, "Run My Algorithm");
```

3. Rebuild and reinstall the wheel.

Wire the algorithm into the Python API

1. In `orienteering-api/models.py` there is a mapping `FUNCTION_DICT` that maps string names (used by the API) to callables imported from the pybind module. Add your algorithm name and imported function there:

```python
from orienteering_solver import my_algorithm

FUNCTION_DICT = {
    "nearest_neighbour": nearest_neighbour,
    "my_algorithm": my_algorithm,
}
```

2. Add an enum entry to `OrienteeringProblemAlgorithm` if you want the API input model to accept the algorithm name via the `algorithm` field.

3. If your algorithm requires additional configuration parameters, update the `models.InputData` Pydantic model to include them (with sensible defaults) and ensure `to_solver_input()` transfers those fields into the `OrienteeringProblemInputData` DTO.

Notes about the DTO
- The `OrienteeringProblemInputData` pybind-exported class may not accept constructor args depending on how it's bound. The safe approach used here is to instantiate it with a no-arg constructor and assign attributes explicitly on the returned object (this avoids mismatches between Python and C++ signatures).

API specifics
- The FastAPI endpoint is `/solve` (see `main.py`). The input JSON maps to `models.InputData` which provides a `to_solver_input()` method that constructs and returns the DTO the native solver expects.
- Validation is performed in `input_validation.py` against the DTO fields and raises domain `ValidationError` subclasses for client errors; `InternalConversionError` is raised for server-side conversion problems. `main.py` maps these to HTTP 400/500 responses.

Tests, linting and type checking

Python tooling used in the repo:
- pytest for tests
- ruff for linting
- mypy for static type checking

To run tests and lint locally:

```powershell
# run tests
pytest -q

# run ruff (autofix)
pip install ruff
ruff check --fix .

# run mypy (we ignore pybind generated package by default using mypy.ini)
pip install mypy
python -m mypy --config-file mypy.ini .
```

Troubleshooting

- CMake build errors: Make sure Visual Studio and CMake are installed and that the correct tools are in PATH. Check the CMake and build output for the specific compiler error — often it's a missing header or a C++ signature mismatch when adding new sources.

- pybind import issues with mypy: The pybind-generated package is intentionally ignored by `mypy.ini` because static analyzers cannot inspect the binary extension. Use `Any` and `# type: ignore` where you interact with pybind DTOs.

- If algorithm fails at runtime: Re-run tests with a simple monkeypatched solver to confirm the problem is in the binding or algorithm implementation. The tests in this repo stub the solver behavior in `test_solve_endpoint.py`.

Developer notes

- Keep HTTP concerns in `main.py` — validation should raise domain exceptions, and the API boundary should map exceptions to HTTP codes.
- When adding algorithms, prefer clear, documented function signatures and write unit tests (C++ and Python) for the solver function exposing expected behavior for simple inputs.
- Consider adding a CI job to run ruff and mypy on each PR.

Contact

If anything in this README is out of date with the code base, open an issue or a PR updating the instructions.

