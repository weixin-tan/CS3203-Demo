#ifndef SPA_OPTIMISER_H
#define SPA_OPTIMISER_H

#include "Result.h"
#include "Group.h"

class Optimiser {
public:
    Optimiser();
    std::vector<Group> optimize(std::vector<Result> results);
};


#endif //SPA_OPTIMISER_H
