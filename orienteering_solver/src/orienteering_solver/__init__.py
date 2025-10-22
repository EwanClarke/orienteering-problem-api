from __future__ import annotations

from ._core import (
    OrienteeringProblemInputData,
    __doc__,
    __version__,
    nearest_neighbour,
    insertion_heuristic,
    find_cheapest_insertion,
    find_most_profitable_insertion,
    find_greedy_insertion,
    ant_colony_optimisation_cpp,
    genetic_algorithm_cpp
    )


__all__ = ["OrienteeringProblemInputData",
           "__doc__",
           "__version__", 
           "nearest_neighbour",
           "cheapest_insertion",
           "most_profitable_insertion",
           "greedy_insertion",
           "ant_colony_optimisation",
           "genetic_algorithm",
           "solve_orienteering_problem"
           ]

def cheapest_insertion(problem_data):
    return insertion_heuristic(problem_data, find_cheapest_insertion)

def most_profitable_insertion(problem_data):
    return insertion_heuristic(problem_data, find_most_profitable_insertion)

def greedy_insertion(problem_data):
    return insertion_heuristic(problem_data, find_greedy_insertion)

def ant_colony_optimisation(problem_data):
    # Parameters for the ACO algorithm can be adjusted as needed
    no_of_ants = 50
    no_of_iterations = 200
    alpha = 1.0  # Importance of pheromone
    beta = 3.0   # Importance of heuristic information
    evaporation_rate = 0.2  # Rate at which pheromone evaporates
    initial_pheromone = 1.0  # Initial pheromone level on edges

    return ant_colony_optimisation_cpp(problem_data, no_of_ants, no_of_iterations, alpha, beta, evaporation_rate, initial_pheromone)

def genetic_algorithm(problem_data):
    # Parameters for the Genetic Algorithm can be adjusted as needed
    population_size = 2
    generations = 2
    mutation_rate = 0.01  # Probability of mutation

    return genetic_algorithm_cpp(problem_data, population_size, generations, mutation_rate)

def solve_orienteering_problem(input_data: OrienteeringProblemInputData, algorithm: str):
    FUNCTION_DICT = {
        "nearest_neighbour": nearest_neighbour,
        "cheapest_insertion": cheapest_insertion,
        "most_profitable_insertion": most_profitable_insertion,
        "greedy_insertion": greedy_insertion,
        "ant_colony_optimisation": ant_colony_optimisation,
        "genetic_algorithm": genetic_algorithm
    }

    if algorithm not in FUNCTION_DICT:
        raise ValueError(f"Algorithm '{algorithm}' is not supported. Choose from {list(FUNCTION_DICT.keys())}.")

    solve_function = FUNCTION_DICT[algorithm]
    return solve_function(input_data)