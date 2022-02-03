#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "Entity.h"

// TODO: update once enum naming is fixed
class PkbGetter {
    virtual bool isRelationship(relationshipType r, Entity leftSide, Entity rightSide) = 0;
    virtual std::vector<Entity> getEntity(entityType typeToGet) = 0;
    // TODO: does not explicitly returns statements
    virtual std::vector<Entity> getRelationshipStatements(relationshipType r) = 0;
    virtual std::vector<Entity> getLeftSide(relationshipType r, Entity rightSide, entityType typeToGet) = 0;
    virtual std::vector<Entity> getRightSide(relationshipType r, Entity leftSide, entityType typeToGet) = 0;
};

#endif //SPA_PKBGETTER_H
