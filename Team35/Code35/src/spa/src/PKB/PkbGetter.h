#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "Entity.h"

class PkbGetter {
private:
    DB* db;

public:
    PkbGetter(DB* db);
    // TODO: change to pass by reference or pointer
    bool isRelationship(RelationshipType r, Entity leftSide, Entity rightSide);
//    std::vector<Entity> getEntity(EntityType typeToGet);
//    // TODO: does not explicitly returns statements, maybe use template?
//    std::vector<Entity> getRelationshipStatements(RelationshipType r);
//    std::vector<Entity> getLeftSide(RelationshipType r, Entity rightSide, EntityType typeToGet);
//    std::vector<Entity> getRightSide(RelationshipType r, Entity leftSide, EntityType typeToGet);
};

#endif //SPA_PKBGETTER_H
