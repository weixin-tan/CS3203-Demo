#ifndef SPA_PKBGETTERSTUB_H
#define SPA_PKBGETTERSTUB_H

#include "PkbGetter.h"
#include "QPSUtil.h"
#include <set>

class PkbGetterStub : public PkbGetter {
private:
    QPS_SOURCE_INFO sourceInfo;
    PATTERN_SOURCE_INFO patternInfo;
public:
    explicit PkbGetterStub(DB *db);
    std::set<ProgramElement*> getEntity(const ElementType& typeToGet) const override;
    std::set<std::pair<ProgramElement*, ProgramElement*>> getRelationshipPairs(const PkbRelationshipType& r, const ElementType& leftTypeToGet, const ElementType& rightTypeToGet) override;
    bool isRelationship(const PkbRelationshipType &r, const ProgramElement &leftSide, const ProgramElement &rightSide) override;
    std::set<ProgramElement*> getLeftSide(const PkbRelationshipType& r, const ProgramElement& rightSide, const ElementType& typeToGet) override;
    std::set<ProgramElement*> getRightSide(const PkbRelationshipType& r, const ProgramElement& leftSide, const ElementType& typeToGet) override;
    std::set<std::pair<ProgramElement*, ProgramElement*>>getWhileWithVariable() const override;
    std::set<std::pair<ProgramElement*, ProgramElement*>>getIfWithVariable() const override;
    std::set<ProgramElement*> getIfGivenVariable(const ProgramElement& variable) const override;
    std::set<ProgramElement*> getWhileGivenVariable(const ProgramElement& variable) const override;
    std::set<ProgramElement*> getAssignmentGivenExpression(const Expr &expr, const ExpressionIndicator &indicator) const override;
    std::set<ProgramElement*> getAssignmentGivenVariableAndExpression(const ProgramElement& variable, const Expr &expr, const ExpressionIndicator &indicator) override;
    std::set<std::pair<ProgramElement*, ProgramElement*>>getAssignmentWithVariableGivenExpression(const Expr &expr, const ExpressionIndicator &indicator) const override;
};


#endif //SPA_PKBGETTERSTUB_H
