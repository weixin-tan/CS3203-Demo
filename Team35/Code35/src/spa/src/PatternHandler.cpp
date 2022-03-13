#include "PatternHandler.h"
#include "QpsTypeToPkbTypeConvertor.h"
#include "ExpressionProcessor.h"

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

  result.setResultType(ResultType::PatternClause);

  if (left.eType == EntityType::Wildcard) {
    oneSynSet = handleLeftWildcard(right, patternType);
    result.setOneSynEntity(patternType);
    result.setOneSynSet(oneSynSet);
  } else if (left.eType == EntityType::FixedString) {
    oneSynSet = handleLeftFixed(left, right, patternType);
    result.setOneSynEntity(patternType);
    result.setOneSynSet(oneSynSet);
  } else {
    twoSynSet = handleLeftVariable(left, right, patternType);
    result.setTwoSynEntities(std::pair<Entity, Entity> (patternType, left));
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
  if (patternElem == ElementType::kIf || patternElem == ElementType::kWhile ||
    (patternElem == ElementType::kAssignment && right.eType == EntityType::Wildcard)) {
    return pg->getEntity(patternElem);
  } else if (right.eType == EntityType::FixedString) {
    Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
    return pg->getAssignmentGivenExpression(rhsPattern, ExpressionIndicator::FULL_MATCH);
  } else if (right.eType == EntityType::FixedStringWithinWildcard) {
    Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
    return pg->getAssignmentGivenExpression(rhsPattern, ExpressionIndicator::PARTIAL_MATCH);
  }
}

// Handles cases where there is a fixed string on the left-hand side
std::set<ProgramElement> PatternHandler::handleLeftFixed(Entity left, Entity right, Entity patternType) {
  ElementType patternElem = QpsTypeToPkbTypeConvertor::convertToPkbElement(patternType.eType);
  if (right.eType == EntityType::Wildcard) {
    if (patternElem == ElementType::kIf) {
      return pg->getIfGivenVariable(ProgramElement::createVariable(left.name));
    } else if (patternElem == ElementType::kWhile) {
      return pg->getWhileGivenVariable(ProgramElement::createVariable(left.name));
    } else if (patternElem == ElementType::kAssignment){
      return pg->getLeftSide(PkbRelationshipType::kModifies, ProgramElement::createVariable(left.name), ElementType::kAssignment);
    }
  } else if (right.eType == EntityType::FixedString) {
    Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
    return pg->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable(left.name), rhsPattern, ExpressionIndicator::FULL_MATCH);
  } else if (right.eType == EntityType::FixedStringWithinWildcard) {
    Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
    return pg->getAssignmentGivenVariableAndExpression(ProgramElement::createVariable(left.name), rhsPattern, ExpressionIndicator::PARTIAL_MATCH);
  }
}

// Handles cases where there is a variable synonym on the left-hand side
std::set<std::pair<ProgramElement, ProgramElement>>
PatternHandler::handleLeftVariable(Entity left, Entity right, Entity patternType) {
  ElementType patternElem = QpsTypeToPkbTypeConvertor::convertToPkbElement(patternType.eType);
  if (right.eType == EntityType::Wildcard) {
    if (patternElem == ElementType::kIf) {
      return pg->getIfWithVariable();
    } else if (patternElem == ElementType::kWhile) {
      return pg->getWhileWithVariable();
    } else if (patternElem == ElementType::kAssignment) {
      return pg->getRelationshipPairs(PkbRelationshipType::kModifies, ElementType::kAssignment, ElementType::kVariable);
    }
  } else if (right.eType == EntityType::FixedString) {
    Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
    return pg->getAssignmentWithVariableGivenExpression(rhsPattern, ExpressionIndicator::FULL_MATCH);
  } else if (right.eType == EntityType::FixedStringWithinWildcard) {
    Expr rhsPattern = ExpressionProcessor::stringToExpr(right.name);
    return pg->getAssignmentWithVariableGivenExpression(rhsPattern, ExpressionIndicator::PARTIAL_MATCH);
  }
}
