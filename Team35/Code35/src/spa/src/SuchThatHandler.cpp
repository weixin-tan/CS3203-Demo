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
  PkbRelationshipType pkbRel = convertRel(relType);
  Entity left = relRef.leftEntity;
  Entity right = relRef.rightEntity;

  if (pkbRel == PkbRelationshipType::CALLS || pkbRel == PkbRelationshipType::CALLS_T) {
    result = handleCalls(pkbRel, left, right);
  } else if (left == entityToGet && right == entityToGet) {
      //Iteration 1 has no cases where both sides are the same result and so empty Result is appropriate
  } else if (left == entityToGet) { //If left side is the entity being searched for
    result = handleLeftSide(entityToGet, right, pkbRel);
  } else if (right == entityToGet) { //If right side is the entity being searched for
    result = handleRightSide(entityToGet, left, pkbRel);
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
    PkbRelationshipType pkbRel = convertRel(relRef.rType);

  if (left.eType == EntityType::Wildcard && right.eType == EntityType::Wildcard) {
    assert(relRef.rType != RelationshipType::Modifies || relRef.rType != RelationshipType::Uses);
    std::set<ProgramElement> elements = pg->getEntity(ElementType::STATEMENT);
    check = false;
    for (const ProgramElement& e1 : elements) {
      for (const ProgramElement& e2 : elements) {
        bool isRel = pg->isRelationship(pkbRel, e1, e2);
        if (isRel) {
          check = true;
          checkElements.insert(std::pair<ProgramElement, ProgramElement> (e1, e2));
          break;
        }
      }
    }
  } else if (left.eType == EntityType::Wildcard) { //Case where the left side is a wildcard
    assert(relRef.rType != RelationshipType::Modifies || relRef.rType != RelationshipType::Uses);
    left.eType = EntityType::Statement;
    Result r = handleLeftSide(left, right, pkbRel);
    checkElements = r.getSuchThatElements();
    check = !checkElements.empty();
  } else if (right.eType == EntityType::Wildcard) { //Case where the right side is a wildcard
    if (relRef.rType == RelationshipType::Modifies || relRef.rType == RelationshipType::Uses) {
      right.eType = EntityType::Variable;
    } else {
      right.eType = EntityType::Statement;
    }
    Result r = handleRightSide(right, left, pkbRel);
    checkElements = r.getSuchThatElements();
    check = !checkElements.empty();
  } else if (left.eType == EntityType::FixedInteger && (right.eType == EntityType::FixedString || right.eType == EntityType::FixedInteger)) {
    ProgramElement leftElement = EntityToElementConverter::fixedEntityConverter(left);
    ProgramElement rightElement = EntityToElementConverter::fixedEntityConverter(right);
    check = pg->isRelationship(pkbRel, leftElement, rightElement);
    std::pair<ProgramElement, ProgramElement> fixedPair(leftElement, rightElement);
    checkElements.insert(fixedPair);
  } else if (left.eType == EntityType::FixedInteger){
    Result r = handleRightSide(right, left, pkbRel);
    checkElements = r.getSuchThatElements();
    check = !checkElements.empty();
  } else if (right.eType == EntityType::FixedInteger || right.eType == EntityType::FixedString) {
    Result r = handleLeftSide(left, right, pkbRel);
    checkElements = r.getSuchThatElements();
    check = !checkElements.empty();
  } else { // For all other cases handleLeftSide or handleRightSide would work
    Result r = handleLeftSide(left, right, pkbRel);
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
Result SuchThatHandler::handleLeftSide(const Entity& entityToGet, const Entity& rightEntity, PkbRelationshipType relType) {
  Result result;
  std::set<std::pair<ProgramElement, ProgramElement>> resultPairs;
  ElementType elementTypeToGet = EntityToElementConverter::extractElementType(entityToGet);

  if (relType == PkbRelationshipType::MODIFIES || relType == PkbRelationshipType::USES) {
    if (rightEntity.eType == EntityType::FixedString) {
      resultPairs = getFixedEntityPairs(relType, rightEntity, elementTypeToGet, LEFT);
    } else if (rightEntity.eType == EntityType::Variable || rightEntity.eType == EntityType::Wildcard) {
      resultPairs = getAllCombinationsOfPairs(relType, rightEntity, elementTypeToGet, LEFT);
    } else {
      assert(false);
    }
  } else if (relType == PkbRelationshipType::FOLLOWS || relType == PkbRelationshipType::FOLLOWS_T
      || relType == PkbRelationshipType::PARENT || relType == PkbRelationshipType::PARENT_T) {
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

Result SuchThatHandler::handleRightSide(const Entity& entityToGet, const Entity& leftEntity, PkbRelationshipType relType) {
  Result result;
  std::set<std::pair<ProgramElement, ProgramElement>> resultPairs;
  ElementType elementTypeToGet = EntityToElementConverter::extractElementType(entityToGet);

  if (relType == PkbRelationshipType::MODIFIES || relType == PkbRelationshipType::USES) {
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
SuchThatHandler::getFixedEntityPairs(PkbRelationshipType relType, const Entity& givenEntity, ElementType t, bool direction) {
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
SuchThatHandler::getAllCombinationsOfPairs(PkbRelationshipType relType, const Entity& givenEntity, ElementType t, bool direction) {
  std::set<std::pair<ProgramElement, ProgramElement>> finalResult;
  std::set<ProgramElement> pkbResult;
  ElementType elementToIterate;

  if (direction == LEFT && (relType == PkbRelationshipType::MODIFIES || relType == PkbRelationshipType::USES)) {
    elementToIterate = ElementType::VARIABLE;
  } else {
    if (givenEntity.eType == EntityType::Wildcard) {
      elementToIterate = ElementType::STATEMENT;
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

PkbRelationshipType SuchThatHandler::convertRel(RelationshipType r) {
  switch(r){
    case RelationshipType::Modifies: return PkbRelationshipType::MODIFIES;
    case RelationshipType::Uses: return PkbRelationshipType::USES;
    case RelationshipType::Parent: return PkbRelationshipType::PARENT;
    case RelationshipType::ParentT: return PkbRelationshipType::PARENT_T;
    case RelationshipType::Follows: return PkbRelationshipType::FOLLOWS;
    case RelationshipType::FollowsT: return PkbRelationshipType::FOLLOWS_T;
    case RelationshipType::Calls: return PkbRelationshipType::CALLS;
    case RelationshipType::CallsT: return PkbRelationshipType::CALLS_T;
  }
}

Result SuchThatHandler::handleCalls(PkbRelationshipType pkbRel, const Entity &leftEntity, const Entity &rightEntity) {
  std::set<std::pair<ProgramElement, ProgramElement>> resultElements;
  if (leftEntity.eType == EntityType::FixedString && rightEntity.eType == EntityType::FixedString) {
    ProgramElement left = ProgramElement::createProcedure(leftEntity.name);
    ProgramElement right = ProgramElement::createProcedure(rightEntity.name);
    bool check = pg->isRelationship(pkbRel, left, right);
    if (check) {
      std::pair<ProgramElement, ProgramElement> pair(left, right);
      resultElements.insert(pair);
    }
  } else if (leftEntity.eType == EntityType::FixedString) {
    ProgramElement left = ProgramElement::createProcedure(leftEntity.name);
    std::set<ProgramElement> rightElements = pg->getRightSide(pkbRel, left, ElementType::PROCEDURE);
    for (const auto& e: rightElements) {
      std::pair<ProgramElement, ProgramElement> pair(left, e);
      resultElements.insert(pair);
    }
  } else if (rightEntity.eType == EntityType::FixedString) {
    ProgramElement right = ProgramElement::createProcedure(rightEntity.name);
    std::set<ProgramElement> leftElements = pg->getLeftSide(pkbRel, right, ElementType::PROCEDURE);
    for (const auto& e: leftElements) {
      std::pair<ProgramElement, ProgramElement> pair(e, right);
      resultElements.insert(pair);
    }
  } else {
    std::set<ProgramElement> allCalls = pg->getEntity(ElementType::PROCEDURE);
    for (const auto& c1 : allCalls) {
      for (const auto& c2 : allCalls) {
        bool isRel = pg->isRelationship(pkbRel, c1, c2);
        if (isRel) {
          std::pair<ProgramElement, ProgramElement> pair(c1, c2);
          resultElements.insert(pair);
        }
      }
    }
  }
  Result result;
  result.setSuchThatElements(resultElements);
  result.setNoClauseElements(pg->getEntity(ElementType::PROCEDURE));
  return result;
}
