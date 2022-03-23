#ifndef SPA_RESULTPROCESSOR_H
#define SPA_RESULTPROCESSOR_H

#include "Result.h"
#include "ResultGroup.h"
#include <vector>
#include <iostream>
#include "Result.h"

class ResultProcessor {
public:
    ResultProcessor();
    std::vector<ProgramElement> processResults(std::vector<ResultGroup> groups);
    int getIndexEntity(std::vector<Entity> v, Entity e);

    std::vector<ProgramElement> setToVector(std::set<ProgramElement> s);
    std::vector<std::pair<ProgramElement, ProgramElement>> setPairToVectorPair(std::set<std::pair<ProgramElement, ProgramElement>> s);
};

#endif //SPA_RESULTPROCESSOR_H
