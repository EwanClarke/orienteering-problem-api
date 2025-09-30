import math
from typing import List

from orienteering_solver import OrienteeringProblemInputData
from errors import (
    ValidationError,
    MissingFieldError,
    InvalidTypeError,
    MatrixShapeError,
    MatrixValueError,
    ProfitsLengthError,
    OutOfBoundsError,
    NegativeValueError,
    InternalConversionError,
)


def validate_input(data: OrienteeringProblemInputData) -> None:
    """Validate an OrienteeringProblemInputData instance.

    Raises HTTPException with status_code=400 for client errors.
    """
    # Basic attribute presence and extraction
    try:
        matrix: List[List[float]] | None = data.adjacency_matrix
        profits: List[float] | None = data.profits
        budget = data.budget
        start_node = data.start_node
        end_node = data.end_node
    except Exception:
        raise InternalConversionError("Internal: solver input missing expected attributes")

    # adjacency_matrix checks
    if matrix is None:
        raise MissingFieldError("adjacency_matrix is required")
    if not isinstance(matrix, list):
        raise InvalidTypeError("adjacency_matrix must be a list of lists")
    n = len(matrix)
    if n == 0:
        raise MatrixShapeError("adjacency_matrix cannot be empty")

    for i, row in enumerate(matrix):
        if not isinstance(row, list):
            raise InvalidTypeError(f"adjacency_matrix row {i} must be a list")
        if len(row) != n:
            raise MatrixShapeError("Adjacency matrix must be square (n x n)")
        for j, val in enumerate(row):
            if not isinstance(val, (int, float)) or not math.isfinite(val):
                raise MatrixValueError(f"adjacency_matrix[{i}][{j}] must be a finite number")
            if val < 0:
                raise MatrixValueError(f"adjacency_matrix[{i}][{j}] must be non-negative")

    # diagonal (self-distance) sanity
    for i in range(n):
        if abs(matrix[i][i]) > 1e-9:
            raise MatrixValueError("Diagonal elements of adjacency matrix must be zero")

    # profits checks
    if profits is None:
        raise MissingFieldError("profits is required")
    if not isinstance(profits, list) or len(profits) != n:
        raise ProfitsLengthError("profits must be a list with the same length as adjacency_matrix")
    for idx, p in enumerate(profits):
        if not isinstance(p, (int, float)) or not math.isfinite(p):
            raise MatrixValueError(f"profit[{idx}] must be a finite number")

    # budget checks
    if budget is None or not isinstance(budget, (int, float)) or not math.isfinite(budget) or budget < 0:
        raise NegativeValueError("budget must be a non-negative finite number")

    # start/end checks
    if not isinstance(start_node, int) or not (0 <= start_node < n):
        raise OutOfBoundsError(f"start_node must be an integer in [0, {n-1}]")
    if not isinstance(end_node, int) or not (0 <= end_node < n):
        raise OutOfBoundsError(f"end_node must be an integer in [0, {n-1}]")
    if start_node == end_node:
        raise ValidationError("start_node and end_node must be different")
