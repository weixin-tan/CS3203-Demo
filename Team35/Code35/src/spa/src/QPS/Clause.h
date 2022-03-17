#ifndef SPA_CLAUSE_H
#define SPA_CLAUSE_H

#include <vector>
#include "Entity.h"
#include "RelationshipRef.h"

class Clause {
public:
    Clause();

    std::vector<Entity> entityToFindList;
    std::vector<RelationshipRef> refList;

    void appendEntityToFind(const Entity& e);
    void appendRef(const RelationshipRef& r);
    std::string toString();
    bool operator==(const Clause& c1);
    bool equals(const Clause& c1);
};

#endif //SPA_CLAUSE_H
