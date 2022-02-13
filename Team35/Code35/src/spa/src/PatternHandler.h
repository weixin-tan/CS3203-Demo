#ifndef SPA_PATTERNHANDLER_H
#define SPA_PATTERNHANDLER_H


#include "PkbGetter.h"
#include "Result.h"
#include "RelationshipRef.h"

class PatternHandler {
private:
  PkbGetter* pg;

  Result handleDoubleWildcard(Entity entityToGet);
  Result handleLeftWildcard(Entity entityToGet, Entity rightEntity, Entity assignEntity);
  Result handleRightWildcard(Entity entityToGet, Entity leftEntity, Entity assignEntity);
  Result handleNoWildcard(Entity entityToGet, Entity rightEntity, Entity leftEntity, Entity assignEntity);
public:
  explicit PatternHandler(PkbGetter* pg);
  Result handlePattern(Entity entityToGet, RelationshipRef relRef);

};


#endif //SPA_PATTERNHANDLER_H
