#ifndef SPA_PATTERNHANDLER_H
#define SPA_PATTERNHANDLER_H

#include "../EP/ExpressionProcessor.h"
#include "../PKB/PkbGetter.h"
#include "QpsTypeToPkbTypeConvertor.h"
#include "RelationshipRef.h"
#include "Result.h"

class PatternHandler {
private:
    PkbGetter* pg;

    std::set<ProgramElement> handleLeftWildcard(const Entity& right, const Entity& patternType);
    std::set<ProgramElement> handleLeftFixed(const Entity& left, const Entity& right, const Entity& patternType);
    std::set<std::pair<ProgramElement, ProgramElement>> handleLeftVariable(const Entity& left, const Entity& right,
                                                                           const Entity& patternType);

public:
    explicit PatternHandler(PkbGetter* pg);
    Result handlePattern(const RelationshipRef& relRef);

};

#endif //SPA_PATTERNHANDLER_H
