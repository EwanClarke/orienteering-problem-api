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
        if self.profits is None:
            raise ValueError("profits is required to convert to solver input")
        if self.budget is None:
            raise ValueError("budget is required to convert to solver input")
        if self.start_node is None or self.end_node is None:
            raise ValueError("start_node and end_node are required to convert to solver input")

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
