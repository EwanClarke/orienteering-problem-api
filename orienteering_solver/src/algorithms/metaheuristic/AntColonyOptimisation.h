
#ifndef ANTCOLONYOPTIMISATION_H
#define ANTCOLONYOPTIMISATION_H

#include <vector>
#include <set>
#include "OrienteeringProblemInputData.h"
#include <functional>
#include <limits>
#include <unordered_map>

std::vector<int> antColonyOptimisation(const OrienteeringProblemInputData& input, int noOfAnts, int noOfIterations, double alpha, double beta, double evaporationRate, double initialPheromone);

std::vector<int> antTraversal(const OrienteeringProblemInputData& problemData, const std::vector<std::vector<double>>& pheromoneLevels, double alpha, double beta);

std::unordered_map<int, double> calculateWeights(const OrienteeringProblemInputData& problemData, int currentNode, const std::set<int>& visited, const std::vector<std::vector<double>>& pheromoneLevels, double alpha, double beta, double remainingBudget);

int nextNode(const OrienteeringProblemInputData& problemData, int currentNode, const std::set<int>& visited, const std::vector<std::vector<double>>& pheromoneLevels, double alpha, double beta, double remainingBudget);

double evaluatePath(const OrienteeringProblemInputData& problemData, const std::vector<int>& path);

void depositPheromones(std::vector<std::vector<double>>& pheromoneLevels, const std::vector<int>& path, double Q);

void evaporatePheromones(std::vector<std::vector<double>>& pheromoneLevels, double evaporationRate);


#endif //ANTCOLONYOPTIMISATION_H
