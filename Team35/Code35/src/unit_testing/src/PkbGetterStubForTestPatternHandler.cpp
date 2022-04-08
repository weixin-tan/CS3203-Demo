#include "PkbGetterStubForTestPatternHandler.h"

/*
PkbGetterStubForTestPatternHandler::PkbGetterStubForTestPatternHandler(DB *db) : PkbGetter(db) {}

std::set<std::pair<ProgramElement, ProgramElement>>
PkbGetterStubForTestPatternHandler::getRelationshipPairs(const PkbRelationshipType &r, const ElementType &leftTypeToGet,
                                                         const ElementType &rightTypeToGet) {
    std::set<std::pair<ProgramElement, ProgramElement>> results;
    if (r == PkbRelationshipType::MODIFIES && leftTypeToGet == ElementType::ASSIGNMENT && rightTypeToGet == ElementType::VARIABLE) {
        results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ProgramElement::createVariable("x")));
        results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::ASSIGNMENT, 2), ProgramElement::createVariable("y")));
        results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::ASSIGNMENT, 4), ProgramElement::createVariable("z")));
        results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::ASSIGNMENT, 5), ProgramElement::createVariable("z")));
        results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::ASSIGNMENT, 7), ProgramElement::createVariable("z")));
        results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::ASSIGNMENT, 8), ProgramElement::createVariable("a")));
    }
    return results;
}

std::set<ProgramElement> PkbGetterStubForTestPatternHandler::getEntity(const ElementType &typeToGet) const {
    std::set<ProgramElement> results;
    if (typeToGet == ElementType::ASSIGNMENT) {
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 1));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 2));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 4));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 5));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 7));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 8));
    }
    if (typeToGet == ElementType::WHILE) {
        results.insert(ProgramElement::createStatement(ElementType::WHILE, 6));
    }
    if (typeToGet == ElementType::IF) {
        results.insert(ProgramElement::createStatement(ElementType::IF, 3));
    }
    return results;
}

std::set<ProgramElement>
PkbGetterStubForTestPatternHandler::getLeftSide(const PkbRelationshipType &r, const ProgramElement &rightSide,
                                                const ElementType &typeToGet) {
    std::set<ProgramElement> results;
    if (r == PkbRelationshipType::MODIFIES && rightSide == ProgramElement::createVariable("z") && typeToGet == ElementType::ASSIGNMENT) {
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 4));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 5));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 7));
    }
    return results;
}

std::set<std::pair<ProgramElement, ProgramElement>> PkbGetterStubForTestPatternHandler::getIfWithVariable() const {
    std::set<std::pair<ProgramElement, ProgramElement>> results;
    results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::IF, 3), ProgramElement::createVariable("y")));
    return results;
}

std::set<std::pair<ProgramElement, ProgramElement>> PkbGetterStubForTestPatternHandler::getWhileWithVariable() const {
    std::set<std::pair<ProgramElement, ProgramElement>> results;
    results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::WHILE, 6), ProgramElement::createVariable("z")));
    results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::WHILE, 6), ProgramElement::createVariable("x")));
    return results;
}

std::set<ProgramElement> PkbGetterStubForTestPatternHandler::getIfGivenVariable(const ProgramElement &variable) const {
    std::set<ProgramElement> results;
    if (variable == ProgramElement::createVariable("y")) {
        results.insert(ProgramElement::createStatement(ElementType::IF, 3));
    }
    return results;
}

std::set<ProgramElement>
PkbGetterStubForTestPatternHandler::getWhileGivenVariable(const ProgramElement &variable) const {
    std::set<ProgramElement> results;
    if (variable == ProgramElement::createVariable("x")) {
        results.insert(ProgramElement::createStatement(ElementType::WHILE, 6));
    }
    return results;
}

std::set<std::pair<ProgramElement, ProgramElement>>
PkbGetterStubForTestPatternHandler::getAssignmentWithVariableGivenExpression(const Expr expr,
                                                                             const ExpressionIndicator indicator) const {
    std::set<std::pair<ProgramElement, ProgramElement>> results;
    ExpressionProcessor ep;
    if (ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("b + c"), ExpressionIndicator::FULL_MATCH) && indicator == ExpressionIndicator::PARTIAL_MATCH) {
        results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::ASSIGNMENT, 1), ProgramElement::createVariable("x")));
        results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::ASSIGNMENT, 8), ProgramElement::createVariable("a")));
    }
    if (ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("b + c"), ExpressionIndicator::FULL_MATCH) && indicator==ExpressionIndicator::FULL_MATCH) {
        results.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::ASSIGNMENT, 8), ProgramElement::createVariable("a")));
    }
    return results;
}

std::set<ProgramElement> PkbGetterStubForTestPatternHandler::getAssignmentGivenExpression(const Expr expr, const ExpressionIndicator indicator) const {
    std::set<ProgramElement> results;
    ExpressionProcessor ep;
    if (ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("x"), ExpressionIndicator::FULL_MATCH) && indicator == ExpressionIndicator::PARTIAL_MATCH) {
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 2));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 4));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 5));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 7));
    }
    if (ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("a + b + c"), ExpressionIndicator::FULL_MATCH) && indicator==ExpressionIndicator::FULL_MATCH) {
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 1));
    }
    return results;
}

std::set<ProgramElement>
PkbGetterStubForTestPatternHandler::getAssignmentGivenVariableAndExpression(const ProgramElement &variable,
                                                                            const Expr expr,
                                                                            const ExpressionIndicator indicator) {
    std::set<ProgramElement> results;
    ExpressionProcessor ep;
    if (variable == ProgramElement::createVariable("x") && ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("b + c"), ExpressionIndicator::FULL_MATCH)) {
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 1));
    }
    if (variable == ProgramElement::createVariable("z") && ep.fullfillsMatching(expr, ExpressionProcessor::stringToExpr("x"), ExpressionIndicator::FULL_MATCH) && indicator == ExpressionIndicator::FULL_MATCH) {
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 4));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 5));
    }
    return results;
}
*/
