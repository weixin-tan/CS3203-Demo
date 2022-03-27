#ifndef SPA_RESULTPROCESSOR_H
#define SPA_RESULTPROCESSOR_H

#include <vector>
#include <iostream>

#include "FormattedResult.h"
#include "Result.h"
#include "ResultGroup.h"
#include "Result.h"
#include "Table.h"

class ResultProcessor {
public:
    FormattedResult processResults(std::vector<ResultGroup> groups);

private:
    Table buildIntermediateTable(std::vector<Result> results);
    Table buildFinalTable(std::vector<Table> tables);
    set <Entity> extractEntities(vector <Result> resultList);
    FormattedResult handleInvalidResult(Result r);
    FormattedResult handleZeroClause(vector <Result> resultList);
    FormattedResult extractTableInformation(Table table);
    vector <Entity> extractTableEntities(Table table);
};

#endif //SPA_RESULTPROCESSOR_H
