#include "PatternHandler.h"
#include "EntityToElementConverter.h"

PatternHandler::PatternHandler(PkbGetter* pg) {
  PatternHandler::pg = pg;
}

Result PatternHandler::handlePattern(const Entity& entityToGet, const RelationshipRef& relRef) {
  assert(("This must be a pattern relationship!\n", relRef.rType == RelationshipType::Pattern));

  Entity left = relRef.leftEntity;
  Entity right = relRef.rightEntity;
  Entity assign = relRef.AssignmentEntity;
  ElementType pkbElement = EntityToElementConverter::extractElementType(entityToGet);
  Result result;

  if (left.eType == EntityType::Wildcard && right.eType == EntityType::Wildcard) {
    result = handleDoubleWildcard();
  } else if (left.eType == EntityType::Wildcard) {
    result = handleLeftWildcard(right);
  } else if (right.eType == EntityType::Wildcard) {
    result = handleRightWildcard(left);
  } else {
    result = handleNoWildcard(right, left);
  }

  result.setNoClauseElements(pg->getEntity(pkbElement));
  result.setAssignEntity(assign);
  result.setResultEntity(entityToGet);
  return result;
}

Result PatternHandler::handleDoubleWildcard() {
  Result result;
  std::set<ProgramElement> resultElements = pg->getEntity(ElementType::ASSIGNMENT);
  result.setPatternElements(resultElements);
  return result;
}

Result PatternHandler::handleLeftWildcard(const Entity& rightEntity) {
  assert(rightEntity.eType == EntityType::FixedStringWithinWildcard); //Iteration 1
  Result result;
  ProgramElement rightElement = EntityToElementConverter::fixedEntityConverter(rightEntity);
  std::set<ProgramElement> resultElements = pg->getLeftSide(PkbRelationshipType::USES, rightElement, ElementType::ASSIGNMENT);
  result.setPatternElements(resultElements);
  return result;
}

Result PatternHandler::handleRightWildcard(const Entity& leftEntity) {
  Result result;
  std::set<ProgramElement> resultElements;
  std::set<std::pair<ProgramElement,ProgramElement>> entRefResultElements;
  if (leftEntity.eType == EntityType::FixedString) {
    ProgramElement leftElement = EntityToElementConverter::fixedEntityConverter(leftEntity);
    resultElements = pg->getLeftSide(PkbRelationshipType::MODIFIES, leftElement, ElementType::ASSIGNMENT);
  } else if (leftEntity.eType == EntityType::Variable) {
    result.setAssignEntRef(leftEntity);
    std::set<ProgramElement> entRefElements = pg->getEntity(ElementType::VARIABLE);
    for (const auto& e : entRefElements) {
      std::set<ProgramElement> assign = pg->getLeftSide(PkbRelationshipType::MODIFIES, e, ElementType::ASSIGNMENT);
      for (const auto& a : assign) {
        std::pair <ProgramElement, ProgramElement> combination (a, e);
        entRefResultElements.insert(combination);
      }
    }
  } else {
    assert(false);
  }
  result.setPatternElements(resultElements);
  result.setEntRefElements(entRefResultElements);
  return result;
}

Result PatternHandler::handleNoWildcard(const Entity& rightEntity, const Entity& leftEntity) {
  Result result;
  Result leftResult = handleRightWildcard(leftEntity);
  Result rightResult = handleLeftWildcard(rightEntity);
  std::set<ProgramElement> resultElements;
  if (leftResult.getAssignEntRef().eType == EntityType::Null) {
    for (const auto& e : leftResult.getPatternElements()) {
      if (rightResult.getPatternElements().count(e)) {
        resultElements.insert(e);
      }
    }
    result.setPatternElements(resultElements);
  } else {
    result.setAssignEntRef(leftResult.getAssignEntRef());
    std::set<std::pair<ProgramElement, ProgramElement>> pairResult;
    for (const auto& p: leftResult.getEntRefElements()) {
      for (const auto& e: rightResult.getPatternElements()) {
        if (p.first == e) {
          pairResult.insert(p);
        }
      }
    }
    result.setEntRefElements(pairResult);
  }
  return result;
}
