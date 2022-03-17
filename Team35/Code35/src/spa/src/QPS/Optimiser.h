#ifndef SPA_OPTIMISER_H
#define SPA_OPTIMISER_H

#include "Group.h"
#include "Result.h"

class Optimiser {
public:
    Optimiser();
    std::vector<Group> optimise(std::vector<Result> results);
};


#endif //SPA_OPTIMISER_H
