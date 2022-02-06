#include "SuchThatHandler.h"

SuchThatHandler::SuchThatHandler(PkbGetter* pg) {
  SuchThatHandler::pg = pg;
}

std::vector<Result> SuchThatHandler::processClause(const std::vector<Clause>& clauses) const {
  std::vector<Result> results;
  for(const Clause& c : clauses) {
    std::vector<Entity> entityToFindList = c.entityToFindList;
    Entity entityToFind = entityToFindList.front(); // For basic Modifies
    std::vector<RelationshipRef> refList = c.refList;

    // Handle queries with no such that or pattern clauses
    if (refList.empty()) {
      getEntity(entityToFind.eType);
      continue;
    }

    RelationshipRef ref = refList.front(); // For basic Modifies
    RelationshipType refType = ref.rType;
    Entity left = ref.leftEntity;
    Entity right = ref.rightEntity;

    std::vector<Entity> resultEntities;

    if (left == entityToFind && right == entityToFind) {
      //Do nothing for now
    } else if (right.eType == Wildcard && left == entityToFind) {
      // Handle wildcard for modifies
      // Only handles for all Modifies statement for now
      resultEntities = getRelationshipStatements(refType);
    } else if (left == entityToFind) {
      resultEntities = getLeftSide(refType, right, entityToFind.eType);
    } else if (right == entityToFind) {
      resultEntities = getRightSide(refType, left, entityToFind.eType);
    } else {
      bool check = isRelationship(refType, left, right);
      if (check) {
        resultEntities = getEntity(entityToFind.eType);
      }
    }
    auto* result = new Result(resultEntities);
    results.push_back(*result);
  }
  return results;
}

bool SuchThatHandler::isRelationship(RelationshipType r, const Entity& e1, const Entity& e2) const {
  bool isRel = pg->isRelationship(r, e1, e2);
  return isRel;
}

std::vector<Entity> SuchThatHandler::getEntity(EntityType typeToGet) const {
  std::vector<Entity> resultEntities = pg->getEntity(typeToGet);
  return resultEntities;
}

std::vector<Entity> SuchThatHandler::getRelationshipStatements(RelationshipType r) const {
  std::vector<Entity> resultEntities = pg->getRelationshipStatements(r);
  return resultEntities;
}

std::vector<Entity> SuchThatHandler::getLeftSide(RelationshipType r, const Entity& rightSide, EntityType typeToGet) const {
  std::vector<Entity> resultEntities = pg->getLeftSide(r, rightSide, typeToGet);
  return resultEntities;
}

std::vector<Entity> SuchThatHandler::getRightSide(RelationshipType r, const Entity& leftSide, EntityType typeToGet) const {
  std::vector<Entity> resultEntities = pg->getRightSide(r, leftSide, typeToGet);
  return resultEntities;
}


