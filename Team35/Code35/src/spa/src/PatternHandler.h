#ifndef SPA_PATTERNHANDLER_H
#define SPA_PATTERNHANDLER_H

#include "PkbGetter.h"
#include "Result.h"
#include "RelationshipRef.h"
#include "EntityToElementConverter.h"

class PatternHandler {
private:
  PkbGetter* pg;

  Result handleDoubleWildcard();
  Result handleLeftWildcard(const Entity& rightEntity);
  Result handleRightWildcard(const Entity& leftEntity);
  Result handleNoWildcard(const Entity& rightEntity, const Entity& leftEntity);
public:
  explicit PatternHandler(PkbGetter* pg);
  Result handlePattern(const Entity& entityToGet, const RelationshipRef& relRef);

};


#endif //SPA_PATTERNHANDLER_H
