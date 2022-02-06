#include "Type.h"

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
  }
}