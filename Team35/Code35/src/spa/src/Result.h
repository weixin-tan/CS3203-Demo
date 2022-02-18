#ifndef SPA_RESULT_H
#define SPA_RESULT_H

#include <set>
#include "ProgramElement.h"
#include "RelationshipRef.h"

class Result {
 private:
  std::set<ProgramElement> programElements = {};
  RelationshipRef relRef;
  Entity entityToFind;
 public:
  Result();
  explicit Result(std::set<ProgramElement> programElements);
  std::set<ProgramElement> getProgramElements();
  void setRelationshipRef(const RelationshipRef& r);
  void setEntityToFind(const Entity& e);
};


#endif //SPA_RESULT_H
