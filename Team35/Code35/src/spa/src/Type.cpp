#include <unordered_map>
#include "Type.h"

const std::unordered_map<ResultType, std::string> Type::resultTypeToStringMap = {
  {ResultType::NO_CLAUSE,        "NoClause"},
  {ResultType::SUCH_THAT_CLAUSE, "SuchThatClause"},
  {ResultType::PATTERN_CLAUSE,   "PatternClause"},
  {ResultType::WITH_CLAUSE,      "WithClause"},
  {ResultType::NULL_CLAUSE, "NullClause"}
};


const std::unordered_map<EntityType, std::string> Type::entityTypeToStringMap = {
    {EntityType::STATEMENT, "STATEMENT"},
    {EntityType::ASSIGNMENT, "ASSIGNMENT"},
    {EntityType::VARIABLE, "VARIABLE"},
    {EntityType::IF, "IF"},
    {EntityType::WHILE, "WHILE"},
    {EntityType::PROCEDURE, "PROCEDURE"},
    {EntityType::READ, "READ"},
    {EntityType::PRINT, "PRINT"},
    {EntityType::CALL, "CALL"},
    {EntityType::BOOLEAN, "BOOLEAN"},
    {EntityType::CONSTANT, "CONSTANT"},
    {EntityType::WILDCARD, "WILDCARD"},
    {EntityType::FIXED_INTEGER, "FIXED_INTEGER"},
    {EntityType::FIXED_STRING, "FIXED_STRING"},
    {EntityType::FIXED_STRING_WITHIN_WILDCARD, "FIXED_STRING_WITHIN_WILDCARD"},
    {EntityType::NULL_ENTITY, "NULL_ENTITY"}
};

const std::unordered_map<RelationshipType, std::string> Type::relationshipTypeToStringMap = {
    {RelationshipType::MODIFIES, "Modifies"},
    {RelationshipType::USES, "Uses"},
    {RelationshipType::PARENT, "Parent"},
    {RelationshipType::PARENT_T, "PARENT_T"},
    {RelationshipType::FOLLOWS, "Follows"},
    {RelationshipType::FOLLOWS_T, "FOLLOWS_T"},
    {RelationshipType::CALLS, "Calls"},
    {RelationshipType::CALLS_T, "CALLS_T"},
    {RelationshipType::NEXT, "Next"},
    {RelationshipType::NEXT_T, "NEXT_T"},
    {RelationshipType::AFFECTS, "Affects"},
    {RelationshipType::AFFECTS_T, "AFFECTS_T"},
    {RelationshipType::PATTERN, "PATTERN"},
    {RelationshipType::WITH, "WITH"},
    {RelationshipType::NULL_RELATIONSHIP, "NULL_ENTITY"}
};

const std::unordered_map<EntityAttributeType, std::string> Type::attributeTypeToStringMap = {
    {EntityAttributeType::PROCNAME, "PROCNAME"},
    {EntityAttributeType::VARNAME, "VARNAME"},
    {EntityAttributeType::VALUE, "VALUE"},
    {EntityAttributeType::STMT, "STMT"},
    {EntityAttributeType::NULL_ATTRIBUTE, "NULL_ENTITY"},
};

std::string Type::entityTypeToString(EntityType e){
    auto iter = Type::entityTypeToStringMap.find(e);
    return iter->second;
}


std::string Type::relationshipTypeToString(RelationshipType r) {
    auto iter = Type::relationshipTypeToStringMap.find(r);
    return iter->second;
}

std::string Type::attributeTypeToString(EntityAttributeType a) {
    auto iter = Type::attributeTypeToStringMap.find(a);
    return iter->second;
}

std::string Type::resultTypeToString(ResultType r) {
  auto iter = Type::resultTypeToStringMap.find(r);
  return iter->second;
}
