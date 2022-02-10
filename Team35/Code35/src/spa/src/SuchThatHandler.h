#ifndef SPA_SUCHTHATHANDLER_H
#define SPA_SUCHTHATHANDLER_H


#include <vector>
#include "Entity.h"
#include "Type.h"
#include "Clause.h"
#include "Result.h"
#include "PKB/PkbGetter.h"

class SuchThatHandler {
 public:
  explicit SuchThatHandler(PkbGetter* pg);

  PkbGetter* pg;

  std::vector<Result> processClause(const std::vector<Clause>& clauses) const;
  bool isRelationship(RelationshipType r, const Entity& e1, const Entity& e2) const;
  std::vector<Entity> getEntity(EntityType typeToGet) const;
  std::vector<Entity> getRelationshipStatements(RelationshipType r) const;
  std::vector<Entity> getLeftSide(RelationshipType r, const Entity& rightSide, EntityType typeToGet) const;
  std::vector<Entity> getRightSide(RelationshipType r, const Entity& leftSide, EntityType typeToGet) const;
};


#endif //SPA_SUCHTHATHANDLER_H
