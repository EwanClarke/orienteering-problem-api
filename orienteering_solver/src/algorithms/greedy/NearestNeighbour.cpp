#include "../../OrienteeringProblemInputData.h"
#include <set>
#include <vector>
#include <limits>
#include "NearestNeighbour.h"

int findNearestUnvisitedNode(const std::vector<std::vector<double>>& adjacencyMatrix, const double remainingBudget, const int currentNode,
    const int endNode, const std::size_t noOfNodes, const std::set<int>& visited) {

    int currentLowest = -1;
    double lowestDistance = std::numeric_limits<double>::infinity();

    for (int i = 0; i < noOfNodes; ++i) {
        if (visited.contains(i)) {
            continue;
        }

        if (const double distance = adjacencyMatrix[currentNode][i];
            distance + adjacencyMatrix[i][endNode] <= remainingBudget && distance < lowestDistance) {
            currentLowest = i;
            lowestDistance = distance;
            }
    }
    return currentLowest;
}

std::vector<int> nearestNeighbour(const OrienteeringProblemInputData& input) {

    double remainingBudget = input.budget;
    std::vector<int> path = {input.startNode};
    std::set<int> visited = std::set<int>{input.startNode, input.endNode};
    const std::size_t noOfNodes = input.adjacencyMatrix.size();

    int nextNode = findNearestUnvisitedNode(input.adjacencyMatrix, remainingBudget, input.startNode, input.endNode, noOfNodes, visited);
    while (nextNode != -1) {
        remainingBudget -= input.adjacencyMatrix[path.back()][nextNode];
        path.emplace_back(nextNode);
        visited.insert(nextNode);
        nextNode = findNearestUnvisitedNode(input.adjacencyMatrix, remainingBudget, path.back(), input.endNode, noOfNodes, visited);
    }
    path.emplace_back(input.endNode);

    return path;
}
