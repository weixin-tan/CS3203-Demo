#ifndef SPA_CLAUSE_H
#define SPA_CLAUSE_H

#include <vector>
#include "Entity.h"
#include "RelationshipRef.h"

class Clause{
public:
  Clause();

  std::vector<Entity> entityToFindList;
  std::vector<RelationshipRef> refList;

  void appendEntityToFind(const Entity& e);
  void appendRef(const RelationshipRef& r);
  std::string toString();
};


#endif //SPA_CLAUSE_H
