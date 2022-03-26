#ifndef SPA_RESULTPROCESSOR_H
#define SPA_RESULTPROCESSOR_H

#include <vector>
#include <iostream>

#include "Result.h"
#include "ResultGroup.h"
#include "Result.h"
#include "Table.h"

class ResultProcessor {
public:
    std::vector<ProgramElement> processResults(std::vector<ResultGroup> groups);

private:
    Table buildIntermediateTable(std::vector<Result> results);
    Table buildFinalTable(std::vector<Table> tables);
};

#endif //SPA_RESULTPROCESSOR_H
