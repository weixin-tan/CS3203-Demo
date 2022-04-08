#ifndef SPA_QPSHANDLER_H
#define SPA_QPSHANDLER_H

#include "Clause.h"
#include "PatternHandler.h"
#include "../PKB/PkbGetter.h"
#include "QpsTypeToPkbTypeConvertor.h"
#include "Result.h"
#include "SuchThatHandler.h"
#include "WithHandler.h"
#include "RelationshipRefGroup.h"
#include "GroupedClause.h"
#include "FormattedResult.h"

class QPSHandler {
private:
    PkbGetter* pg;
    SuchThatHandler* suchThatHandler;
    PatternHandler* patternHandler;
    WithHandler* withHandler;

    std::vector<Result> getNoClauseResults(const std::vector<Entity>& entitiesToFind) const;
    std::vector<Result> handleRelRefGroups(const RelationshipRefGroup &relRefGroup) const;

public:
    explicit QPSHandler(PkbGetter* pg);
    FormattedResult processClause(const GroupedClause& groupedClause) const;

    Table buildIntermediateTable(const std::vector<Result> &results) const;

    std::set<Entity> extractEntitySet(const std::vector<Entity> &entityList) const;

    std::vector<Entity> getNecessaryEntities(const std::set<Entity> &tableEntities, const TableRow &row) const;

    std::vector<Table>
    extractNecessaryTables(const std::vector<Table> &intermediateTables, const std::set<Entity> &tableEntities) const;

    Table buildFinalTable(const std::vector<Table> &tables) const;

    std::set<Entity> extractTableEntities(const TableRow &tableRow) const;

    std::set<Entity>
    findMissingEntities(const std::set<Entity> &returnEntities, const std::set<Entity> &finalTableEntities) const;

    std::vector<Result> findMissingResults(const std::set<Entity> &entities) const;

    FormattedResult handleZeroClause(const std::vector<Entity> &entitiesToFind) const;

    Result getAllElements(const Entity &e) const;
};

#endif //SPA_QPSHANDLER_H
