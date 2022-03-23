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

    Result getNoClauseResult(const Entity& entityToFind) const;

public:
    explicit QPSHandler(PkbGetter* pg);
    std::vector<ResultGroup> processClause(const GroupedClause& groupedClause) const;
};

#endif //SPA_QPSHANDLER_H
