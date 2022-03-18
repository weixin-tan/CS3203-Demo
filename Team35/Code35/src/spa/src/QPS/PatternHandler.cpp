#include "PatternHandler.h"

PatternHandler::PatternHandler(PkbGetter* pg) {
    PatternHandler::pg = pg;
}

// Handles pattern relationships
Result PatternHandler::handlePattern(const RelationshipRef& relRef) {
    Result result;
    std::set<ProgramElement> oneSynSet;
    std::set<std::pair<ProgramElement, ProgramElement>> twoSynSet;

    Entity left = relRef.leftEntity;
    Entity right = relRef.rightEntity;
    Entity patternType = relRef.AssignmentEntity;

    result.setResultType(ResultType::PATTERN_CLAUSE);

    if (left.eType == EntityType::WILDCARD) {
        oneSynSet = handleLeftWildcard(right, patternType);
        result.setOneSynEntity(patternType);
        result.setOneSynSet(oneSynSet);
    } else if (left.eType == EntityType::FIXED_STRING) {
        oneSynSet = handleLeftFixed(left, right, patternType);
        result.setOneSynEntity(patternType);
        result.setOneSynSet(oneSynSet);
    } else {
        twoSynSet = handleLeftVariable(left, right, patternType);
        result.setTwoSynEntities(std::pair<Entity, Entity>(patternType, left));
        result.setTwoSynSet(twoSynSet);
    }

    if (oneSynSet.empty() && twoSynSet.empty()) {
        result.setValid(false);
    } else {
        result.setValid(true);
    }

    return result;
}

// Handles cases where there is a wildcard on the left-hand side
std::set<ProgramElement> PatternHandler::handleLeftWildcard(Entity right, Entity patternType) {
    ElementType patternElem = QpsTypeToPkbTypeConvertor::convertToPkbElement(patternType.eType);
    if (patternElem == ElementType::IF || patternElem == ElementType::WHILE ||
            (patternElem == ElementType::ASSIGNMENT && right.eType == EntityType::WILDCARD)) {
        return pg->getEntity(patternElem);
    } else if (right.eType == EntityType::FIXED_STRING) {
        Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
        return pg->getAssignmentGivenExpression(rhsPattern, ExpressionIndicator::FULL_MATCH);
    } else if (right.eType == EntityType::FIXED_STRING_WITHIN_WILDCARD) {
        Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
        return pg->getAssignmentGivenExpression(rhsPattern, ExpressionIndicator::PARTIAL_MATCH);
    } else {
        return std::set<ProgramElement>{};
    }
}

// Handles cases where there is a fixed string on the left-hand side
std::set<ProgramElement> PatternHandler::handleLeftFixed(Entity left, Entity right, Entity patternType) {
    ElementType patternElem = QpsTypeToPkbTypeConvertor::convertToPkbElement(patternType.eType);
    if (right.eType == EntityType::WILDCARD) {
        if (patternElem == ElementType::IF) {
            return pg->getIfGivenVariable(ProgramElement::createVariable(left.name));
        } else if (patternElem == ElementType::WHILE) {
            return pg->getWhileGivenVariable(ProgramElement::createVariable(left.name));
        } else if (patternElem == ElementType::ASSIGNMENT) {
            return pg->getLeftSide(PkbRelationshipType::MODIFIES,
                                   ProgramElement::createVariable(left.name),
                                   ElementType::ASSIGNMENT);
        } else {
            return std::set<ProgramElement>{};
        }
    } else if (right.eType == EntityType::FIXED_STRING) {
        Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
        return pg->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable(left.name),
                                                           rhsPattern,
                                                           ExpressionIndicator::FULL_MATCH);
    } else if (right.eType == EntityType::FIXED_STRING_WITHIN_WILDCARD) {
        Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
        return pg->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable(left.name),
                                                           rhsPattern,
                                                           ExpressionIndicator::PARTIAL_MATCH);
    } else {
        return std::set<ProgramElement>{};
    }
}

// Handles cases where there is a variable synonym on the left-hand side
std::set<std::pair<ProgramElement, ProgramElement>>
PatternHandler::handleLeftVariable(Entity left, Entity right, Entity patternType) {
    ElementType patternElem = QpsTypeToPkbTypeConvertor::convertToPkbElement(patternType.eType);
    if (right.eType == EntityType::WILDCARD) {
        if (patternElem == ElementType::IF) {
            return pg->getIfWithVariable();
        } else if (patternElem == ElementType::WHILE) {
            return pg->getWhileWithVariable();
        } else if (patternElem == ElementType::ASSIGNMENT) {
            return pg->getRelationshipPairs(PkbRelationshipType::MODIFIES,
                                            ElementType::ASSIGNMENT,
                                            ElementType::VARIABLE);
        } else {
            return std::set<std::pair<ProgramElement, ProgramElement>>{};
        }
    } else if (right.eType == EntityType::FIXED_STRING) {
        Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
        return pg->getAssignmentWithVariableGivenExpression(rhsPattern, ExpressionIndicator::FULL_MATCH);
    } else if (right.eType == EntityType::FIXED_STRING_WITHIN_WILDCARD) {
        Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
        return pg->getAssignmentWithVariableGivenExpression(rhsPattern, ExpressionIndicator::PARTIAL_MATCH);
    } else {
        return std::set<std::pair<ProgramElement, ProgramElement>>{};
    }
}
