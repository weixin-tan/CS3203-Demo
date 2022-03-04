#include "RelationshipRef.h"

#include <utility>

RelationshipRef::RelationshipRef() {
  RelationshipRef::rType = RelationshipType::Null;
}

RelationshipRef::RelationshipRef(RelationshipType rType, Entity leftEntity, Entity rightEntity) {
  RelationshipRef::rType = rType;
  RelationshipRef::leftEntity = std::move(leftEntity);
  RelationshipRef::rightEntity = std::move(rightEntity);
}

RelationshipRef::RelationshipRef(RelationshipType rType, Entity leftEntity, Entity rightEntity, Entity AssignmentEntity){
  if (rType == RelationshipType::Pattern){
    RelationshipRef::rType = rType;
  }else{
    RelationshipRef::rType = RelationshipType::Null;
  }
  RelationshipRef::leftEntity = std::move(leftEntity);
  RelationshipRef::rightEntity = std::move(rightEntity);
  RelationshipRef::AssignmentEntity = std::move(AssignmentEntity);
}

std::string RelationshipRef::toString() {
  std::ostringstream buffer;
  buffer << "Relationship Type: " << Type::relationshipTypeToString(rType) << " leftEntity: "
         << leftEntity.toString() << " rightEntity " << rightEntity.toString();
  if (rType == RelationshipType::Pattern){
    buffer << " assignEntity: " << AssignmentEntity.toString();
  }
  buffer << std::endl;
  return buffer.str();
}
bool RelationshipRef::operator==(const RelationshipRef &r1) {
  return rType == r1.rType && leftEntity == r1.leftEntity && rightEntity == r1.rightEntity && AssignmentEntity == r1.AssignmentEntity;
}
