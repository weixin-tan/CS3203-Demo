#include "SuchThatHandler.h"

//Constructor
SuchThatHandler::SuchThatHandler(PkbGetter *pg) {
  SuchThatHandler::pg = pg;
}

//Handles Such That relationships
Result SuchThatHandler::handleSuchThat(Entity entityToGet, RelationshipRef relRef) {
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
Result SuchThatHandler::handleBoolCheck(Entity entityToGet, RelationshipRef relRef) {
  bool check;
  Entity left = relRef.leftEntity;
  Entity right = relRef.rightEntity;
  std::vector<Entity> checkEntities;

  if (relRef.leftEntity.eType == EntityType::Wildcard) { //Case where the left side is a wildcard
    Result r = handleRightSide(right, left, relRef.rType);
    checkEntities = r.getResultEntities();
    /*
    //Modifies and Uses cannot have a wildcard on the left side
    assert(("Cannot be Modifies or Uses!",
        (relRef.rType != RelationshipType::Modifies && relRef.rType != RelationshipType::Uses)));

    //Account for both cases where it is either a FixedInteger or any statement type
    if (right.eType == EntityType::FixedInteger) {
      right.eType = EntityType::Statement;
      checkEntities = pg->getLeftSide(relRef.rType, right, EntityType::Statement);
    } else {
      //Get all instances of the right entity from PKB and iterate to check if there are any statements that fit
      std::vector<Entity> iterables = pg->getEntity(right.eType);
      if (!iterables.empty()) {
        for (const Entity& e : iterables) {
          checkEntities = pg->getLeftSide(relRef.rType, e, EntityType::Statement);
          if (!checkEntities.empty()) {
            break;
          }
        }
      }
    }
    */
    check = !checkEntities.empty();

  } else if (relRef.rightEntity.eType == EntityType::Wildcard) { //Case where the right side is a wildcard
    Result r = handleLeftSide(left, right, relRef.rType);
    checkEntities = r.getResultEntities();
      /*
    //Account for both cases where it is either a FixedInteger or any statement type
    if (left.eType == EntityType::FixedInteger) {
      left.eType = EntityType::Statement;
      //Different entity types on RHS based on relationship type
      if (relRef.rType == RelationshipType::Modifies || relRef.rType == RelationshipType::Uses) {
        checkEntities = pg->getRightSide(relRef.rType, left, EntityType::Variable);
      } else {
        checkEntities = pg->getRightSide(relRef.rType, left, EntityType::Statement);
      }
    } else {
      //Get all instances of the left entity from PKB and iterate to check if there are any statements that fit
      std::vector<Entity> iterables = pg->getEntity(left.eType);
      if (!iterables.empty()) {
        for (const Entity& e : iterables) {
          checkEntities = pg->getRightSide(relRef.rType, e, EntityType::Statement);
          if (!checkEntities.empty()) {
            break;
          }
        }
      }
    }
    */
    check = !checkEntities.empty();

  } else {
    check = pg->isRelationship(relRef.rType, relRef.leftEntity, relRef.rightEntity);
  }

  // If conditions are met return all entities belonging to the type requested and if not, return empty Result
  if (check) {
    std::vector<Entity> resultEntities = pg->getEntity(entityToGet.eType);
    return Result(resultEntities);
  } else {
    return {};
  }
}

//If the entity being searched for is on the left side, find all instances that match the conditions
Result SuchThatHandler::handleLeftSide(Entity entityToGet, Entity rightEntity, RelationshipType relType) {
  std::vector<Entity> resultEntities;

  if (relType == RelationshipType::Modifies || relType == RelationshipType::Uses) {
    if (rightEntity.eType == EntityType::FixedString) {
      rightEntity.eType = EntityType::Variable;
      resultEntities = pg->getLeftSide(relType, rightEntity, entityToGet.eType);
    } else if (rightEntity.eType == EntityType::Variable || rightEntity.eType == EntityType::Wildcard) {
      std::vector<Entity> iterables = pg->getEntity(EntityType::Variable);
      for (const Entity& e : iterables) {
        std::vector<Entity> getEntities = pg->getLeftSide(relType, e, entityToGet.eType);
        resultEntities.insert(resultEntities.end(), getEntities.begin(), getEntities.end());
      }
    } else {
      assert(false);
    }
  } else if (relType == RelationshipType::Follows || relType == RelationshipType::FollowsT
      || relType == RelationshipType::Parent || relType == RelationshipType::ParentT) {
    if (rightEntity.eType == EntityType::FixedInteger) {
      rightEntity.eType = EntityType::Statement;
      resultEntities = pg->getLeftSide(relType, rightEntity, entityToGet.eType);
    } else {
      if (rightEntity.eType == EntityType::Wildcard) {
        rightEntity.eType = EntityType::Statement;
      }
      std::vector<Entity> iterables = pg->getEntity(rightEntity.eType);
      for (const Entity& e : iterables) {
          std::vector<Entity> getEntities = pg->getLeftSide(relType, e, entityToGet.eType);
          resultEntities.insert(resultEntities.end(), getEntities.begin(), getEntities.end());
      }
    }
  } else {
    assert(false);
  }
  return Result(resultEntities);
}

Result SuchThatHandler::handleRightSide(Entity entityToGet, Entity leftEntity, RelationshipType relType) {
  std::vector<Entity> resultEntities;

  if (relType == RelationshipType::Modifies || relType == RelationshipType::Uses) {
    assert(("Cannot be Wildcard for Modifies and Uses!", leftEntity.eType != EntityType::Wildcard));
  }

  if (leftEntity.eType == EntityType::FixedInteger || leftEntity.eType == EntityType::Wildcard) {
    leftEntity.eType = EntityType::Statement;
    resultEntities = pg->getRightSide(relType, leftEntity, entityToGet.eType);
  } else {
    std::vector<Entity> iterables = pg->getEntity(leftEntity.eType);
    for (const Entity &e: iterables) {
      std::vector<Entity> getEntities = pg->getRightSide(relType, e, entityToGet.eType);
      resultEntities.insert(resultEntities.end(), getEntities.begin(), getEntities.end());
    }
  }

  return Result(resultEntities);
}
