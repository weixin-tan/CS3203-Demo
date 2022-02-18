#ifndef SPA_ENTITYTOELEMENTCONVERTER_H
#define SPA_ENTITYTOELEMENTCONVERTER_H


#include "ProgramElement.h"
#include "Entity.h"

class EntityToElementConverter {
 private:
  EntityToElementConverter();

  //static bool isInteger(std::string s);

 public:
  static ElementType extractElementType(const Entity& e);
  static ProgramElement fixedEntityConverter(const Entity& e);
  static bool isInteger(std::string s);


};


#endif //SPA_ENTITYTOELEMENTCONVERTER_H
