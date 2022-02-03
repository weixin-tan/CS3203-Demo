#include <string>
#include <sstream>
#include "Type.h"

#ifndef SPA_ENTITY_H
#define SPA_ENTITY_H


class Entity {
public:
    Entity();
    Entity(entityType cType, std::string name);

    entityType cType;
    std::string name;

    std::string toString();
};


#endif //SPA_ENTITY_H