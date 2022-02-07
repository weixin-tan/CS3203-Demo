#include "Entity.h"
#include "Type.h"

Entity::Entity() {
    Entity::eType = EntityType::Wildcard;
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
  if(eType == e1.eType && name == e1.name) {
    return true;
  } else {
    return false;
  }
}

