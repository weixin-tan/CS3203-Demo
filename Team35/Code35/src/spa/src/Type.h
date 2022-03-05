#ifndef SPA_TYPE_H
#define SPA_TYPE_H

#include <map>
#include <string>

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

class Type{
 public:
  static std::string entityTypeToString(EntityType e);
  static std::string relationshipTypeToString(RelationshipType r);
  static std::string attributeTypeToString(EntityAttributeType a);
};

#endif //SPA_TYPE_H
