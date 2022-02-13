#ifndef SPA_PATTERNHANDLER_H
#define SPA_PATTERNHANDLER_H


#include "PkbGetter.h"
#include "Result.h"
#include "RelationshipRef.h"

class PatternHandler {
private:
  PkbGetter* pg;
public:
  explicit PatternHandler(PkbGetter* pg);
  Result handlePattern(Entity entityToGet, RelationshipRef relRef);

};


#endif //SPA_PATTERNHANDLER_H
