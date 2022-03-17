#include "RelationshipRef.h"

RelationshipRef::RelationshipRef() {
    RelationshipRef::rType = RelationshipType::NULL_RELATIONSHIP;
}

RelationshipRef::RelationshipRef(RelationshipType rType, Entity leftEntity, Entity rightEntity) {
    RelationshipRef::rType = rType;
    RelationshipRef::leftEntity = std::move(leftEntity);
    RelationshipRef::rightEntity = std::move(rightEntity);
}

RelationshipRef::RelationshipRef(RelationshipType rType,
                                 Entity leftEntity,
                                 Entity rightEntity,
                                 Entity AssignmentEntity) {
    if (rType == RelationshipType::PATTERN) {
        RelationshipRef::rType = rType;
    } else {
        RelationshipRef::rType = RelationshipType::NULL_RELATIONSHIP;
    }
    RelationshipRef::leftEntity = std::move(leftEntity);
    RelationshipRef::rightEntity = std::move(rightEntity);
    RelationshipRef::AssignmentEntity = std::move(AssignmentEntity);
}

std::string RelationshipRef::toString() {
    std::ostringstream buffer;
    buffer << "Relationship Type: " << Type::relationshipTypeToString(rType) << " leftEntity: "
           << leftEntity.toString() << " rightEntity " << rightEntity.toString();
    if (rType == RelationshipType::PATTERN) {
        buffer << " assignEntity: " << AssignmentEntity.toString();
    }
    buffer << std::endl;
    return buffer.str();
}
bool RelationshipRef::operator==(const RelationshipRef& r1) const {
    return rType == r1.rType && leftEntity == r1.leftEntity && rightEntity == r1.rightEntity
            && AssignmentEntity == r1.AssignmentEntity;
}
bool RelationshipRef::equals(const RelationshipRef& r1) {
    return rType == r1.rType && leftEntity.equals(r1.leftEntity) && rightEntity.equals(rightEntity)
            && AssignmentEntity.equals(r1.AssignmentEntity);
}
