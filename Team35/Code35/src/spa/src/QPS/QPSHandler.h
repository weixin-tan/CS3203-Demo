#ifndef SPA_QPSHANDLER_H
#define SPA_QPSHANDLER_H

#include "Clause.h"
#include "PatternHandler.h"
#include "../PKB/PkbGetter.h"
#include "QpsTypeToPkbTypeConvertor.h"
#include "Result.h"
#include "ResultGroup.h"
#include "SuchThatHandler.h"
#include "WithHandler.h"
#include "RelationshipRefGroup.h"
#include "GroupedClause.h"

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
    ResultGroup processClause(const GroupedClause& groupedClause) const;

};

#endif //SPA_QPSHANDLER_H
