#ifndef SPA_RELATIONSHIPREF_H
#define SPA_RELATIONSHIPREF_H

#include "Type.h"
#include "Entity.h"

class RelationshipRef{
public:
    relationshipType rType;
    Entity leftEntity;
    Entity rightEntity;
    RelationshipRef(relationshipType rType, Entity leftEntity, Entity rightEntity);
    std::string toString();
};


#endif //SPA_RELATIONSHIPREF_H
