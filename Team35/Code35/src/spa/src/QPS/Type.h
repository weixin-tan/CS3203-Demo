#ifndef SPA_TYPE_H
#define SPA_TYPE_H

#include <map>
#include <string>
#include <unordered_map>

enum class EntityType {
    STATEMENT,
    ASSIGNMENT,
    VARIABLE,
    IF,
    WHILE,
    PROCEDURE,
    READ,
    PRINT,
    CALL,
    BOOLEAN,
    CONSTANT,
    WILDCARD,
    FIXED_INTEGER,
    FIXED_STRING,
    FIXED_STRING_WITHIN_WILDCARD,
    NULL_ENTITY
};

enum class RelationshipType {
    MODIFIES,
    USES,
    PARENT,
    PARENT_T,
    FOLLOWS,
    FOLLOWS_T,
    CALLS,
    CALLS_T,
    NEXT,
    NEXT_T,
    AFFECTS,
    AFFECTS_T,
    PATTERN,
    WITH,
    NULL_RELATIONSHIP
};

enum class EntityAttributeType {
    PROCNAME,
    VARNAME,
    VALUE,
    STMT,
    NULL_ATTRIBUTE
};

enum class ResultType {
    NO_CLAUSE,
    SUCH_THAT_CLAUSE,
    PATTERN_CLAUSE,
    WITH_CLAUSE,
    NULL_CLAUSE
};

enum class FormattedResultType {
    BOOLEAN,
    SINGLE,
    TUPLE,
    NULL_RESULT
};

class Type {
private:
    static const std::unordered_map<ResultType, std::string> resultTypeToStringMap;
    static const std::unordered_map<EntityType, std::string> entityTypeToStringMap;
    static const std::unordered_map<RelationshipType, std::string> relationshipTypeToStringMap;
    static const std::unordered_map<EntityAttributeType, std::string> attributeTypeToStringMap;
    static const std::unordered_map<FormattedResultType, std::string> formattedResultTypeToStringMap;

public:
    static std::string entityTypeToString(EntityType e);
    static std::string relationshipTypeToString(RelationshipType r);
    static std::string attributeTypeToString(EntityAttributeType a);
    static std::string resultTypeToString(ResultType r);
    static std::string formattedResultTypeToString(FormattedResultType f);
};

#endif //SPA_TYPE_H
