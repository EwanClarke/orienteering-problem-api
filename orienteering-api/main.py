from typing import Any, Dict

from fastapi import FastAPI, HTTPException
from .models import InputData, PathResponse
from orienteering_prep.matrix_construction import construct_geodesic_adjacency_matrix
from .input_validation import validate_input
from .errors import ValidationError, InternalConversionError
from orienteering_solver import solve_orienteering_problem

app = FastAPI()

@app.post("/solve", response_model=PathResponse)
def solve(input_data: InputData) -> Dict[str, Any]:
    has_matrix = input_data.adjacency_matrix is not None
    has_locations = input_data.locations is not None

    if has_matrix and has_locations:
        raise HTTPException(status_code=400, detail="Provide either adjacency_matrix or locations, not both.")
    elif not has_matrix and not has_locations:
        raise HTTPException(status_code=400, detail="Either adjacency_matrix or locations must be provided.")

    if has_locations:
        input_data.adjacency_matrix = construct_geodesic_adjacency_matrix(input_data.locations)

    try:
        solver_input = input_data.to_solver_input()
    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))
    
    try:
        validate_input(solver_input)
    except ValidationError as e:
        raise HTTPException(status_code=getattr(e, "status_code", 400), detail=str(e))
    except InternalConversionError as e:
        raise HTTPException(status_code=getattr(e, "status_code", 500), detail="Internal validation error")
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Internal validation error: {e}")

    try:
        result = solve_orienteering_problem(solver_input, input_data.algorithm)
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"An error occurred while solving the problem: {e}")
    
    distance = sum(input_data.adjacency_matrix[result[i]][result[i+1]] for i in range(len(result)-1))
    profit = sum(input_data.profits[node] for node in result)
    
    return {"path": result, "distance": distance, "profit": profit}