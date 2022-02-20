#include "RelationshipRef.h"

RelationshipRef::RelationshipRef() {
  RelationshipRef::rType = RelationshipType::Null;
}

RelationshipRef::RelationshipRef(RelationshipType rType, Entity leftEntity, Entity rightEntity) {
  RelationshipRef::rType = rType;
  RelationshipRef::leftEntity = leftEntity;
  RelationshipRef::rightEntity = rightEntity;
}

RelationshipRef::RelationshipRef(RelationshipType rType, Entity leftEntity, Entity rightEntity, Entity AssignmentEntity){
  assert(rType == RelationshipType::Pattern);
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
bool RelationshipRef::operator==(const RelationshipRef &r1) {
  return rType == r1.rType && leftEntity == r1.leftEntity && rightEntity == r1.rightEntity && AssignmentEntity == r1.AssignmentEntity;
}
