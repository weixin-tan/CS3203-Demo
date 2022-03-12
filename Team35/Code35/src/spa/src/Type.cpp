#include <unordered_map>
#include "Type.h"

const std::unordered_map<ResultType, std::string> Type::resultTypeToStringMap = {
  {ResultType::NoClause, "NoClause"},
  {ResultType::SuchThatClause, "SuchThatClause"},
  {ResultType::PatternClause, "PatternClause"},
  {ResultType::WithClause, "WithClause"},
  {ResultType::NullClause, "NullClause"}
};

std::string Type::entityTypeToString(EntityType e){
  switch(e){
    case EntityType::Statement: return "Statement";
    case EntityType::Assignment: return "Assignment";
    case EntityType::Variable: return "Variable";
    case EntityType::If: return "If";
    case EntityType::While: return "While";
    case EntityType::Procedure: return "Procedure";
    case EntityType::Read: return "Read";
    case EntityType::Print: return "Print";
    case EntityType::Call: return "Call";
    case EntityType::Boolean: return "Boolean";
    case EntityType::Constant: return "Constant";
    case EntityType::Wildcard: return "Wildcard";
    case EntityType::FixedInteger: return "FixedInteger";
    case EntityType::FixedString: return "FixedString";
    case EntityType::FixedStringWithinWildcard: return "FixedStringWithinWildcard";
    case EntityType::Null: return "Null";
  }

}
std::string Type::relationshipTypeToString(RelationshipType r) {
  switch(r){
    case RelationshipType::Modifies: return "Modifies";
    case RelationshipType::Uses: return "Uses";
    case RelationshipType::Parent: return "Parent";
    case RelationshipType::ParentT: return "ParentT";
    case RelationshipType::Follows: return "Follows";
    case RelationshipType::FollowsT: return "FollowsT";
    case RelationshipType::Calls: return "Calls";
    case RelationshipType::CallsT: return "CallsT";
    case RelationshipType::Next: return "Next";
    case RelationshipType::NextT: return "NextT";
    case RelationshipType::Affects: return "Affects";
    case RelationshipType::AffectsT: return "AffectsT";
    case RelationshipType::Pattern: return "Pattern";
    case RelationshipType::With: return "With";
    case RelationshipType::Null: return "Null";
  }
}

std::string Type::attributeTypeToString(EntityAttributeType a) {
  switch(a){
    case EntityAttributeType::ProcName: return "ProcName";
    case EntityAttributeType::VarName: return "VarName";
    case EntityAttributeType::Value: return "Value";
    case EntityAttributeType::Stmt: return "Stmt";
    case EntityAttributeType::Null: return "Null";
  }
}

std::string Type::resultTypeToString(ResultType r) {
  auto iter = Type::resultTypeToStringMap.find(r);
  return iter->second;
}
