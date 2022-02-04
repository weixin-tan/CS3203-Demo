#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "Entity.h"
#include "DB.h"

class PkbGetter {
private:
  DB* db;

public:
  explicit PkbGetter(DB* db);
  // TODO: change to pass by reference or pointer
  bool isRelationship(const RelationshipType& r, const Entity& leftSide, const Entity& rightSide) const;
//  std::vector<Entity> getEntity(const EntityType& typeToGet) const;
  // TODO: does not explicitly returns statements, maybe use template?
//  std::vector<Entity> getRelationshipStatements(RelationshipType r);
//  std::vector<Entity> getLeftSide(RelationshipType r, Entity rightSide, EntityType typeToGet);
//  std::vector<Entity> getRightSide(RelationshipType r, Entity leftSide, EntityType typeToGet);
};

#endif //SPA_PKBGETTER_H
