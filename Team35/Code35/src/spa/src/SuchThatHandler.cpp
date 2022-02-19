#include "SuchThatHandler.h"
#include "EntityToElementConverter.h"

//Constructor
SuchThatHandler::SuchThatHandler(PkbGetter *pg) {
  SuchThatHandler::pg = pg;
}

//Handles Such That relationships
Result SuchThatHandler::handleSuchThat(const Entity& entityToGet, const RelationshipRef& relRef) {
  Result result;
  RelationshipType relType = relRef.rType;
  Entity left = relRef.leftEntity;
  Entity right = relRef.rightEntity;

  if (left == entityToGet && right == entityToGet) {
      //Iteration 1 has no cases where both sides are the same result and so empty Result is appropriate
  } else if (left == entityToGet) { //If left side is the entity being searched for
    result = handleLeftSide(entityToGet, right, relType);
  } else if (right == entityToGet) { //If right side is the entity being searched for
    result = handleRightSide(entityToGet, left, relType);
  } else {
    result = handleBoolCheck(entityToGet, relRef); //If both sides are not the entity being searched for
  }

  result.setLeftSuchThatEntity(left);
  result.setRightSuchThatEntity(right);
  result.setResultEntity(entityToGet);
  return result;
}

//Get all instances of the searched entity based on whether the relationship exists in PKB
Result SuchThatHandler::handleBoolCheck(const Entity& entityToGet, const RelationshipRef& relRef) {
  Result result;
  bool check;
  Entity left = relRef.leftEntity;
  Entity right = relRef.rightEntity;
  std::set<std::pair<ProgramElement, ProgramElement>> checkElements;
  ElementType elementTypeToGet = EntityToElementConverter::extractElementType(entityToGet);

  if (left.eType == EntityType::Wildcard) { //Case where the left side is a wildcard
    Result r = handleRightSide(right, left, relRef.rType);
    checkElements = r.getSuchThatElements();
    check = !checkElements.empty();
  } else if (right.eType == EntityType::Wildcard) { //Case where the right side is a wildcard
    Result r = handleLeftSide(left, right, relRef.rType);
    checkElements = r.getSuchThatElements();
    check = !checkElements.empty();
  } else if (left.eType == EntityType::FixedInteger && (right.eType == EntityType::FixedString || right.eType == EntityType::FixedInteger)) {
    ProgramElement leftElement = EntityToElementConverter::fixedEntityConverter(left);
    ProgramElement rightElement = EntityToElementConverter::fixedEntityConverter(right);
    check = pg->isRelationship(relRef.rType, leftElement, rightElement);
  } else {
    Result r = handleLeftSide(left, right, relRef.rType);
    checkElements = r.getSuchThatElements();
    check = !checkElements.empty();
  }

  // If conditions are met return all entities belonging to the type requested and if not, return empty Result
  if (check) {
    std::set<ProgramElement> resultElements = pg->getEntity(elementTypeToGet);
    result.setSuchThatElements(checkElements);
    result.setNoClauseElements(resultElements);
    return result;
  } else {
    return {};
  }
}

//If the entity being searched for is on the left side, find all instances that match the conditions
Result SuchThatHandler::handleLeftSide(const Entity& entityToGet, const Entity& rightEntity, RelationshipType relType) {
  Result result;
  std::set<std::pair<ProgramElement, ProgramElement>> resultPairs;
  ElementType elementTypeToGet = EntityToElementConverter::extractElementType(entityToGet);

  if (relType == RelationshipType::Modifies || relType == RelationshipType::Uses) {
    if (rightEntity.eType == EntityType::FixedString) {
      resultPairs = getFixedEntityPairs(relType, rightEntity, elementTypeToGet, LEFT);
    } else if (rightEntity.eType == EntityType::Variable || rightEntity.eType == EntityType::Wildcard) {
      resultPairs = getAllCombinationsOfPairs(relType, rightEntity, elementTypeToGet, LEFT);
    } else {
      assert(false);
    }
  } else if (relType == RelationshipType::Follows || relType == RelationshipType::FollowsT
      || relType == RelationshipType::Parent || relType == RelationshipType::ParentT) {
    if (rightEntity.eType == EntityType::FixedInteger) {
      resultPairs = getFixedEntityPairs(relType, rightEntity, elementTypeToGet, LEFT);
    } else {
      resultPairs = getAllCombinationsOfPairs(relType, rightEntity, elementTypeToGet, LEFT);
    }
  } else {
    assert(false);
  }
  result.setSuchThatElements(resultPairs);
  return result;
}

