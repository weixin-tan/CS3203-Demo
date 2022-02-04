//
// Created by viv on 4/2/2022.
//

#ifndef SPA_SUCHTHATHANDLER_H
#define SPA_SUCHTHATHANDLER_H


#include <vector>
#include "Entity.h"
#include "Type.h"
#include "Clause.h"

class SuchThatHandler {
 public:
  SuchThatHandler(PkbGetter* pg);

  PkbGetter* pg;

  std::vector<Result> processClause(Clause c);
  bool isRelationship(RelationshipType r, Entity e1, Entity e2);
  Result getEntity(Type typeToGet);
  Result getRelationshipStatements(RelationshipType r);
  Result getLeftSide(RelationshipType r, Entity rightSide, Type typeToGet);
  Result getRightSide(RelationshipType r, Entity rightSide, Type typeToGet);

};


#endif //SPA_SUCHTHATHANDLER_H
