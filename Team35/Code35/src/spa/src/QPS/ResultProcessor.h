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
    FormattedResult processResults(const ResultGroup& resultGroup);

private:
    FormattedResult handleZeroClause(const std::vector<Entity>& entities, const std::vector<Result>& resultList);
    std::vector<Result> scrubResultList(const std::vector<Result>& resultList);
    Table buildIntermediateTable(const std::vector<Result>& results);
    std::vector<Table> buildAllIntermediateTables(const std::vector<std::vector<Result>>& resultLists);
    std::set<Entity> extractEntitySet(const std::vector<Entity>& entityList);
    std::vector<Entity> getNecessaryEntities(const std::set<Entity>& tableEntities, const TableRow& row);
    std::vector<Table> extractNecessaryTables(const std::vector<Table>& intermediateTables,
                                              const std::set<Entity>& tableEntities);
    Table buildFinalTable(const std::vector<Table>& tables);
    std::set<Entity> extractTableEntities(const TableRow& tableRow);
    std::set<Entity> findMissingEntities(const std::set<Entity>& returnEntities,
                                         const std::set<Entity>& finalTableEntities);
    std::vector<Result> findMissingResults(const std::set<Entity>& entities, const std::vector<Result>& results);

};

#endif //SPA_RESULTPROCESSOR_H
