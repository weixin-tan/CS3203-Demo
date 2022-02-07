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
  FixedString
};

enum class RelationshipType{
  Modifies,
  Uses,
  Parent,
  ParentT,
  Follows,
  FollowsT
};

class Type{
 public:
  static constexpr const char* entityTypeStringArr[] = {
      "Statement",
      "Assignment",
      "Variable",
      "If",
      "While",
      "Procedure",
      "Read",
      "Print",
      "Call",
      "Boolean",
      "Constant",
      "Wildcard",
      "FixedInteger",
      "FixedString"
  };

  static constexpr const char* relationshipTypeStringArr[] = {"Modifies",
                                                              "Uses",
                                                              "Parent",
                                                              "ParentT",
                                                              "Follows",
                                                              "FollowsT"};
  static std::string entityTypeToString(EntityType e);

  static std::string relationshipTypeToString(RelationshipType r);;
};

#endif //SPA_TYPE_H
