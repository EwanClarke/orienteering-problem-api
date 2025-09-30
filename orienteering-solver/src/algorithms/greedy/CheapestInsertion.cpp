#include "CheapestInsertion.h"
#include "../../OrienteeringProblemInputData.h"
#include <vector>

std::vector<int> CheapestInsertion(const OrienteeringProblemInputData& input){
    return std::vector<int>{input.startNode, input.endNode};
}
