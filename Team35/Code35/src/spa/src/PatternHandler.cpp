#include "PatternHandler.h"


PatternHandler::PatternHandler(PkbGetter* pg) {
  PatternHandler::pg = pg;
}

Result PatternHandler::handlePattern(Entity entityToGet, RelationshipRef relRef) {
  assert(("This must be a pattern relationship!\n", relRef.rType == RelationshipType::Pattern));

  Entity left = relRef.leftEntity;
  Entity right = relRef.rightEntity;
  Entity assign = relRef.AssignmentEntity;

  if (entityToGet != assign) {

  }

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

Result PatternHandler::handleDoubleWildcard(Entity entityToGet) {
  std::vector<Entity> resultEntities = pg->getEntity(EntityType::Assignment);
  return Result(resultEntities);
}

Result PatternHandler::handleLeftWildcard(Entity entityToGet, Entity rightEntity, Entity assignEntity) {
  assert(rightEntity.eType == EntityType::FixedStringWithinWildcard); //Iteration 1
  Expression e = Expression(rightEntity.name);//Missing Expression class here
  std::vector<Entity> resultEntities = pg->getAssignment(e);
  return Result(resultEntities);
}

Result PatternHandler::handleRightWildcard(Entity entityToGet, Entity leftEntity, Entity assignEntity) {
  std::vector<Entity> resultEntities;
  if (leftEntity.eType == EntityType::FixedString) {
    leftEntity.eType = EntityType::Variable;
    resultEntities = pg->getLeftSide(RelationshipType::Modifies, leftEntity, EntityType::Assignment);
    return Result(resultEntities);
  } else if (leftEntity.eType == EntityType::Variable) {
    resultEntities = pg->getEntity(EntityType::Assignment);
  } else {
    assert(false);
  }
  return Result(resultEntities);
}

Result PatternHandler::handleNoWildcard(Entity entityToGet, Entity rightEntity, Entity leftEntity, Entity assignEntity) {
  std::vector<Entity> left = handleRightWildcard(entityToGet, leftEntity, assignEntity).getResultEntities();
  std::vector<Entity> right = handleLeftWildcard(entityToGet, rightEntity, assignEntity).getResultEntities();
  //TODO
  //Combine them and return value

  return Result();
}
