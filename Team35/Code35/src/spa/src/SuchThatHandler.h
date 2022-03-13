#ifndef SPA_SUCHTHATHANDLER_H
#define SPA_SUCHTHATHANDLER_H

#include <utility>
#include <vector>
#include "Entity.h"
#include "PKB/PkbGetter.h"
#include "RelationshipRef.h"
#include "Result.h"

class SuchThatHandler {
 private:
  PkbGetter* pg;

  bool isNonSynonymEntity(EntityType e);
  bool handleNoSynonyms(PkbRelationshipType r, Entity left, Entity right);
  bool handleNoSynProcWildcard(PkbRelationshipType r, Entity left, Entity right);
  bool handleNoSynStmtWildcard(PkbRelationshipType r, Entity left, Entity right);
  bool handleNoSynVarWildcard(PkbRelationshipType r, Entity left);
  std::set<ProgramElement> handleRightSyn(PkbRelationshipType r, Entity left, Entity right);
  std::set<ProgramElement> handleLeftSyn(PkbRelationshipType r, Entity left, Entity right);
  std::set<std::pair<ProgramElement, ProgramElement>> handleTwoSyn(PkbRelationshipType r, Entity left, Entity right);

 public:
  explicit SuchThatHandler(PkbGetter* pg);
  Result handleSuchThat(const RelationshipRef& relRef);

};


#endif //SPA_SUCHTHATHANDLER_H
