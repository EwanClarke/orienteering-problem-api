#ifndef ORIENTEERINGPROBLEMINPUTDATA_H
#define ORIENTEERINGPROBLEMINPUTDATA_H
#include <vector>

struct OrienteeringProblemInputData {
    std::vector<std::vector<double>> adjacencyMatrix;
    std::vector<double> profits;
    double budget;
    int startNode;
    int endNode;
};

#endif //ORIENTEERINGPROBLEMINPUTDATA_H
