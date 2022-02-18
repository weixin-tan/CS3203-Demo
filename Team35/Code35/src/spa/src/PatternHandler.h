#ifndef SPA_PATTERNHANDLER_H
#define SPA_PATTERNHANDLER_H


#include "PkbGetter.h"
#include "Result.h"
#include "RelationshipRef.h"

class PatternHandler {
private:
  PkbGetter* pg;

  Result handleDoubleWildcard(const Entity& entityToGet);
  Result handleLeftWildcard(const Entity& entityToGet, const Entity& rightEntity, const Entity& assignEntity);
  Result handleRightWildcard(const Entity& entityToGet, const Entity& leftEntity, const Entity& assignEntity);
  Result handleNoWildcard(const Entity& entityToGet, const Entity& rightEntity, const Entity& leftEntity, const Entity& assignEntity);
public:
  explicit PatternHandler(PkbGetter* pg);
  Result handlePattern(const Entity& entityToGet, const RelationshipRef& relRef);

};


#endif //SPA_PATTERNHANDLER_H
