#include "TwoOpt.h"


std::vector<int> twoOptFirstImprovement(const OrienteeringProblemInputData& problemData, const std::vector<int>& path) {
    std::vector<int> improvedPath = path;
    bool improvement = true;

    while (improvement) {
        improvement = false;
        for (size_t i = 1; i < improvedPath.size() - 2; ++i) {
            for (size_t j = i + 1; j < improvedPath.size() - 1; ++j) {
                double currentCost = problemData.adjacencyMatrix[improvedPath[i - 1]][improvedPath[i]] +
                                     problemData.adjacencyMatrix[improvedPath[j]][improvedPath[j + 1]];
                double newCost = problemData.adjacencyMatrix[improvedPath[i - 1]][improvedPath[j]] +
                                 problemData.adjacencyMatrix[improvedPath[i]][improvedPath[j + 1]];

                if (newCost < currentCost) {
                    std::reverse(improvedPath.begin() + i, improvedPath.begin() + j + 1);
                    improvement = true;
                }
            }
        }
    }

    return improvedPath;
}

std::vector<int> twoOptBestImprovement(const OrienteeringProblemInputData& problemData, const std::vector<int>& path) {
    std::vector<int> improvedPath = path;
    bool improvement = true;

    while (improvement) {
        improvement = false;
        double bestImprovement = 0.0;
        size_t bestI = 0, bestJ = 0;

        for (size_t i = 0; i < improvedPath.size() - 3; ++i) {
            for (size_t j = i + 2; j < improvedPath.size() - 2; ++j) {
                double currentCost = problemData.adjacencyMatrix[improvedPath[i]][improvedPath[i + 1]] +
                                     problemData.adjacencyMatrix[improvedPath[j]][improvedPath[j + 1]];
                double newCost = problemData.adjacencyMatrix[improvedPath[i]][improvedPath[j]] +
                                 problemData.adjacencyMatrix[improvedPath[i + 1]][improvedPath[j + 1]];

                double improvementValue = currentCost - newCost;
                if (improvementValue > bestImprovement) {
                    bestImprovement = improvementValue;
                    bestI = i;
                    bestJ = j;
                    improvement = true;
                }
            }
        }

        if (improvement) {
            std::reverse(improvedPath.begin() + bestI, improvedPath.begin() + bestJ + 1);
        }
    }

    return improvedPath;
}