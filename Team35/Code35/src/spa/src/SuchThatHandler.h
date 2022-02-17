#ifndef SPA_SUCHTHATHANDLER_H
#define SPA_SUCHTHATHANDLER_H


#include <vector>
#include "Entity.h"
#include "Type.h"
#include "Clause.h"
#include "Result.h"
#include "PKB/PkbGetter.h"

class SuchThatHandler {
private:
  PkbGetter* pg;

  Result handleBoolCheck(Entity entityToGet, RelationshipRef relRef);
  Result handleLeftSide(Entity entityToGet, Entity rightEntity, RelationshipType relType);
  Result handleRightSide(Entity entityToGet, Entity leftEntity, RelationshipType relType);
public:
  explicit SuchThatHandler(PkbGetter* pg);
  Result handleSuchThat(Entity entityToGet, RelationshipRef relRef);
};


#endif //SPA_SUCHTHATHANDLER_H
