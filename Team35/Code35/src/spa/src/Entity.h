#ifndef SPA_ENTITY_H
#define SPA_ENTITY_H

#include <string>
#include <sstream>
#include "Type.h"

class Entity {
public:
    Entity();
    Entity(EntityType eType, std::string name);

    EntityType eType;
    std::string name;

    std::string toString();
    bool operator==(const Entity & e1) const;
};


#endif //SPA_ENTITY_H
