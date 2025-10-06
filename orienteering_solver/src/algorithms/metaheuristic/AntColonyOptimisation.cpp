
#include "AntColonyOptimisation.h"
#include <vector>
#include <limits>
#include <unordered_map>

std::vector<int> antColonyOptimisation(const OrienteeringProblemInputData& input, int noOfAnts, int noOfIterations, double alpha, double beta, double evaporationRate, double initialPheromone) {
    std::vector<std::vector<double>> pheromoneLevels(input.profits.size(), std::vector<double>(input.profits.size(), initialPheromone));
    std::vector<int> bestPath;
    double bestValue = -std::numeric_limits<double>::infinity();
    double Q = 1.0;
    for (int iteration = 0; iteration < noOfIterations; ++iteration) {
        for (int ant = 0; ant < noOfAnts; ++ant) {
            std::vector<int> path = antTraversal(input, pheromoneLevels, alpha, beta);
            double pathValue = evaluatePath(input, path);
            if (bestPath.empty() || pathValue > bestValue) {
                bestPath = path;
                bestValue = pathValue;
            }
            depositPheromones(pheromoneLevels, path, Q * pathValue); // More profit -> more pheromone
        }
        evaporatePheromones(pheromoneLevels, evaporationRate);
    }

    return bestPath;
}

std::vector<int> antTraversal(const OrienteeringProblemInputData& problemData, const std::vector<std::vector<double>>& pheromoneLevels, double alpha, double beta) {
    std::set<int> visited = {problemData.startNode, problemData.endNode};
    int currentNode = problemData.startNode;
    std::vector<int> antPath = {currentNode};
    double remainingBudget = problemData.budget;
    while (true) {
        int next = nextNode(problemData, currentNode, visited, pheromoneLevels, alpha, beta, remainingBudget);
        if (next == -1) {
            break;
        }

        double travelCost = problemData.adjacencyMatrix[currentNode][next];
        if (travelCost > remainingBudget) {
            break;
        }

        antPath.push_back(next);
        visited.insert(next);
        remainingBudget -= travelCost;
        currentNode = next;
    }
    antPath.push_back(problemData.endNode);

    return antPath;
}

std::unordered_map<int, double> calculateWeights(const OrienteeringProblemInputData& problemData, int currentNode, const std::set<int>& visited, const std::vector<std::vector<double>>& pheromoneLevels, double alpha, double beta, double remainingBudget) {
    std::unordered_map<int, double> weights;
    for (int node = 0; node < problemData.profits.size(); ++node) {
        if (visited.find(node) != visited.end()) {
            continue;
        } else if (problemData.adjacencyMatrix[currentNode][node] == std::numeric_limits<double>::infinity()) {
            continue; // No edge
        } else if (problemData.adjacencyMatrix[currentNode][node] + problemData.adjacencyMatrix[node][problemData.endNode] > remainingBudget) {
            continue; // Can't afford
        }

        double pheromone = pheromoneLevels[currentNode][node];
        double heuristic = problemData.profits[node] / (problemData.adjacencyMatrix[currentNode][node] + 1e-10); // Avoid division by zero
        double weight = std::pow(pheromone, alpha) * std::pow(heuristic, beta);
        weights[node] = weight;
    }
    return weights;
}

int nextNode(const OrienteeringProblemInputData& problemData, int currentNode, const std::set<int>& visited, const std::vector<std::vector<double>>& pheromoneLevels, double alpha, double beta, double remainingBudget) {
    std::unordered_map<int, double> weights = calculateWeights(problemData, currentNode, visited, pheromoneLevels, alpha, beta, remainingBudget);
    double totalWeight = 0.0;
    for (const auto& [node, weight] : weights) {
        totalWeight += weight;
    }
    if (totalWeight == 0.0) {
        return -1; // No valid next node
    }
    double randValue = static_cast<double>(rand()) / RAND_MAX * totalWeight;
    double cumulativeWeight = 0.0;
    for (const auto& [node, weight] : weights) {
        cumulativeWeight += weight;
        if (randValue <= cumulativeWeight) {
            return node;
        }
    }
    return -1; // Fallback, should not reach here
}

double evaluatePath(const OrienteeringProblemInputData& problemData, const std::vector<int>& path) {
    double totalProfit = 0.0;
    double totalCost = 0.0;

    for (size_t i = 0; i < path.size() - 1; ++i) {
        int from = path[i];
        int to = path[i + 1];
        totalCost += problemData.adjacencyMatrix[from][to];
    }

    for (size_t i = 1; i < path.size() - 1; ++i) {
        totalProfit += problemData.profits[path[i]];
    }

    if (totalCost > problemData.budget) {
        return -std::numeric_limits<double>::infinity(); // Invalid path
    }
    return totalProfit;
}

void depositPheromones(std::vector<std::vector<double>>& pheromoneLevels, const std::vector<int>& path, double Q) {
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int from = path[i];
        int to = path[i + 1];
        pheromoneLevels[from][to] += Q;
        pheromoneLevels[to][from] += Q; // Assuming undirected graph
    }
}

void evaporatePheromones(std::vector<std::vector<double>>& pheromoneLevels, double evaporationRate) {
    for (auto& row : pheromoneLevels) {
        for (auto& level : row) {
            level *= (1.0 - evaporationRate);
        }
    }
}



