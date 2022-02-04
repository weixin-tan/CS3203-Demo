#include "Entity.h"
#include "Type.h"

Entity::Entity() {
    Entity::rType = Wildcard;
}

Entity::Entity(EntityType rType, std::string name) {
    Entity::rType = rType;
    Entity::name = name;
}

std::string Entity::toString() {
    std::ostringstream buffer;
    buffer << "Type: " << Type::entityTypeStringArr[rType] << ", Name: " << name;
    return buffer.str();
}


