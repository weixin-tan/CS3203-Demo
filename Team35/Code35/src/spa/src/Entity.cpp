#include "Entity.h"
#include "Type.h"

Entity::Entity() {
    Entity::eType = Wildcard;
}

Entity::Entity(EntityType eType, std::string name) {
    Entity::eType = eType;
    Entity::name = name;
}

std::string Entity::toString() {
    std::ostringstream buffer;
    buffer << "Type: " << Type::entityTypeStringArr[eType] << ", Name: " << name;
    return buffer.str();
}


