#ifndef SPA_ENTITY_H
#define SPA_ENTITY_H

#include <string>
#include <sstream>
#include <utility>
#include "Type.h"

class Entity {
public:
    Entity();
    Entity(EntityType eType, std::string name);
    Entity(EntityType eType, std::string name, EntityAttributeType aType);

    EntityType eType;
    EntityAttributeType aType;
    std::string name;

    std::string toString() const;
    bool equals(const Entity& e1) const;

    size_t getEntityHash() const;

    void clear_aType();
    bool operator==(const Entity& e1) const;
    bool operator!=(const Entity& e1) const;
    bool operator<(const Entity& e1) const;
};

class EntityHashFunction {
public:

    // Use sum of lengths of first and last names
    // as hash function.
    size_t operator()(const Entity& e) const;

};

#endif //SPA_ENTITY_H
