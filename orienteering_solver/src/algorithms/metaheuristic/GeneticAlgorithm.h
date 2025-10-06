
#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <vector>
#include "OrienteeringProblemInputData.h"

std::vector<int> geneticAlgorithm(const OrienteeringProblemInputData& input, int populationSize, int generations, double mutationRate);

double evaluateFitness(const std::vector<int>& individual, const OrienteeringProblemInputData& input);

std::vector<std::vector<int>> generateInitialPopulation(const OrienteeringProblemInputData& input, int populationSize);
std::vector<int> generateIndividual(const OrienteeringProblemInputData& input);

std::pair<std::vector<int>, std::vector<int>> selectParents(const std::vector<std::vector<int>>& population, const OrienteeringProblemInputData& input);
int rouletteWheelSelection(const std::vector<int>& fitnessScores, int totalFitness);

std::pair<std::vector<int>, std::vector<int>> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2);
std::pair<std::vector<int>, std::vector<int>> orderCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2);
std::pair<std::vector<int>, std::vector<int>> pmxCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2);
std::pair<std::vector<int>, std::vector<int>> cycleCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2);
std::pair<std::vector<int>, std::vector<int>> edgeRecombinationCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2);

void mutate(std::pair<std::vector<int>, std::vector<int>>& individual, double mutationRate, const OrienteeringProblemInputData& input);

std::vector<int> selectBestIndividual(const std::vector<std::vector<int>>& population, const OrienteeringProblemInputData& input);


#endif //GENETICALGORITHM_H