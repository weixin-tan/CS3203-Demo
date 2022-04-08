#include <utility>
#include "PkbGetterStub.h"

PkbGetterStub::PkbGetterStub(DB *db) : PkbGetter(db) {}

std::set<ProgramElement*> PkbGetterStub::getEntity(const ElementType& typeToGet) const {

    if (typeToGet == ElementType::PROCEDURE) {
        return sourceInfo.procedures;
    }

    if (typeToGet == ElementType::CALL) {
        return sourceInfo.calls;
    }

    if (typeToGet == ElementType::VARIABLE) {
        return sourceInfo.variables;
    }

    if (typeToGet == ElementType::WHILE) {
        return sourceInfo.whiles;
    }

    if (typeToGet == ElementType::READ) {
        return sourceInfo.reads;
    }

    if (typeToGet == ElementType::PRINT) {
        return sourceInfo.prints;
    }

    if (typeToGet == ElementType::CONSTANT) {
        return sourceInfo.constants;
    }

    if (typeToGet == ElementType::IF) {
        return sourceInfo.ifs;
    }

    if (typeToGet == ElementType::STATEMENT) {
        return sourceInfo.stmts;
    }

    if (typeToGet == ElementType::ASSIGNMENT) {
        return sourceInfo.assigns;
    }

    return {};
}

std::set<std::pair<ProgramElement*, ProgramElement*>>
PkbGetterStub::getRelationshipPairs(const PkbRelationshipType &r, const ElementType &leftTypeToGet,
                                                          const ElementType &rightTypeToGet) {
    std::set<std::pair<ProgramElement, ProgramElement>> result;
    if (r == PkbRelationshipType::MODIFIES && leftTypeToGet == ElementType::PROCEDURE && rightTypeToGet == ElementType::VARIABLE) {
        return {{&sourceInfo.procF, &sourceInfo.varX}, {&sourceInfo.procF, &sourceInfo.varY}, {&sourceInfo.procF, &sourceInfo.varZ}};
    }
    if (r == PkbRelationshipType::MODIFIES && leftTypeToGet == ElementType::READ && rightTypeToGet == ElementType::VARIABLE) {
        return {{&sourceInfo.read5, &sourceInfo.varZ}};
    }
    if (r == PkbRelationshipType::MODIFIES && leftTypeToGet ==ElementType::PROCEDURE && rightTypeToGet == ElementType::STATEMENT) {
        return {{&sourceInfo.procF, &sourceInfo.stmt1}, {&sourceInfo.procF, &sourceInfo.stmt2}, {&sourceInfo.procF, &sourceInfo.stmt3},
                {&sourceInfo.procF, &sourceInfo.stmt4}, {&sourceInfo.procF, &sourceInfo.stmt5}, {&sourceInfo.procF, &sourceInfo.stmt6},
                {&sourceInfo.procF, &sourceInfo.stmt7}, {&sourceInfo.procF, &sourceInfo.stmt8}, {&sourceInfo.procF, &sourceInfo.stmt9},
                {&sourceInfo.procF, &sourceInfo.stmt10}, {&sourceInfo.procF, &sourceInfo.stmt11}, {&sourceInfo.procF, &sourceInfo.stmt12}};
    }
    if (r == PkbRelationshipType::MODIFIES && leftTypeToGet == ElementType::STATEMENT && rightTypeToGet == ElementType::VARIABLE) {
        return {{&sourceInfo.stmt1, &sourceInfo.varX}, {&sourceInfo.stmt2, &sourceInfo.varY}, {&sourceInfo.stmt3, &sourceInfo.varX},
                {&sourceInfo.stmt5, &sourceInfo.varZ}, {&sourceInfo.stmt7, &sourceInfo.varX}, {&sourceInfo.stmt8, &sourceInfo.varZ},
                {&sourceInfo.stmt9, &sourceInfo.varY}, {&sourceInfo.stmt10, &sourceInfo.varZ}};
    }
    if (r == PkbRelationshipType::MODIFIES && leftTypeToGet == ElementType::ASSIGNMENT && rightTypeToGet == ElementType::VARIABLE) {
        return {{&sourceInfo.assign1, &sourceInfo.varX}, {&sourceInfo.assign2, &sourceInfo.varY}, {&sourceInfo.assign3, &sourceInfo.varX},
                {&sourceInfo.assign7, &sourceInfo.varX}, {&sourceInfo.assign8, &sourceInfo.varZ}, {&sourceInfo.assign9, &sourceInfo.varY},
                {&sourceInfo.assign10, &sourceInfo.varZ}};
    }
    if (r == PkbRelationshipType::CALLS && leftTypeToGet == ElementType::PROCEDURE && rightTypeToGet == ElementType::PROCEDURE) {
        return {{&sourceInfo.procF, &sourceInfo.procG}};
    }
    if (r == PkbRelationshipType::FOLLOWS && leftTypeToGet == ElementType::PRINT && rightTypeToGet == ElementType::CALL) {
        return {{&sourceInfo.print11, &sourceInfo.call12}};
    }
    if (r == PkbRelationshipType::PARENT && leftTypeToGet == ElementType::WHILE && rightTypeToGet == ElementType::STATEMENT) {
        return {{&sourceInfo.while4, &sourceInfo.stmt5}, {&sourceInfo.while4, &sourceInfo.stmt6}};
    }
    if (r == PkbRelationshipType::AFFECTS && leftTypeToGet == ElementType::STATEMENT && rightTypeToGet == ElementType::STATEMENT) {
        return {{&sourceInfo.stmt1, &sourceInfo.stmt2}, {&sourceInfo.stmt2, &sourceInfo.stmt9}};
    }
    if (r == PkbRelationshipType::AFFECTS_T && leftTypeToGet == ElementType::STATEMENT && rightTypeToGet == ElementType::STATEMENT) {
        return {{&sourceInfo.stmt1, &sourceInfo.stmt2}, {&sourceInfo.stmt2, &sourceInfo.stmt9}};
    }
    return {};
}

