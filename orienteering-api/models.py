from typing import Any, Dict, List, Optional

from orienteering_solver import OrienteeringProblemInputData, nearest_neighbour, cheapest_insertion, most_profitable_insertion, greedy_insertion
from pydantic import BaseModel, Field

class location(BaseModel):
    lat: float
    lon: float

class InputData(BaseModel):
    adjacency_matrix: Optional[List[List[float]]] = Field(
        None,
        description=(
            "Adjacency matrix representing distances between nodes. Either this or locations must be provided."
        ),
    )
    locations: Optional[List[location]] = Field(
        None,
        description=(
            "List of locations represented by latitude and longitude. Either this or adjacency_matrix must be provided."
        ),
    )

    profits: Optional[List[float]] = None
    budget: Optional[float] = None
    start_node: Optional[int] = None
    end_node: Optional[int] = None

    algorithm: Optional[str] = Field(
        "nearest_neighbour",
        description=(
            "The algorithm to use for solving the orienteering problem. If not provided, defaults to 'nearest_neighbour'."
        ),
    )

    def to_solver_input(self) -> OrienteeringProblemInputData:
        if self.adjacency_matrix is None:
            raise ValueError("adjacency_matrix is required to convert to solver input")
        elif len(self.adjacency_matrix) == 0:
            raise ValueError("adjacency_matrix cannot be empty")
        elif any(len(row) != len(self.adjacency_matrix) for row in self.adjacency_matrix):
            raise ValueError("adjacency_matrix must be square")
        elif any(any(type(cell) not in [int, float] for cell in row) for row in self.adjacency_matrix):
            raise ValueError("adjacency_matrix must contain only numeric values")
        elif any(self.adjacency_matrix[i][i] != 0 for i in range(len(self.adjacency_matrix))):
            raise ValueError("adjacency_matrix must have zeros on its diagonal")
        elif any(self.adjacency_matrix[i][j] < 0 for i in range(len(self.adjacency_matrix)) for j in range(len(self.adjacency_matrix)) if i != j):
            raise ValueError("adjacency_matrix must not contain negative distances")
        
        if self.profits is None:
            raise ValueError("profits is required to convert to solver input")
        elif len(self.profits) != len(self.adjacency_matrix):
            raise ValueError("profits length must match adjacency_matrix size")
        elif any(type(p) not in [int, float] for p in self.profits):
            raise ValueError("profits must contain only numeric values")
        elif any(p < 0 for p in self.profits):
            raise ValueError("profits must not contain negative values")
        
        if self.budget is None:
            raise ValueError("budget is required to convert to solver input")
        elif type(self.budget) not in [int, float]:
            raise ValueError("budget must be a numeric value")
        elif self.budget < 0:
            raise ValueError("budget must not be negative")

        if self.start_node is None or self.end_node is None:
            raise ValueError("start_node and end_node are required to convert to solver input")
        elif type(self.start_node) is not int or type(self.end_node) is not int:
            raise ValueError("start_node and end_node must be integers")
        elif not (0 <= self.start_node < len(self.adjacency_matrix)) or not (0 <= self.end_node < len(self.adjacency_matrix)):
            raise ValueError("start_node and end_node must be valid node indices")
        elif self.start_node == self.end_node:
            raise ValueError("start_node and end_node must be different")
        elif self.adjacency_matrix[self.start_node][self.end_node] > self.budget:
            raise ValueError("budget is too small to travel from start_node to end_node")

        obj: Any = OrienteeringProblemInputData()
        obj.adjacency_matrix = self.adjacency_matrix
        obj.profits = self.profits
        obj.budget = float(self.budget)
        obj.start_node = int(self.start_node) 
        obj.end_node = int(self.end_node)
        return obj

class PathResponse(BaseModel):
    path: List[int]
    distance: float
    profit: float
