#include <cassert>
#include <regex>
#include "QpsTypeToPkbTypeConvertor.h"

const std::unordered_map<RelationshipType, PkbRelationshipType> QpsTypeToPkbTypeConvertor::qpsRelToPkbRelMap = {
  {RelationshipType::Modifies, PkbRelationshipType::kModifies},
  {RelationshipType::Uses, PkbRelationshipType::kUses},
  {RelationshipType::Follows, PkbRelationshipType::kFollows},
  {RelationshipType::FollowsT, PkbRelationshipType::kFollowsT},
  {RelationshipType::Parent, PkbRelationshipType::kParent},
  {RelationshipType::ParentT, PkbRelationshipType::kParentT},
  {RelationshipType::Calls, PkbRelationshipType::kCalls},
  {RelationshipType::CallsT, PkbRelationshipType::kCallsT},
  {RelationshipType::Next, PkbRelationshipType::kNext},
  {RelationshipType::NextT, PkbRelationshipType::kNextT},
  {RelationshipType::Affects, PkbRelationshipType::kAffects},
  {RelationshipType::AffectsT, PkbRelationshipType::kAffectsT}
};

const std::unordered_map<EntityType, ElementType> QpsTypeToPkbTypeConvertor::entityToElementMap = {
  {EntityType::Statement, ElementType::kStatement},
  {EntityType::Assignment, ElementType::kAssignment},
  {EntityType::Variable, ElementType::kVariable},
  {EntityType::Constant, ElementType::kConstant},
  {EntityType::If, ElementType::kIf},
  {EntityType::While, ElementType::kWhile},
  {EntityType::Procedure, ElementType::kProcedure},
  {EntityType::Read, ElementType::kRead},
  {EntityType::Print, ElementType::kPrint},
  {EntityType::Call, ElementType::kCall}
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


ProgramElement QpsTypeToPkbTypeConvertor::fixedEntityConverter(const Entity &e) {
  switch (e.eType) {
    case EntityType::FixedString:
      return ProgramElement::createVariable(e.name);
    case EntityType::FixedInteger:
      return ProgramElement::createStatement(ElementType::kStatement, std::stoi(e.name));
    case EntityType::FixedStringWithinWildcard:
      if (isInteger(e.name)) {
        return ProgramElement::createConstant(e.name);
      } else {
        return ProgramElement::createVariable(e.name);
      }
    default:
      assert(false);
  }
}

bool QpsTypeToPkbTypeConvertor::isInteger(std::string s) {
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;
  return !s.empty() && it == s.end();
}

bool QpsTypeToPkbTypeConvertor::isValid(std::string s) {
  std::regex validRegex("^[a-zA-Z][A-Za-z0-9]*$");
  std::regex wildcardRegex("^[_]$");
  bool match = std::regex_match(s, validRegex);
  bool wildcardMatch = std::regex_match(s, wildcardRegex);
  bool empty = (s == "");
  return isInteger(s) || match || wildcardMatch || empty;
}


