#include <vector>
#include "Entity.h"
#include "RelationshipRef.h"

#ifndef SPA_CLAUSE_H
#define SPA_CLAUSE_H

class Clause{

public:
    Clause();

    std::vector<Entity> entityToFindList;
    std::vector<RelationshipRef> refList;

    void appendEntityToFind(Entity e);
    void appendRef(RelationshipRef r);
    std::string toString();



};


#endif //SPA_CLAUSE_H
