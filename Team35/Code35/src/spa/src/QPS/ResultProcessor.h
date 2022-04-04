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
<<<<<<< Updated upstream
    std::set<Entity> extractEntitySet(std::vector<Result> resultList);
    FormattedResult handleInvalidResult(Result r);
    FormattedResult handleZeroClause(std::vector<Result> resultList);
    FormattedResult extractTableInformation(std::vector<Entity> entities, Table table);
    std::vector<Entity> extractTableEntities(Table table);
    std::vector<Entity> extractOrderedEntities(std::vector<Result> results);
=======
    std:: set <Entity> extractEntitySet(std::vector <Result> resultList);
    FormattedResult handleInvalidResult(Result r);
    FormattedResult handleZeroClause(std::vector <Result> resultList);
    FormattedResult extractTableInformation(std::vector<Entity> entities, Table table);
    std::vector <Entity> extractTableEntities(Table table);

    std::vector <Entity> extractOrderedEntities(std::vector <Result> results);
>>>>>>> Stashed changes
};

#endif //SPA_RESULTPROCESSOR_H
