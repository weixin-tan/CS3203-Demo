#ifndef SPA_ENTITY_H
#define SPA_ENTITY_H

#include <string>
#include <sstream>
#include "Type.h"

class Entity {
public:
  Entity();
  Entity(EntityType eType, std::string name);
  Entity(EntityType eType, std::string name, EntityAttributeType aType);

  EntityType eType;
  EntityAttributeType aType;
  std::string name;

  std::string toString();
  bool operator==(const Entity & e1) const;
  bool operator!=(const Entity & e1) const;
};


#endif //SPA_ENTITY_H
