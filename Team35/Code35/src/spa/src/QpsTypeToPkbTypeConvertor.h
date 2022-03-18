#ifndef SPA_QPSTYPETOPKBTYPECONVERTOR_H
#define SPA_QPSTYPETOPKBTYPECONVERTOR_H

#include "Entity.h"
#include "../PKB/PkbRelationshipType.h"
#include "../PKB/ProgramElement.h"

class QpsTypeToPkbTypeConvertor {
private:
    QpsTypeToPkbTypeConvertor();
    static const std::unordered_map<EntityType, ElementType> entityToElementMap;
    static const std::unordered_map<RelationshipType, PkbRelationshipType> qpsRelToPkbRelMap;

public:
    static ElementType convertToPkbElement(EntityType e);
    static PkbRelationshipType convertToPkbRelType(RelationshipType t);
};


#endif //SPA_QPSTYPETOPKBTYPECONVERTOR_H
