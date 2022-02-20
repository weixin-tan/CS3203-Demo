#ifndef SPA_ENTITYTOELEMENTCONVERTER_H
#define SPA_ENTITYTOELEMENTCONVERTER_H


#include "ProgramElement.h"
#include "Entity.h"

class EntityToElementConverter {
 private:
  EntityToElementConverter();

 public:
  static ElementType extractElementType(const Entity& e);
  static ProgramElement fixedEntityConverter(const Entity& e);
  static bool isInteger(std::string s);
  static bool isValid(std::string s);
};


#endif //SPA_ENTITYTOELEMENTCONVERTER_H
