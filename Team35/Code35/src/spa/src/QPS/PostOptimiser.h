#ifndef SPA_POSTOPTIMISER_H
#define SPA_POSTOPTIMISER_H

#include "../QPS/ResultGroup.h"
#include "Result.h"

class PostOptimiser {
public:
    PostOptimiser();
    std::vector<ResultGroup> optimise(std::vector<ResultGroup> resultsGroups);
};

#endif //SPA_POSTOPTIMISER_H
