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

void Entity::clear_aType() {
    this->aType = EntityAttributeType::NULL_ATTRIBUTE;
}

std::string Entity::toString() const {
    std::ostringstream buffer;
    buffer << "Type: " << Type::entityTypeToString(eType) << ", Name: " << name;

    if (aType != EntityAttributeType::NULL_ATTRIBUTE) {
        buffer << ", Attribute: " << Type::attributeTypeToString(aType);
    }
    buffer << "\n";
    return buffer.str();
}

size_t Entity::getEntityHash() const {
    return std::hash<std::string>{}(name) ^ (static_cast<std::size_t>(eType) * static_cast<std::size_t>(aType));
}

bool Entity::operator==(const Entity& e1) const {
    return eType == e1.eType && name == e1.name;
}

bool Entity::operator!=(const Entity& e1) const {
    return !(eType == e1.eType && name == e1.name);
}
bool Entity::equals(const Entity& e1) const {
    return eType == e1.eType && name == e1.name && aType == e1.aType;
}
bool Entity::operator<(const Entity& e1) const {
    return name < e1.name;
}

// for use in mapping in TableRow. 
size_t EntityHashFunction::operator()(const Entity& e) const {
    return std::hash<std::string>{}(e.name) ^ (static_cast<std::size_t>(e.eType) * static_cast<std::size_t>(e.aType));
}