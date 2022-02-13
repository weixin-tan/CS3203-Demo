#ifndef SPA_QPSHANDLER_H
#define SPA_QPSHANDLER_H


#include "PkbGetter.h"
#include "Result.h"
#include "Clause.h"
#include "SuchThatHandler.h"
#include "PatternHandler.h"

class QPSHandler {
private:
  PkbGetter* pg;
  SuchThatHandler* suchThatHandler;
  PatternHandler* patternHandler;
public:
  explicit QPSHandler(PkbGetter* pg);
  std::vector<Result> processClause(const std::vector<Clause>& clauses) const;
  Result handleNoRelationshipRef(Entity entityToFind) const;
};


#endif //SPA_QPSHANDLER_H
