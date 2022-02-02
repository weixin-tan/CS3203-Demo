#include "RelationshipRef.h"

RelationshipRef::RelationshipRef(relationshipType rType, Entity leftEntity, Entity rightEntity) {
    RelationshipRef::rType = rType;
    RelationshipRef::leftEntity = leftEntity;
    RelationshipRef::rightEntity = rightEntity;
}

std::string RelationshipRef::toString() {
    std::ostringstream buffer;
    buffer << "Relationship Type: " << Type::relationshipTypeStringArr[rType] << " leftEntity: "
        << leftEntity.toString() << " rightEntity " << rightEntity.toString() << std::endl;

    return buffer.str();
};
