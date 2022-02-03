#include "Entity.h"
#include "Type.h"

Entity::Entity() {
    Entity::cType = Wildcard;
}

Entity::Entity(entityType cType, std::string name) {
    Entity::cType = cType;
    Entity::name = name;
}

std::string Entity::toString() {
    std::ostringstream buffer;
    buffer << "Type: " << Type::entityTypeStringArr[cType] << ", Name: " << name;
    return buffer.str();
}


