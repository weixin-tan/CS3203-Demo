#ifndef SPA_PKBGETTERSTUBFORTESTPATTERNHANDLER_H
#define SPA_PKBGETTERSTUBFORTESTPATTERNHANDLER_H


#include "PkbGetter.h"

class PkbGetterStubForTestPatternHandler : public PkbGetter {
public:
    explicit PkbGetterStubForTestPatternHandler(DB *db);

    //bool isRelationship(const PkbRelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide);
    std::set<ProgramElement> getEntity(const ElementType& typeToGet) const override;
    std::set<ProgramElement> getLeftSide(const PkbRelationshipType& r, const ProgramElement& rightSide, const ElementType& typeToGet) override;
    //std::set<ProgramElement> getRightSide(const PkbRelationshipType& r, const ProgramElement& leftSide, const ElementType& typeToGet);
    std::set<std::pair<ProgramElement, ProgramElement>> getRelationshipPairs(const PkbRelationshipType& r, const ElementType& leftTypeToGet, const ElementType& rightTypeToGet) override;
    //std::set<ProgramElement> getAssignmentGivenExpression(const Expr expr, const ExpressionIndicator indicator) const;
    //std::set<ProgramElement> getAssignmentGivenVariableAndExpression(const ProgramElement& variable, const Expr expr, const ExpressionIndicator indicator);
    //std::set<std::pair<ProgramElement, ProgramElement>>getAssignmentWithVariableGivenExpression(const Expr expr, const ExpressionIndicator indicator) const;
    std::set<ProgramElement> getIfGivenVariable(const ProgramElement& variable) const override;
    std::set<ProgramElement> getWhileGivenVariable(const ProgramElement& variable) const override;
    std::set<std::pair<ProgramElement, ProgramElement>>getIfWithVariable() const override;
    std::set<std::pair<ProgramElement, ProgramElement>>getWhileWithVariable() const override;

};


#endif //SPA_PKBGETTERSTUBFORTESTPATTERNHANDLER_H
