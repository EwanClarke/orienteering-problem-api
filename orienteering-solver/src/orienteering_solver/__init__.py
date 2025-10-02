from __future__ import annotations

from ._core import OrienteeringProblemInputData, __doc__, __version__, nearest_neighbour, insertion_heuristic, find_cheapest_insertion, find_most_profitable_insertion, find_greedy_insertion

__all__ = ["OrienteeringProblemInputData", "__doc__", "__version__", "nearest_neighbour", "cheapest_insertion", "most_profitable_insertion", "greedy_insertion"]

def cheapest_insertion(problem_data):
    return insertion_heuristic(problem_data, find_cheapest_insertion)

def most_profitable_insertion(problem_data):
    return insertion_heuristic(problem_data, find_most_profitable_insertion)

def greedy_insertion(problem_data):
    return insertion_heuristic(problem_data, find_greedy_insertion)