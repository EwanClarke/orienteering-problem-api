
#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <vector>
#include "OrienteeringProblemInputData.h"
#include "InsertionHeuristic.h"
#include "TwoOpt.h"
#include "utils.h"

std::vector<int> geneticAlgorithm(const OrienteeringProblemInputData& input, int populationSize, int generations, double mutationRate);

std::vector<std::vector<int>> generateInitialPopulation(const OrienteeringProblemInputData& problemData, int populationSize);
std::vector<int> generateIndividual(const OrienteeringProblemInputData& problemData);

std::pair<std::vector<int>, std::vector<int>> selectParents(const std::vector<std::vector<int>>& population, const OrienteeringProblemInputData& problemData);
int rouletteWheelSelection(const std::vector<double>& fitnessScores, double totalFitness);

std::vector<int> edgeRecombinationCrossover(const OrienteeringProblemInputData& problemData, const std::vector<int>& parent1, const std::vector<int>& parent2);

std::vector<int> insertionImprovement(const OrienteeringProblemInputData& problemData, const std::vector<int>& individual);

void mutate(const OrienteeringProblemInputData& problemData, std::vector<int>& individual, double mutationRate);

std::vector<int> selectBestIndividual(const OrienteeringProblemInputData& problemData, const std::vector<std::vector<int>>& population);


#endif //GENETICALGORITHM_H