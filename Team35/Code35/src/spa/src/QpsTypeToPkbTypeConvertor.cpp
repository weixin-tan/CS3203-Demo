#include <cassert>
#include <regex>
#include "QpsTypeToPkbTypeConvertor.h"
#include "QPS/QueryRegex.h"

const std::unordered_map<RelationshipType, PkbRelationshipType> QpsTypeToPkbTypeConvertor::qpsRelToPkbRelMap = {
    {RelationshipType::MODIFIES, PkbRelationshipType::MODIFIES},
    {RelationshipType::USES, PkbRelationshipType::USES},
    {RelationshipType::FOLLOWS, PkbRelationshipType::FOLLOWS},
    {RelationshipType::FOLLOWS_T, PkbRelationshipType::FOLLOWS_T},
    {RelationshipType::PARENT, PkbRelationshipType::PARENT},
    {RelationshipType::PARENT_T, PkbRelationshipType::PARENT_T},
    {RelationshipType::CALLS, PkbRelationshipType::CALLS},
    {RelationshipType::CALLS_T, PkbRelationshipType::CALLS_T},
    {RelationshipType::NEXT, PkbRelationshipType::NEXT},
    {RelationshipType::NEXT_T, PkbRelationshipType::NEXT_T},
    {RelationshipType::AFFECTS, PkbRelationshipType::AFFECTS},
    {RelationshipType::AFFECTS_T, PkbRelationshipType::AFFECTS_T}
};

const std::unordered_map<EntityType, ElementType> QpsTypeToPkbTypeConvertor::entityToElementMap = {
    {EntityType::STATEMENT, ElementType::STATEMENT},
    {EntityType::ASSIGNMENT, ElementType::ASSIGNMENT},
    {EntityType::VARIABLE, ElementType::VARIABLE},
    {EntityType::CONSTANT, ElementType::CONSTANT},
    {EntityType::IF, ElementType::IF},
    {EntityType::WHILE, ElementType::WHILE},
    {EntityType::PROCEDURE, ElementType::PROCEDURE},
    {EntityType::READ, ElementType::READ},
    {EntityType::PRINT, ElementType::PRINT},
    {EntityType::CALL, ElementType::CALL}
};

QpsTypeToPkbTypeConvertor::QpsTypeToPkbTypeConvertor() = default;

PkbRelationshipType QpsTypeToPkbTypeConvertor::convertToPkbRelType(RelationshipType t) {
    auto iter = QpsTypeToPkbTypeConvertor::qpsRelToPkbRelMap.find(t);
    return iter->second;
}

ElementType QpsTypeToPkbTypeConvertor::convertToPkbElement(EntityType e) {
    auto iter = QpsTypeToPkbTypeConvertor::entityToElementMap.find(e);
    return iter->second;
}
