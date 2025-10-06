
#include "GeneticAlgorithm.h"
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <cstdlib>

std::vector<int> geneticAlgorithm(const OrienteeringProblemInputData& input, int populationSize, int generations, double mutationRate) {
    
    std::vector<std::vector<int>> population = generateInitialPopulation(input, populationSize);

    while (generations-- > 0) {
        std::vector<std::vector<int>> newPopulation;

        while (newPopulation.size() < populationSize) {
            auto [parent1, parent2] = selectParents(population, input);
            auto offspring = crossover(parent1, parent2);
            mutate(offspring, mutationRate, input);
            newPopulation.push_back(offspring.first);
            if (newPopulation.size() < populationSize) {
                newPopulation.push_back(offspring.second);
            }
        }

        population = std::move(newPopulation);
    }
    
    std::vector<int> bestIndividual = selectBestIndividual(population, input);

    return bestIndividual;
}

std::vector<std::vector<int>> generateInitialPopulation(const OrienteeringProblemInputData& input, int populationSize) {
    std::vector<std::vector<int>> population;
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(generateIndividual(input));
    }
    return population;
}

std::vector<int> generateIndividual(const OrienteeringProblemInputData& input) {
    std::vector<int> path = {input.startNode};
    std::vector<bool> visited(input.profits.size(), false);
    visited[input.startNode] = true;
    double remainingBudget = input.budget;

    while (true) {
        std::vector<int> candidates;
        for (size_t i = 0; i < input.profits.size(); ++i) {
            if (!visited[i] && input.adjacencyMatrix[path.back()][i] + input.adjacencyMatrix[i][input.endNode] <= remainingBudget) {
                candidates.push_back(i);
            }
        }
        if (candidates.empty()) break;
        int nextNode = candidates[rand() % candidates.size()];
        path.push_back(nextNode);
        visited[nextNode] = true;
        remainingBudget -= input.adjacencyMatrix[path[path.size() - 2]][nextNode];
    }
    path.push_back(input.endNode);
    return path;
}

std::pair<std::vector<int>, std::vector<int>> selectParents(const std::vector<std::vector<int>>& population, const OrienteeringProblemInputData& input) {
    std::vector<int> fitnessScores;
    for (const auto& individual : population) {
        fitnessScores.push_back(static_cast<int>(evaluateFitness(individual, input)));
    }
    int totalFitness = std::accumulate(fitnessScores.begin(), fitnessScores.end(), 0);

    return {population[rouletteWheelSelection(fitnessScores, totalFitness)],
            population[rouletteWheelSelection(fitnessScores, totalFitness)]};
}

int rouletteWheelSelection(const std::vector<int>& fitnessScores, int totalFitness) {
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

std::pair<std::vector<int>, std::vector<int>> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
    int size = std::min(parent1.size(), parent2.size());
    int start = rand() % size;
    int end = start + rand() % (size - start);
    // Create children with segments from both parents
    std::vector<int> child1, child2;
    child1.insert(child1.begin(), parent2.begin(), parent2.begin() + start);
    child2.insert(child2.begin(), parent1.begin(), parent1.begin() + start);

    child1.insert(child1.end(), parent1.begin() + start, parent1.begin() + end);
    child2.insert(child2.end(), parent2.begin() + start, parent2.begin() + end);

    child1.insert(child1.end(), parent2.begin() + end, parent2.end());
    child2.insert(child2.end(), parent1.begin() + end, parent1.end());
    // Remove duplicates while preserving order
    std::unordered_set<int> child1Set, child2Set;
    child1Set.insert(*child1.end());
    child2Set.insert(*child2.end());
    std::vector<int>::iterator it;
    for (it = child1.begin(); it != child1.end()-1; ++it) {
        if (child1Set.contains(*it)) {
            child1.erase(it);
        } else {
            child1Set.insert(*it);
        }
    }
    for (it = child2.begin(); it != child2.end()-1; ++it) {
        if (child2Set.contains(*it)) {
            child2.erase(it);
        } else {
            child2Set.insert(*it);
        }
    }
    


    return {child1, child2};
}

std::pair<std::vector<int>, std::vector<int>> edgeRecombinationCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
    // Placeholder for edge recombination crossover implementation
    return crossover(parent1, parent2); // Fallback to simple crossover for now
}
