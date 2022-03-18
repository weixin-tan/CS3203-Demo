#ifndef SPA_GROUP_H
#define SPA_GROUP_H

#include <vector>

#include "Result.h"

class Group {
private:
    std::vector<Result> results;

public:
    Group();
    void setGroup(std::vector<Result> r);
    std::vector<Result> getGroup();

};


#endif //SPA_GROUP_H
