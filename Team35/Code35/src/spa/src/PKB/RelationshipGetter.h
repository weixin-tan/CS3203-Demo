#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_RELATIONSHIPGETTER_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_RELATIONSHIPGETTER_H_

#include <set>

#include "ProgramElement.h"
#include "DB.h"

class RelationshipGetter {

public:
    static bool isStatementTypeToGet(const ElementType& typeToGet, const ElementType& targetType);
    static void insertStmtElement(std::set<ProgramElement*>& result, int stmtNo, const ElementType& typeToGet);

    virtual bool isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) = 0;
    virtual std::set<ProgramElement*> getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) = 0;
    virtual std::set<ProgramElement*> getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) = 0;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_RELATIONSHIPGETTER_H_
