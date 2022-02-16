#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "ProgramElement.h"
#include "DB.h"

class PkbGetter {
private:
    DB* db;

private:
    bool isExists(const ProgramElement& elementToCheck) const;

public:
    explicit PkbGetter(DB* db);
    bool isRelationship(const RelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getEntity(const ElementType& typeToGet) const;
    std::set<ProgramElement> getRelationshipStatements(const RelationshipType& r) const;
    std::set<ProgramElement> getLeftSide(const RelationshipType& r, const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightSide(const RelationshipType& r, const ProgramElement& leftSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getAssignmentGivenExpression(const ProgramElement& expression) const; // TODO: temporary pattern
    std::set<ProgramElement> getAssignmentGivenVariableAndExpression(const ProgramElement& variable, const ProgramElement& expression) const;
};

#endif //SPA_PKBGETTER_H
