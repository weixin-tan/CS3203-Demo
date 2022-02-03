#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "Entity.h"

class PkbGetter {
    virtual bool isRelationship(RelationshipType r, Entity leftSide, Entity rightSide) = 0;
    virtual std::vector<Entity> getEntity(EntityType typeToGet) = 0;
    // TODO: does not explicitly returns statements, maybe use template?
    virtual std::vector<Entity> getRelationshipStatements(RelationshipType r) = 0;
    virtual std::vector<Entity> getLeftSide(RelationshipType r, Entity rightSide, EntityType typeToGet) = 0;
    virtual std::vector<Entity> getRightSide(RelationshipType r, Entity leftSide, EntityType typeToGet) = 0;
};

#endif //SPA_PKBGETTER_H
