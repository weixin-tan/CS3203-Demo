#ifndef SPA_RELATIONSHIPREF_H
#define SPA_RELATIONSHIPREF_H

#include <cassert>
#include <utility>
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
  bool equals(const RelationshipRef & r1);
  bool operator==(const RelationshipRef &r1) const;
};

#endif //SPA_RELATIONSHIPREF_H
