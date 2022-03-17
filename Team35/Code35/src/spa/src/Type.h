#ifndef SPA_TYPE_H
#define SPA_TYPE_H

#include <map>
#include <string>
#include <unordered_map>

enum class EntityType {
  Statement,
  Assignment,
  Variable,
  If,
  While,
  Procedure,
  Read,
  Print,
  Call,
  Boolean,
  Constant,
  Wildcard,
  FixedInteger,
  FixedString,
  FixedStringWithinWildcard,
  Null
};

enum class RelationshipType{
  Modifies,
  Uses,
  Parent,
  ParentT,
  Follows,
  FollowsT,
  Calls,
  CallsT,
  Next,
  NextT,
  Affects,
  AffectsT,
  Pattern,
  With,
  Null
};

enum class EntityAttributeType{
  ProcName,
  VarName,
  Value,
  Stmt,
  Null
};

enum class ResultType{
  NO_CLAUSE,
  SUCH_THAT_CLAUSE,
  PATTERN_CLAUSE,
  WITH_CLAUSE,
  NULL_CLAUSE
};

class Type{
 private:
  static const std::unordered_map<ResultType, std::string> resultTypeToStringMap;

 public:
  static std::string entityTypeToString(EntityType e);
  static std::string relationshipTypeToString(RelationshipType r);
  static std::string attributeTypeToString(EntityAttributeType a);
  static std::string resultTypeToString(ResultType r);
};

#endif //SPA_TYPE_H