bool PkbGetterStub::isRelationship(const PkbRelationshipType &r, const ProgramElement &leftSide,
                                                         const ProgramElement &rightSide) {
    if (r == PkbRelationshipType::MODIFIES && leftSide == sourceInfo.procF && rightSide == sourceInfo.varY) {
        return true;
    }
    if (r == PkbRelationshipType::MODIFIES && leftSide == sourceInfo.stmt5 && rightSide == sourceInfo.varZ) {
        return true;
    }
    if (r == PkbRelationshipType::CALLS && leftSide == sourceInfo.procF && rightSide == sourceInfo.procG) {
        return true;
    }
    if (r == PkbRelationshipType::NEXT && leftSide == sourceInfo.stmt11 && rightSide == sourceInfo.stmt12) {
        return true;
    }
    return false;
}

std::set<ProgramElement*> PkbGetterStub::getRightSide(const PkbRelationshipType &r, const ProgramElement &leftSide,
                                                                           const ElementType &typeToGet) {
    if (r == PkbRelationshipType::USES && leftSide == sourceInfo.procF && typeToGet==ElementType::VARIABLE) {
        return {&sourceInfo.varX, &sourceInfo.varZ};
    }
    if (r == PkbRelationshipType::USES && leftSide == sourceInfo.stmt2 && typeToGet ==ElementType::VARIABLE) {
        return {&sourceInfo.varX};
    }
    if (r == PkbRelationshipType::USES && leftSide == sourceInfo.stmt6 && typeToGet == ElementType::VARIABLE) {
        return {&sourceInfo.varZ};
    }
    if (r == PkbRelationshipType::CALLS && leftSide == sourceInfo.procF && typeToGet == ElementType::PROCEDURE) {
        return {&sourceInfo.procG};
    }
    if (r == PkbRelationshipType::PARENT_T && leftSide == sourceInfo.stmt4 && typeToGet == ElementType::ASSIGNMENT) {
        return {&sourceInfo.assign7, &sourceInfo.assign8, &sourceInfo.assign9, &sourceInfo.assign10};
    }
    if (r == PkbRelationshipType::NEXT_T && leftSide == sourceInfo.stmt1 && typeToGet == ElementType::STATEMENT) {
        return {&sourceInfo.stmt2, &sourceInfo.stmt3, &sourceInfo.stmt4, &sourceInfo.stmt5, &sourceInfo.stmt6,
                &sourceInfo.stmt7, &sourceInfo.stmt8, &sourceInfo.stmt9, &sourceInfo.stmt10, &sourceInfo.stmt11, &sourceInfo.stmt12};
    }
    return {};
}

