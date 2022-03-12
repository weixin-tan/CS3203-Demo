#ifndef SPA_QPSTYPETOPKBTYPECONVERTOR_H
#define SPA_QPSTYPETOPKBTYPECONVERTOR_H


#include "ProgramElement.h"
#include "Entity.h"
#include "PkbRelationshipType.h"

class QpsTypeToPkbTypeConvertor {
 private:
  QpsTypeToPkbTypeConvertor();
  static const std::unordered_map<EntityType, ElementType> entityToElementMap;
  static const std::unordered_map<RelationshipType, PkbRelationshipType> qpsRelToPkbRelMap;

 public:
  static ElementType convertToPkbElement(EntityType e);
  static PkbRelationshipType convertToPkbRelType(RelationshipType t);
  static ProgramElement fixedEntityConverter(const Entity& e);
  static bool isInteger(std::string s);
  static bool isValid(std::string s);
};


#endif //SPA_QPSTYPETOPKBTYPECONVERTOR_H
