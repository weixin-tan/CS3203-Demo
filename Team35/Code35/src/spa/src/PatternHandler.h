#ifndef SPA_PATTERNHANDLER_H
#define SPA_PATTERNHANDLER_H


#include "PkbGetter.h"
#include "Result.h"
#include "RelationshipRef.h"

class PatternHandler {
private:
  PkbGetter* pg;

  std::set<ProgramElement> handleLeftWildcard(Entity right, Entity patternType);
  std::set<ProgramElement> handleLeftFixed(Entity left, Entity right, Entity patternType);
  std::set<std::pair<ProgramElement, ProgramElement>> handleLeftVariable(Entity left, Entity right, Entity patternType);
public:
  explicit PatternHandler(PkbGetter* pg);
  Result handlePattern(const RelationshipRef& relRef);

};


#endif //SPA_PATTERNHANDLER_H
