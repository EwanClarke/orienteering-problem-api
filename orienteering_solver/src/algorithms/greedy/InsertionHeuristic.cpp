//
// Created by ewano on 01/10/2025.
//

#include "InsertionHeuristic.h"

std::vector<int> insertionHeuristic(const OrienteeringProblemInputData& input, InsertionNodeFunction insertionHeuristicFunction) {
    std::vector<int> path = {input.startNode, input.endNode};
    int remainingBudget = input.budget;

    const int noOfNodes = input.profits.size();
    std::vector<bool> visited(noOfNodes, false);
    visited[input.startNode] = true;
    visited[input.endNode] = true;

    while (true) {
        auto [insertionLocation, insertionNode, insertionDelta] = insertionHeuristicFunction(input, path, visited, remainingBudget);
        if (insertionNode == -1) {
            break;
        }

        path.insert(path.begin() + insertionLocation, insertionNode);
        visited[insertionNode] = true;
        remainingBudget -= insertionDelta;
    }

    return path;
}

Insertion findCheapestInsertion(const OrienteeringProblemInputData &problemData, std::vector<int> path, std::vector<bool> visited, double remainingBudget) {
    auto cheapestInsertion = Insertion(-1, -1, std::numeric_limits<double>::infinity());
    for (int node = 0; node < visited.size(); ++node) {
        if (visited[node]) {
            continue;
        }

        for (int insertionLocation = 1; insertionLocation < path.size(); ++insertionLocation) {
            double insertionDelta = 
                problemData.adjacencyMatrix[path[insertionLocation - 1]][node] +
                problemData.adjacencyMatrix[node][path[insertionLocation]] -
                problemData.adjacencyMatrix[path[insertionLocation - 1]][path[insertionLocation]];

            if (insertionDelta <= remainingBudget && insertionDelta < cheapestInsertion.insertionDelta) {
                cheapestInsertion = Insertion(insertionLocation, node, insertionDelta);
            }
        }
    }
    return cheapestInsertion;
}

Insertion findMostProfitableInsertion(const OrienteeringProblemInputData &problemData, std::vector<int> path, std::vector<bool> visited, double remainingBudget) {
    auto mostProfitableInsertion = Insertion(-1, -1, -std::numeric_limits<double>::infinity());
    double highestProfit = 0.0;
    for (int node = 0; node < visited.size(); ++node) {
        if (visited[node]) {
            continue;
        }

        for (int insertionLocation = 1; insertionLocation < path.size(); ++insertionLocation) {
            double insertionDelta = 
                problemData.adjacencyMatrix[path[insertionLocation - 1]][node] +
                problemData.adjacencyMatrix[node][path[insertionLocation]] -
                problemData.adjacencyMatrix[path[insertionLocation - 1]][path[insertionLocation]];

            if (insertionDelta <= remainingBudget) {
                double profit = problemData.profits[node] - insertionDelta;
                if (profit > highestProfit) {
                    highestProfit = profit;
                    mostProfitableInsertion = Insertion(insertionLocation, node, insertionDelta);
                }
            }
        }
    }
    
    return mostProfitableInsertion;
}

Insertion findGreedyInsertion(const OrienteeringProblemInputData &problemData, std::vector<int> path, std::vector<bool> visited, double remainingBudget) {
    auto greedyInsertion = Insertion(-1, -1, -std::numeric_limits<double>::infinity());
    double bestRatio = 0.0;
    for (int node = 0; node < visited.size(); ++node) {
        if (visited[node]) {
            continue;
        }

        for (int insertionLocation = 1; insertionLocation < path.size(); ++insertionLocation) {
            double insertionDelta = 
                problemData.adjacencyMatrix[path[insertionLocation - 1]][node] +
                problemData.adjacencyMatrix[node][path[insertionLocation]] -
                problemData.adjacencyMatrix[path[insertionLocation - 1]][path[insertionLocation]];

            if (insertionDelta <= remainingBudget) {
                double ratio = problemData.profits[node] / insertionDelta;
                if (ratio > bestRatio) {
                    bestRatio = ratio;
                    greedyInsertion = Insertion(insertionLocation, node, insertionDelta);
                }
            }
        }
    }
    return greedyInsertion;
}