Result SuchThatHandler::handleRightSide(const Entity& entityToGet, const Entity& leftEntity, RelationshipType relType) {
  Result result;
  std::set<std::pair<ProgramElement, ProgramElement>> resultPairs;
  ElementType elementTypeToGet = EntityToElementConverter::extractElementType(entityToGet);

  if (relType == RelationshipType::Modifies || relType == RelationshipType::Uses) {
    assert(("Cannot be Wildcard for Modifies and Uses!", leftEntity.eType != EntityType::Wildcard));
  }

  if (leftEntity.eType == EntityType::FixedInteger) {
    resultPairs = getFixedEntityPairs(relType, leftEntity, elementTypeToGet, RIGHT);
  } else {
    resultPairs = getAllCombinationsOfPairs(relType, leftEntity, elementTypeToGet, RIGHT);
  }

  result.setSuchThatElements(resultPairs);
  return result;
}

std::set<std::pair<ProgramElement, ProgramElement>>
SuchThatHandler::getFixedEntityPairs(RelationshipType relType, const Entity& givenEntity, ElementType t, bool direction) {
  std::set<std::pair<ProgramElement, ProgramElement>> finalResult;
  std::set<ProgramElement> pkbResult;
  ProgramElement programElement = EntityToElementConverter::fixedEntityConverter(givenEntity);

  if (direction == LEFT) {
    pkbResult = pg->getLeftSide(relType, programElement, t);
    for (const auto& e : pkbResult) {
      std::pair <ProgramElement, ProgramElement> combination (e, programElement);
      finalResult.insert(combination);
    }
  } else {
    pkbResult = pg->getRightSide(relType, programElement, t);
    for (const auto& e : pkbResult) {
      std::pair <ProgramElement, ProgramElement> combination (programElement, e);
      finalResult.insert(combination);
    }
  }
  return finalResult;
}

std::set<std::pair<ProgramElement, ProgramElement>>
SuchThatHandler::getAllCombinationsOfPairs(RelationshipType relType, const Entity& givenEntity, ElementType t, bool direction) {
  std::set<std::pair<ProgramElement, ProgramElement>> finalResult;
  std::set<ProgramElement> pkbResult;
  ElementType elementToIterate;

  if (direction == LEFT && (relType == RelationshipType::Modifies || relType == RelationshipType::Uses)) {
    elementToIterate = ElementType::kVariable;
  } else {
    if (givenEntity.eType == EntityType::Wildcard) {
      elementToIterate = ElementType::kStatement;
    } else {
      elementToIterate = EntityToElementConverter::extractElementType(givenEntity);
    }
  }

  std::set<ProgramElement> iterables = pg->getEntity(elementToIterate);
  for (const ProgramElement& e : iterables) {
    if (direction == LEFT) {
      pkbResult = pg->getLeftSide(relType, e, t);
      for (const auto& r : pkbResult) {
        std::pair <ProgramElement, ProgramElement> combination (r, e);
        finalResult.insert(combination);
      }
    } else {
      pkbResult = pg->getRightSide(relType, e, t);
      for (const auto& r : pkbResult) {
        std::pair <ProgramElement, ProgramElement> combination (e, r);
        finalResult.insert(combination);
      }
    }
  }
  return finalResult;
}