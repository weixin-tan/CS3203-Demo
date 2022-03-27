#ifndef SPA_OPTIMISER_H
#define SPA_OPTIMISER_H

#include "../QPS/ResultGroup.h"
#include "Result.h"

class Optimiser {
public:
    Optimiser();
    std::vector<ResultGroup> optimise(std::vector<ResultGroup> resultsGroups);
};

#endif //SPA_OPTIMISER_H
