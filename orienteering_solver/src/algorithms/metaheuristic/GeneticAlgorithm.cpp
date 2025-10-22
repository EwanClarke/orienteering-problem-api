
#include "GeneticAlgorithm.h"
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <cstdlib>

std::vector<int> geneticAlgorithm(const OrienteeringProblemInputData& input, int populationSize, int generations, double mutationRate) {
    
    std::vector<std::vector<int>> population = generateInitialPopulation(input, populationSize);

    while (generations-- > 0) {
        std::vector<std::vector<int>> newPopulation;

        while (newPopulation.size() < populationSize) {
            auto [parent1, parent2] = selectParents(population, input);
            auto offspring = edgeRecombinationCrossover(input, parent1, parent2);
            mutate(input, offspring, mutationRate);
            offspring = insertionImprovement(input, offspring);
            newPopulation.push_back(offspring);
        }

        population = std::move(newPopulation);
    }
    
    std::vector<int> bestIndividual = selectBestIndividual(input, population);

    return bestIndividual;
}

std::vector<std::vector<int>> generateInitialPopulation(const OrienteeringProblemInputData& problemData, int populationSize) {
    std::vector<std::vector<int>> population;
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(generateIndividual(problemData));
    }
    return population;
}

std::vector<int> generateIndividual(const OrienteeringProblemInputData& problemData) {
    std::vector<int> path = {problemData.startNode};
    std::vector<bool> visited(problemData.profits.size(), false);
    visited[problemData.startNode] = true;
    double remainingBudget = problemData.budget;

    while (true) {
        std::vector<int> candidates;
        for (size_t i = 0; i < problemData.profits.size(); ++i) {
            if (!visited[i] && problemData.adjacencyMatrix[path.back()][i] + problemData.adjacencyMatrix[i][problemData.endNode] <= remainingBudget) {
                candidates.push_back(i);
            }
        }
        if (candidates.empty()) break;
        int nextNode = candidates[rand() % candidates.size()];
        path.push_back(nextNode);
        visited[nextNode] = true;
        remainingBudget -= problemData.adjacencyMatrix[path[path.size() - 2]][nextNode];
    }
    path.push_back(problemData.endNode);
    return path;
}

std::pair<std::vector<int>, std::vector<int>> selectParents(const std::vector<std::vector<int>>& population, const OrienteeringProblemInputData& problemData) {
    std::vector<double> fitnessScores;
    for (const auto& individual : population) {
        fitnessScores.push_back(evaluatePath(problemData, individual));
    }
    int totalFitness = std::accumulate(fitnessScores.begin(), fitnessScores.end(), 0);

    return {population[rouletteWheelSelection(fitnessScores, totalFitness)],
            population[rouletteWheelSelection(fitnessScores, totalFitness)]};
}

int rouletteWheelSelection(const std::vector<double>& fitnessScores, int totalFitness) {
    int randomValue = rand() % totalFitness;
    int cumulativeFitness = 0;
    for (size_t i = 0; i < fitnessScores.size(); ++i) {
        cumulativeFitness += fitnessScores[i];
        if (randomValue < cumulativeFitness) {
            return i;
        }
    }
    return fitnessScores.size() - 1;
}

std::vector<int> edgeRecombinationCrossover(const OrienteeringProblemInputData& problemData, const std::vector<int>& parent1, const std::vector<int>& parent2) {
    std::unordered_map<int, std::unordered_set<int>> adjacencyList1, adjacencyList2;
    int n = parent1.size();
    for (int i = 0; i < n-2; ++i) { // Exclude last node (endNode)
        adjacencyList1[parent1[i]].insert(parent1[(i+1)]);
        adjacencyList1[parent1[i+1]].insert(parent1[i]);
        adjacencyList2[parent2[i]].insert(parent2[(i+1)]);
        adjacencyList2[parent2[i+1]].insert(parent2[i]);
    }

    std::vector<int> child;
    std::unordered_set<int> visited;
    int current = problemData.startNode;
    child.push_back(current);
    visited.insert(current);
    double remainingBudget = problemData.budget;

    while (child.size() < n) {
        for (auto& [key, neighbors] : adjacencyList1) {
            neighbors.erase(current);
        }
        for (auto& [key, neighbors] : adjacencyList2) {
            neighbors.erase(current);
        }

        const auto& neighbors1 = adjacencyList1[current];
        const auto& neighbors2 = adjacencyList2[current];
        std::unordered_set<int> combinedNeighbors; // Union of neighbors
        combinedNeighbors.insert(neighbors1.begin(), neighbors1.end());
        combinedNeighbors.insert(neighbors2.begin(), neighbors2.end());
        if (combinedNeighbors.empty()) break; // No possible recombination

        int next = -1;
        int minSize = INT_MAX;
        for (int neighbor : combinedNeighbors) {
            int size = adjacencyList1[neighbor].size() + adjacencyList2[neighbor].size();
            if (size < minSize && !visited.contains(neighbor)) {
                minSize = size;
                next = neighbor;
            }
        }
        if (next == -1) break; // No unvisited neighbors

        if (problemData.adjacencyMatrix[current][next] + problemData.adjacencyMatrix[next][problemData.endNode] > remainingBudget) {
            break; // Cannot add next node due to budget constraint
        }
        child.push_back(next);
        visited.insert(next);
        remainingBudget -= problemData.adjacencyMatrix[current][next];
        current = next;
    }
    child.push_back(problemData.endNode);
    remainingBudget -= problemData.adjacencyMatrix[current][problemData.endNode];

    return child;
}

std::vector<int> insertionImprovement(const OrienteeringProblemInputData& problemData, const std::vector<int>& individual) {
    double remainingBudget = problemData.budget;
    for (size_t i = 0; i < individual.size() - 1; ++i) {
        remainingBudget -= problemData.adjacencyMatrix[individual[i]][individual[i + 1]];
    }
    std::vector<int> improvedPath = individual;
    std::vector<bool> visited(problemData.profits.size(), false);
    for (int node : individual) {
        visited[node] = true;
    }

    while (true) {
        auto [insertionLocation, insertionNode, insertionDelta] = findGreedyInsertion(problemData, improvedPath, visited, remainingBudget);
        if (insertionNode == -1) {
            break;
        }

        improvedPath.insert(improvedPath.begin() + insertionLocation, insertionNode);
        visited[insertionNode] = true;
        remainingBudget -= insertionDelta;
    }

    return improvedPath;
}

void mutate(const OrienteeringProblemInputData& problemData, std::vector<int>& individual, double mutationRate) {
    if ((rand() / double(RAND_MAX)) > mutationRate) {
        return;
    }

    if ((rand() / double(RAND_MAX)) > 0.5) {
        if (individual.size() > 3) {
            int removeIndex = rand() % (individual.size() - 2) + 1;
            individual.erase(individual.begin() + removeIndex);
        }
    } else {
        if (individual.size() >= 4) {
        individual = twoOptBestImprovement(problemData, individual);
        }
    }
    
}

std::vector<int> selectBestIndividual(const OrienteeringProblemInputData& problemData, const std::vector<std::vector<int>>& population) {
    return *std::max_element(population.begin(), population.end(), [&](const std::vector<int>& a, const std::vector<int>& b) {
        return evaluatePath(problemData, a) < evaluatePath(problemData, b);
    });
}