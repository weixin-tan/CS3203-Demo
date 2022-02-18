#include "Entity.h"
#include "Type.h"

Entity::Entity() {
    Entity::eType = EntityType::Null;
}

Entity::Entity(EntityType eType, std::string name) {
    Entity::eType = eType;
    Entity::name = name;
}

std::string Entity::toString() {
    std::ostringstream buffer;
    buffer << "Type: " << Type::entityTypeToString(eType) << ", Name: " << name << "\n";
    return buffer.str();
}

bool Entity::operator==(const Entity &e1) const {
  return eType == e1.eType && name == e1.name;
}

bool Entity::operator!=(const Entity &e1) const {
  return eType != e1.eType && name != e1.name;
}
