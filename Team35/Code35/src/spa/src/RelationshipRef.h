#ifndef SPA_RELATIONSHIPREF_H
#define SPA_RELATIONSHIPREF_H

#include <cassert>
#include "Type.h"
#include "Entity.h"

class RelationshipRef{
public:
  RelationshipType rType;
  Entity leftEntity;
  Entity rightEntity;
  Entity AssignmentEntity;
  RelationshipRef();
  RelationshipRef(RelationshipType rType, Entity leftEntity, Entity rightEntity);
  RelationshipRef(RelationshipType rType, Entity leftEntity, Entity rightEntity, Entity AssignmentEntity);
  std::string toString();
  bool operator==(const RelationshipRef &r1);
};

#endif //SPA_RELATIONSHIPREF_H
