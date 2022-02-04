#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "../Entity.h"
#include "DB.h"

class PkbGetter {
private:
  DB* db;

public:
  explicit PkbGetter(DB* db);
  bool isRelationship(const RelationshipType& r, const Entity& leftSide, const Entity& rightSide) const;
  std::vector<Entity> getEntity(const EntityType& typeToGet) const;
  // TODO: does not explicitly returns statements, maybe use template?
  std::vector<Entity> getRelationshipStatements(const RelationshipType& r) const;
  std::vector<Entity> getLeftSide(const RelationshipType& r, const Entity& rightSide, const EntityType& typeToGet) const;
  std::vector<Entity> getRightSide(const RelationshipType& r, const Entity& leftSide, const EntityType& typeToGet) const;
};

#endif //SPA_PKBGETTER_H