std::set<ProgramElement*> PkbGetterStub::getLeftSide(const PkbRelationshipType &r, const ProgramElement &rightSide,
                                                                          const ElementType &typeToGet) {
    if (r == PkbRelationshipType::USES && rightSide == sourceInfo.varX && typeToGet == ElementType::PROCEDURE) {
        return {&sourceInfo.procF};
    }
    if (r == PkbRelationshipType::MODIFIES && rightSide == sourceInfo.varZ && typeToGet == ElementType::ASSIGNMENT) {
        return {&sourceInfo.assign8, &sourceInfo.assign10};
    }
    if (r == PkbRelationshipType::USES && rightSide == sourceInfo.varZ && typeToGet == ElementType::IF) {
        return {&sourceInfo.if6};
    }
    if (r == PkbRelationshipType::CALLS && rightSide == sourceInfo.procG && typeToGet == ElementType::PROCEDURE) {
        return {&sourceInfo.procF};
    }
    if (r == PkbRelationshipType::FOLLOWS_T && rightSide == sourceInfo.stmt12 && typeToGet == ElementType::ASSIGNMENT) {
        return {&sourceInfo.assign1, &sourceInfo.assign2, &sourceInfo.assign3};
    }
    if (r == PkbRelationshipType::NEXT && rightSide == sourceInfo.stmt2 && typeToGet == ElementType::STATEMENT) {
        return {&sourceInfo.stmt1};
    }
    return {};
}

std::set<std::pair<ProgramElement*, ProgramElement*>> PkbGetterStub::getIfWithVariable() const {
    return sourceInfo.ifVar;
}

std::set<std::pair<ProgramElement*, ProgramElement*>> PkbGetterStub::getWhileWithVariable() const {
    return sourceInfo.whileVar;
}

std::set<ProgramElement*> PkbGetterStub::getIfGivenVariable(const ProgramElement &variable) const {
    if (variable == sourceInfo.varZ) {
        return sourceInfo.ifs;
    }
    return {};
}

std::set<ProgramElement*>
PkbGetterStub::getWhileGivenVariable(const ProgramElement &variable) const {
    if (variable == sourceInfo.varX) {
        return sourceInfo.whiles;
    }
    return {};
}

std::set<std::pair<ProgramElement*, ProgramElement*>>
PkbGetterStub::getAssignmentWithVariableGivenExpression(const Expr &expr,
                                                        const ExpressionIndicator &indicator) const {
    ExpressionProcessor ep;
    if (ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("b + c"), ExpressionIndicator::FULL_MATCH) && indicator == ExpressionIndicator::PARTIAL_MATCH) {
        return patternInfo.assignVarPartial;
    }
    if (ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("b + c"), ExpressionIndicator::FULL_MATCH) && indicator==ExpressionIndicator::FULL_MATCH) {
        return patternInfo.assignVarFull;
    }
    return {};
}

std::set<ProgramElement*> PkbGetterStub::getAssignmentGivenExpression(const Expr &expr,
                                                                      const ExpressionIndicator &indicator) const {
    ExpressionProcessor ep;
    if (ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("x"), ExpressionIndicator::FULL_MATCH) && indicator == ExpressionIndicator::PARTIAL_MATCH) {
        return patternInfo.assignGivenExprPartial;
    }
    if (ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("a + b + c"), ExpressionIndicator::FULL_MATCH) && indicator==ExpressionIndicator::FULL_MATCH) {
        return patternInfo.assignGivenExprFull;
    }
    return {};
}

std::set<ProgramElement*>
PkbGetterStub::getAssignmentGivenVariableAndExpression(const ProgramElement &variable,
                                                       const Expr &expr,
                                                       const ExpressionIndicator &indicator) {
    ExpressionProcessor ep;
    if (variable == ProgramElement::createVariable("x") && ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("b + c"), ExpressionIndicator::FULL_MATCH) && indicator == ExpressionIndicator::PARTIAL_MATCH) {
        return patternInfo.assignGivenExprVarPartial;
    }
    if (variable == ProgramElement::createVariable("z") && ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("x"), ExpressionIndicator::FULL_MATCH) && indicator == ExpressionIndicator::FULL_MATCH) {
        return patternInfo.assignGivenExprVarFull;
    }
    return {};
}
