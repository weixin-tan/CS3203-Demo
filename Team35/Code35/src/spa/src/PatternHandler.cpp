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
  
  if (left.eType == EntityType::Wildcard && right.eType == EntityType::Wildcard) {
    return handleDoubleWildcard(entityToGet);
  } else if (left.eType == EntityType::Wildcard) {
    return handleLeftWildcard(entityToGet, right, assign);
  } else if (right.eType == EntityType::Wildcard) {
    return handleRightWildcard(entityToGet, left, assign);
  } else {
    return handleNoWildcard(entityToGet, right, left, assign);
  }

}

Result PatternHandler::handleDoubleWildcard(const Entity& entityToGet) {
  std::set<ProgramElement> resultElements = pg->getEntity(ElementType::kAssignment);
  return Result(resultElements);
}

Result PatternHandler::handleLeftWildcard(const Entity& entityToGet, const Entity& rightEntity, const Entity& assignEntity) {
  assert(rightEntity.eType == EntityType::FixedStringWithinWildcard); //Iteration 1
  ProgramElement rightElement = EntityToElementConverter::fixedEntityConverter(rightEntity);
    std::set<ProgramElement> resultElements = pg->getLeftSide(RelationshipType::Uses, rightElement, ElementType::kAssignment);
  return Result(resultElements);
}

Result PatternHandler::handleRightWildcard(const Entity& entityToGet, const Entity& leftEntity, const Entity& assignEntity) {
  std::set<ProgramElement> resultElements;
  if (leftEntity.eType == EntityType::FixedString) {
    ProgramElement leftElement = EntityToElementConverter::fixedEntityConverter(leftEntity);
    resultElements = pg->getLeftSide(RelationshipType::Modifies, leftElement, ElementType::kAssignment);
  } else if (leftEntity.eType == EntityType::Variable) {
    resultElements = pg->getEntity(ElementType::kAssignment);
  } else {
    assert(false);
  }
  return Result(resultElements);
}

Result PatternHandler::handleNoWildcard(const Entity& entityToGet, const Entity& rightEntity, const Entity& leftEntity, const Entity& assignEntity) {
  std::set<ProgramElement> left = handleRightWildcard(entityToGet, leftEntity, assignEntity).getProgramElements();
  std::set<ProgramElement> right = handleLeftWildcard(entityToGet, rightEntity, assignEntity).getProgramElements();
  std::set<ProgramElement> resultElements;

  for (const auto& e : left) {
    if (right.count(e)) {
      resultElements.insert(e);
    }
  }

  return Result(resultElements);
}
