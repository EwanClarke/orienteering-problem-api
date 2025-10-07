#ifndef TWOOPT_H
#define TWOOPT_H

#include "OrienteeringProblemInputData.h"
#include <vector>

std::vector<int> twoOptFirstImprovement(const OrienteeringProblemInputData& problemData, const std::vector<int>& path);

std::vector<int> twoOptBestImprovement(const OrienteeringProblemInputData& problemData, const std::vector<int>& path);

#endif //TWOOPT_H