#include "utils.h"


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
        // Return a large negative finite value to indicate invalid/over-budget path
        return -1e9;
    }
    return totalProfit;
}