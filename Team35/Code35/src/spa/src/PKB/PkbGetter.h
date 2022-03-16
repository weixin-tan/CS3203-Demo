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
#include "CallsGetter.h"
#include "CallsTGetter.h"
#include "NextGetter.h"

//TODO: FILE NAME CHANGE 
#include "../ExpressionProcessor.h"
#include "../ExpressionIndicator.h"
#include "NextTGetter.h"

class PkbGetter {
private:
    DB* db;
    ModifiesGetter modifiesGetter;
    UsesGetter usesGetter;
    FollowsGetter followsGetter;
    FollowsTGetter followsTGetter;
    ParentGetter parentGetter;
    ParentTGetter parentTGetter;
    CallsGetter callsGetter;
    CallsTGetter callsTGetter;
    NextGetter nextGetter;
    NextTGetter nextTGetter;
    ExpressionProcessor expressionProcessor;

private:
    bool isExists(const ProgramElement& elementToCheck) const;

public:
    explicit PkbGetter(DB* db);
    bool isRelationship(const PkbRelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide);
    std::set<ProgramElement> getEntity(const ElementType& typeToGet) const;
    std::set<ProgramElement> getLeftSide(const PkbRelationshipType& r, const ProgramElement& rightSide, const ElementType& typeToGet);
    std::set<ProgramElement> getRightSide(const PkbRelationshipType& r, const ProgramElement& leftSide, const ElementType& typeToGet);
    std::set<std::pair<ProgramElement, ProgramElement>> getRelationshipPairs(const PkbRelationshipType& r, const ElementType& leftTypeToGet, const ElementType& rightTypeToGet);
    std::set<ProgramElement> getAssignmentGivenExpression(const Expr expr, const ExpressionIndicator indicator) const;
    std::set<ProgramElement> getAssignmentGivenVariableAndExpression(const ProgramElement& variable, const Expr expr, const ExpressionIndicator indicator);
    std::set<std::pair<ProgramElement, ProgramElement>>getAssignmentWithVariableGivenExpression(const Expr expr, const ExpressionIndicator indicator) const;
    std::set<ProgramElement> getIfGivenVariable(const ProgramElement& variable) const;
    std::set<ProgramElement> getWhileGivenVariable(const ProgramElement& variable) const;
    std::set<std::pair<ProgramElement, ProgramElement>>getIfWithVariable() const;
    std::set<std::pair<ProgramElement, ProgramElement>>getWhileWithVariable() const;



};

#endif //SPA_PKBGETTER_H
