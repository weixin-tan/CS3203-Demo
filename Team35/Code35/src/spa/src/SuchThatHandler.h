#ifndef SPA_SUCHTHATHANDLER_H
#define SPA_SUCHTHATHANDLER_H


#include <vector>
#include "Entity.h"
#include "PKB/PkbGetter.h"
#include "RelationshipRef.h"
#include "Result.h"

class SuchThatHandler {
private:
  PkbGetter* pg;

  Result handleBoolCheck(const Entity& entityToGet, const RelationshipRef& relRef);
  Result handleLeftSide(const Entity& entityToGet, const Entity& rightEntity, RelationshipType relType);
  Result handleRightSide(const Entity& entityToGet, const Entity& leftEntity, RelationshipType relType);
public:
  explicit SuchThatHandler(PkbGetter* pg);
  Result handleSuchThat(const Entity& entityToGet, const RelationshipRef& relRef);
};


#endif //SPA_SUCHTHATHANDLER_H
