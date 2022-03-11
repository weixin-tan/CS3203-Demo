#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "ProgramElement.h"
#include "DB.h"
#include "PkbRelationshipType.h"

//TODO: FILE NAME CHANGE 
#include "../ExpressionProcessor.h"
#include "../ExpressionIndicator.h"

class PkbGetter {
private:
    DB* db;
    ExpressionProcessor expressionProcessor;

private:
    bool isExists(const ProgramElement& elementToCheck) const;
    std::set<int> getUsesStmtNosGivenConstant(const std::string& c) const;
    static void insertStmtElement(std::set<ProgramElement>& result, const ProgramElement& statement, const ElementType& typeToGet);

public:
    explicit PkbGetter(DB* db);
    bool isRelationship(const PkbRelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getEntity(const ElementType& typeToGet) const;
    std::set<ProgramElement> getLeftSide(const PkbRelationshipType& r, const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightSide(const PkbRelationshipType& r, const ProgramElement& leftSide, const ElementType& typeToGet) const;
    std::set<std::pair<ProgramElement, ProgramElement>> getRelationshipPairs(const PkbRelationshipType& r, const ElementType& leftTypeToGet, const ElementType& rightTypeToGet) const;
    std::set<ProgramElement> getAssignmentGivenExpression(const std::string expr, const ExpressionIndicator indicator) const; // TODO: temporary pattern
    std::set<ProgramElement> getAssignmentGivenVariableAndExpression(const ProgramElement& variable, const std::string expr, const ExpressionIndicator indicator) const;
    std::set<std::pair<ProgramElement, ProgramElement>>getAssignmentWithVariableGivenExpression(const std::string expr, const ExpressionIndicator indicator) const;
    std::set<ProgramElement> getIfGivenVariable(const ProgramElement& variable) const;
    std::set<ProgramElement> getWhileGivenVariable(const ProgramElement& variable) const;


};

#endif //SPA_PKBGETTER_H
