#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "ProgramElement.h"
#include "DB.h"
#include "PkbRelationshipType.h"

class PkbGetter {
private:
    DB* db;

private:
    bool isExists(const ProgramElement& elementToCheck) const;
    std::set<int> getUsesStmtNosGivenConstant(const std::string& c) const;
    ElementType getStmtType(int stmtNo) const;

public:
    explicit PkbGetter(DB* db);
    bool isRelationship(const PkbRelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getEntity(const ElementType& typeToGet) const;
    std::set<ProgramElement> getLeftSide(const PkbRelationshipType& r, const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightSide(const PkbRelationshipType& r, const ProgramElement& leftSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getAssignmentGivenExpression(const ProgramElement& expression) const; // TODO: temporary pattern
    std::set<ProgramElement> getAssignmentGivenVariableAndExpression(const ProgramElement& variable, const ProgramElement& expression) const;
};

#endif //SPA_PKBGETTER_H
