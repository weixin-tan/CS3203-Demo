#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "ProgramElement.h"
#include "DB.h"
#include "PkbRelationshipType.h"
#include "ModifiesGetter.h"
#include "UsesGetter.h"
#include "FollowsGetter.h"
#include "FollowsTGetter.h"
#include "ParentGetter.h"
#include "ParentTGetter.h"

class PkbGetter {
private:
    DB* db;
    ModifiesGetter modifiesGetter;
    UsesGetter usesGetter;
    FollowsGetter followsGetter;
    FollowsTGetter followsTGetter;
    ParentGetter parentGetter;
    ParentTGetter parentTGetter;

private:
    bool isExists(const ProgramElement& elementToCheck) const;

public:
    explicit PkbGetter(DB* db);
    bool isRelationship(const PkbRelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getEntity(const ElementType& typeToGet) const;
    std::set<ProgramElement> getLeftSide(const PkbRelationshipType& r, const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightSide(const PkbRelationshipType& r, const ProgramElement& leftSide, const ElementType& typeToGet) const;
    std::set<std::pair<ProgramElement, ProgramElement>> getRelationshipPairs(const PkbRelationshipType& r, const ElementType& leftTypeToGet, const ElementType& rightTypeToGet) const;
    std::set<ProgramElement> getAssignmentGivenExpression(const ProgramElement& expression) const; // TODO: temporary pattern
    std::set<ProgramElement> getAssignmentGivenVariableAndExpression(const ProgramElement& variable, const ProgramElement& expression) const;
};

#endif //SPA_PKBGETTER_H
