#include "SuchThatHandler.h"
#include "EntityToElementConverter.h"

//Constructor
SuchThatHandler::SuchThatHandler(PkbGetter *pg) {
  SuchThatHandler::pg = pg;
}

//Handles Such That relationships
Result SuchThatHandler::handleSuchThat(const Entity& entityToGet, const RelationshipRef& relRef) {
  RelationshipType relType = relRef.rType;
  Entity left = relRef.leftEntity;
  Entity right = relRef.rightEntity;

  if (left != entityToGet && right != entityToGet) { //If both sides are not the entity being searched for
    return handleBoolCheck(entityToGet, relRef);
  } else if (left == entityToGet) { //If left side is the entity being searched for
    return handleLeftSide(entityToGet, right, relType);
  } else if (right == entityToGet) { //If right side is the entity being searched for
    return handleRightSide(entityToGet, left, relType);
  } else {
    return {};
  }
}

//Get all instances of the searched entity based on whether the relationship exists in PKB
Result SuchThatHandler::handleBoolCheck(const Entity& entityToGet, const RelationshipRef& relRef) {
  bool check;
  Entity left = relRef.leftEntity;
  Entity right = relRef.rightEntity;
  std::set<ProgramElement> checkElements;
  ElementType elementTypeToGet = EntityToElementConverter::extractElementType(entityToGet);

  if (left.eType == EntityType::Wildcard) { //Case where the left side is a wildcard
    Result r = handleRightSide(right, left, relRef.rType);
    checkElements = r.getProgramElements();
    check = !checkElements.empty();
  } else if (right.eType == EntityType::Wildcard) { //Case where the right side is a wildcard
    Result r = handleLeftSide(left, right, relRef.rType);
    checkElements = r.getProgramElements();
    check = !checkElements.empty();
  } else if (left.eType == EntityType::FixedInteger && (right.eType == EntityType::FixedString || right.eType == EntityType::FixedInteger)) {
    ProgramElement leftElement = EntityToElementConverter::fixedEntityConverter(left);
    ProgramElement rightElement = EntityToElementConverter::fixedEntityConverter(right);
    check = pg->isRelationship(relRef.rType, leftElement, rightElement);
  } else {
    Result r = handleLeftSide(left, right, relRef.rType);
    checkElements = r.getProgramElements();
    check = !checkElements.empty();
  }

  // If conditions are met return all entities belonging to the type requested and if not, return empty Result
  if (check) {
      std::set<ProgramElement> resultElements = pg->getEntity(elementTypeToGet);
    return Result(resultElements);
  } else {
    return {};
  }
}

//If the entity being searched for is on the left side, find all instances that match the conditions
Result SuchThatHandler::handleLeftSide(const Entity& entityToGet, const Entity& rightEntity, RelationshipType relType) {
  std::set<ProgramElement> resultElements;
  ElementType elementTypeToGet = EntityToElementConverter::extractElementType(entityToGet);

  if (relType == RelationshipType::Modifies || relType == RelationshipType::Uses) {
    if (rightEntity.eType == EntityType::FixedString) {
      ProgramElement rightProgramElement = EntityToElementConverter::fixedEntityConverter(rightEntity);
      resultElements = pg->getLeftSide(relType, rightProgramElement, elementTypeToGet);
    } else if (rightEntity.eType == EntityType::Variable || rightEntity.eType == EntityType::Wildcard) {
      std::set<ProgramElement> iterables = pg->getEntity(ElementType::kVariable);
      for (const ProgramElement& e : iterables) {
        std::set<ProgramElement> getElements = pg->getLeftSide(relType, e, elementTypeToGet);
        resultElements.insert(getElements.begin(), getElements.end());
      }
    } else {
      assert(false);
    }
  } else if (relType == RelationshipType::Follows || relType == RelationshipType::FollowsT
      || relType == RelationshipType::Parent || relType == RelationshipType::ParentT) {
    if (rightEntity.eType == EntityType::FixedInteger) {
      ProgramElement rightProgramElement = EntityToElementConverter::fixedEntityConverter(rightEntity);
      resultElements = pg->getLeftSide(relType, rightProgramElement, elementTypeToGet);
    } else {
      ElementType elementToIterate;
      if (rightEntity.eType == EntityType::Wildcard) {
        elementToIterate = ElementType::kStatement;
      } else {
        elementToIterate = EntityToElementConverter::extractElementType(rightEntity);
      }
      std::set<ProgramElement> iterables = pg->getEntity(elementToIterate);
      for (const ProgramElement& e : iterables) {
        std::set<ProgramElement> getElements = pg->getLeftSide(relType, e, elementTypeToGet);
        resultElements.insert(getElements.begin(), getElements.end());
      }
    }
  } else {
    assert(false);
  }
  return Result(resultElements);
}

Result SuchThatHandler::handleRightSide(const Entity& entityToGet, const Entity& leftEntity, RelationshipType relType) {
  std::set<ProgramElement> resultElements;
  ElementType elementTypeToGet = EntityToElementConverter::extractElementType(entityToGet);

  if (relType == RelationshipType::Modifies || relType == RelationshipType::Uses) {
    assert(("Cannot be Wildcard for Modifies and Uses!", leftEntity.eType != EntityType::Wildcard));
  }

  if (leftEntity.eType == EntityType::FixedInteger) {
    ProgramElement leftProgramElement = EntityToElementConverter::fixedEntityConverter(leftEntity);
    resultElements = pg->getRightSide(relType, leftProgramElement, elementTypeToGet);
  } else {
    ElementType elementToIterate;
    if (leftEntity.eType == EntityType::Wildcard) {
      elementToIterate = ElementType::kStatement;
    } else {
      elementToIterate = EntityToElementConverter::extractElementType(leftEntity);
    }
    std::set<ProgramElement> iterables = pg->getEntity(elementToIterate);
    for (const ProgramElement &e: iterables) {
      std::set<ProgramElement> getElements = pg->getRightSide(relType, e, elementTypeToGet);
      resultElements.insert(getElements.begin(), getElements.end());
    }
  }
  return Result(resultElements);
}
