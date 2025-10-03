#ifndef NEARESTNEIGHBOUR_H
#define NEARESTNEIGHBOUR_H

#include <vector>
#include <set>
#include "OrienteeringProblemInputData.h"

int findNearestUnvisitedNode(const std::vector<std::vector<double>>& adjacencyMatrix, double remainingBudget, int currentNode,
    int endNode, std::size_t noOfNodes, const std::set<int>& visited);

std::vector<int> nearestNeighbour(const OrienteeringProblemInputData& input);


#endif //NEARESTNEIGHBOUR_H
