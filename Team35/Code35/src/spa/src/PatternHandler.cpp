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

  result.setNoClauseElements(pg->getEntity(ElementType::kAssignment));
  result.setAssignEntity(assign);
  result.setResultEntity(entityToGet);
  return result;
}

Result PatternHandler::handleDoubleWildcard() {
  Result result;
  std::set<ProgramElement> resultElements = pg->getEntity(ElementType::kAssignment);
  result.setPatternElements(resultElements);
  return result;
}

Result PatternHandler::handleLeftWildcard(const Entity& rightEntity) {
  assert(rightEntity.eType == EntityType::FixedStringWithinWildcard); //Iteration 1
  Result result;
  ProgramElement rightElement = EntityToElementConverter::fixedEntityConverter(rightEntity);
  std::set<ProgramElement> resultElements = pg->getLeftSide(PkbRelationshipType::kUses, rightElement, ElementType::kAssignment);
  result.setPatternElements(resultElements);
  return result;
}

Result PatternHandler::handleRightWildcard(const Entity& leftEntity) {
  Result result;
  std::set<ProgramElement> resultElements;
  if (leftEntity.eType == EntityType::FixedString) {
    ProgramElement leftElement = EntityToElementConverter::fixedEntityConverter(leftEntity);
    resultElements = pg->getLeftSide(PkbRelationshipType::kModifies, leftElement, ElementType::kAssignment);
  } else if (leftEntity.eType == EntityType::Variable) {
    resultElements = pg->getEntity(ElementType::kAssignment);
  } else {
    assert(false);
  }
  result.setPatternElements(resultElements);
  return result;
}

Result PatternHandler::handleNoWildcard(const Entity& rightEntity, const Entity& leftEntity) {
  Result result;
  std::set<ProgramElement> left = handleRightWildcard(leftEntity).getPatternElements();
  std::set<ProgramElement> right = handleLeftWildcard(rightEntity).getPatternElements();
  std::set<ProgramElement> resultElements;

  for (const auto& e : left) {
    if (right.count(e)) {
      resultElements.insert(e);
    }
  }

  result.setPatternElements(resultElements);
  return result;
}
