#ifndef SPA_RESULTPROCESSOR_H
#define SPA_RESULTPROCESSOR_H

#include "Result.h"
#include "ResultGroup.h"
#include <vector>
#include <iostream>
#include "Result.h"

class ResultProcessor {
public:
    std::vector<ProgramElement> processResults(std::vector<Group> groups);
};

#endif //SPA_RESULTPROCESSOR_H
