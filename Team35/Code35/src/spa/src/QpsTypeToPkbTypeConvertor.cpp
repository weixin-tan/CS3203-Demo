#include <cassert>
#include <regex>
#include "QpsTypeToPkbTypeConvertor.h"
#include "QueryRegex.h"

const std::unordered_map<RelationshipType, PkbRelationshipType> QpsTypeToPkbTypeConvertor::qpsRelToPkbRelMap = {
    {RelationshipType::Modifies, PkbRelationshipType::MODIFIES},
    {RelationshipType::Uses, PkbRelationshipType::USES},
    {RelationshipType::Follows, PkbRelationshipType::FOLLOWS},
    {RelationshipType::FollowsT, PkbRelationshipType::FOLLOWS_T},
    {RelationshipType::Parent, PkbRelationshipType::PARENT},
    {RelationshipType::ParentT, PkbRelationshipType::PARENT_T},
    {RelationshipType::Calls, PkbRelationshipType::CALLS},
    {RelationshipType::CallsT, PkbRelationshipType::CALLS_T},
    {RelationshipType::Next, PkbRelationshipType::NEXT},
    {RelationshipType::NextT, PkbRelationshipType::NEXT_T},
    {RelationshipType::Affects, PkbRelationshipType::AFFECTS},
    {RelationshipType::AffectsT, PkbRelationshipType::AFFECTS_T}
};

const std::unordered_map<EntityType, ElementType> QpsTypeToPkbTypeConvertor::entityToElementMap = {
    {EntityType::Statement, ElementType::STATEMENT},
    {EntityType::Assignment, ElementType::ASSIGNMENT},
    {EntityType::Variable, ElementType::VARIABLE},
    {EntityType::Constant, ElementType::CONSTANT},
    {EntityType::If, ElementType::IF},
    {EntityType::While, ElementType::WHILE},
    {EntityType::Procedure, ElementType::PROCEDURE},
    {EntityType::Read, ElementType::READ},
    {EntityType::Print, ElementType::PRINT},
    {EntityType::Call, ElementType::CALL}
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
