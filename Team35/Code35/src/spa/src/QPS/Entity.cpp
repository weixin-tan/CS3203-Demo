#include "Entity.h"

Entity::Entity() {
    Entity::eType = EntityType::NULL_ENTITY;
    Entity::aType = EntityAttributeType::NULL_ATTRIBUTE;
}

Entity::Entity(EntityType eType, std::string name) {
    Entity::eType = eType;
    Entity::name = std::move(name);
    Entity::aType = EntityAttributeType::NULL_ATTRIBUTE;
}

Entity::Entity(EntityType eType, std::string name, EntityAttributeType aType) {
    Entity::eType = eType;
    Entity::name = std::move(name);
    Entity::aType = aType;
}

std::string Entity::toString() {
    std::ostringstream buffer;
    buffer << "Type: " << Type::entityTypeToString(eType) << ", Name: " << name;

    if (aType != EntityAttributeType::NULL_ATTRIBUTE) {
        buffer << ", Attribute: " << Type::attributeTypeToString(aType);
    }
    buffer << "\n";
    return buffer.str();
}

bool Entity::operator==(const Entity& e1) const {
    return eType == e1.eType && name == e1.name;
}

bool Entity::operator!=(const Entity& e1) const {
    return !(eType == e1.eType && name == e1.name);
}
bool Entity::equals(const Entity& e1) {
    return eType == e1.eType && name == e1.name && aType == e1.aType;
}

