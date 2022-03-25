#ifndef SPA_GROUP_H
#define SPA_GROUP_H

#include <vector>

#include "Result.h"

class ResultGroup {
private:
    std::vector<Result> results;

public:
    ResultGroup();
    void setGroup(std::vector<Result> r);
    std::vector<Result> getGroup();
};

#endif //SPA_GROUP_H
