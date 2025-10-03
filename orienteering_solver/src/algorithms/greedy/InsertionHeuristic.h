//
// Created by ewano on 01/10/2025.
//

#ifndef INSERTIONHEURISTIC_H
#define INSERTIONHEURISTIC_H

#include <vector>
#include <set>
#include "OrienteeringProblemInputData.h"
#include <functional>

struct Insertion {
    int insertionLocation;
    int insertionNode;
    double insertionDelta;
};

Insertion findCheapestInsertion(const OrienteeringProblemInputData &problemData, std::vector<int> path, std::vector<bool> visited, double remainingBudget);
Insertion findMostProfitableInsertion(const OrienteeringProblemInputData &problemData, std::vector<int> path, std::vector<bool> visited, double remainingBudget);
Insertion findGreedyInsertion(const OrienteeringProblemInputData &problemData, std::vector<int> path, std::vector<bool> visited, double remainingBudget);

using InsertionNodeFunction = std::function<
    Insertion(const OrienteeringProblemInputData &problemData, std::vector<int> path, std::vector<bool> visited, double remainingBudget)
>;

std::vector<int> insertionHeuristic(const OrienteeringProblemInputData& input, InsertionNodeFunction insertionHeuristicFunction);

#endif //INSERTIONHEURISTIC_H
