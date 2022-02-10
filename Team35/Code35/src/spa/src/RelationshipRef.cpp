#include "RelationshipRef.h"

RelationshipRef::RelationshipRef(RelationshipType rType, Entity leftEntity, Entity rightEntity) {
  RelationshipRef::rType = rType;
  RelationshipRef::leftEntity = leftEntity;
  RelationshipRef::rightEntity = rightEntity;
}

RelationshipRef::RelationshipRef(RelationshipType rType, Entity leftEntity, Entity rightEntity, Entity AssignmentEntity){
  assert(rType == RelationshipType::Pattern);
  assert(AssignmentEntity.eType == EntityType::Assignment);
  RelationshipRef::rType = rType;
  RelationshipRef::leftEntity = leftEntity;
  RelationshipRef::rightEntity = rightEntity;
  RelationshipRef::AssignmentEntity = AssignmentEntity;
}

std::string RelationshipRef::toString() {
  std::ostringstream buffer;
  buffer << "Relationship Type: " << Type::relationshipTypeToString(rType) << " leftEntity: "
         << leftEntity.toString() << " rightEntity " << rightEntity.toString() << std::endl;

  return buffer.str();
}