#ifndef SPA_SUCHTHATHANDLER_H
#define SPA_SUCHTHATHANDLER_H

#include <utility>
#include <vector>
#include "Entity.h"
#include "EntityToElementConverter.h"
#include "PKB/PkbGetter.h"
#include "RelationshipRef.h"
#include "Result.h"

class SuchThatHandler {
private:
  PkbGetter* pg;
  const bool LEFT = true;
  const bool RIGHT = false;
  Result handleBoolCheck(const Entity& entityToGet, const RelationshipRef& relRef);
  Result handleLeftSide(const Entity& entityToGet, const Entity& rightEntity, PkbRelationshipType relType);
  Result handleRightSide(const Entity& entityToGet, const Entity& leftEntity, PkbRelationshipType relType);
  std::set<std::pair<ProgramElement, ProgramElement>> getFixedEntityPairs(PkbRelationshipType relType, const Entity& givenEntity, ElementType t, bool direction);
  std::set<std::pair<ProgramElement, ProgramElement>> getAllCombinationsOfPairs(PkbRelationshipType relType, const Entity& givenEntity, ElementType t, bool direction);
  PkbRelationshipType convertRel(RelationshipType r);
  Result handleCalls(PkbRelationshipType pkbRel, const Entity& leftEntity, const Entity& rightEntity);
public:
  explicit SuchThatHandler(PkbGetter* pg);
  Result handleSuchThat(const Entity& entityToGet, const RelationshipRef& relRef);
};


#endif //SPA_SUCHTHATHANDLER_H
