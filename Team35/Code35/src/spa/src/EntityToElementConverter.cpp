#include "EntityToElementConverter.h"

EntityToElementConverter::EntityToElementConverter() = default;

ElementType EntityToElementConverter::extractElementType(const Entity& e) {
  switch (e.eType) {
    case EntityType::Statement:
      return ElementType::kStatement;
    case EntityType::Assignment:
      return ElementType::kAssignment;
    case EntityType::Variable:
      return ElementType::kVariable;
    case EntityType::If:
      return ElementType::kIf;
    case EntityType::While:
      return ElementType::kWhile;
    case EntityType::Procedure:
      return ElementType::kProcedure;
    case EntityType::Read:
      return ElementType::kRead;
    case EntityType::Print:
      return ElementType::kPrint;
    case EntityType::Call:
      return ElementType::kCall;
    case EntityType::Constant:
      return ElementType::kConstant;
    case EntityType::Boolean:
      assert(false); //Not in Basic SPA
    default:
      assert(false);
  }
}

ProgramElement EntityToElementConverter::fixedEntityConverter(const Entity &e) {
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

bool EntityToElementConverter::isInteger(std::string s) {
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;
  return !s.empty() && it == s.end();
}

bool EntityToElementConverter::isValid(std::string s) {
  std::regex validRegex("^[a-zA-Z][A-Za-z0-9]*$");
  std::regex wildcardRegex("^[_]$");
  bool match = std::regex_match(s, validRegex);
  bool wildcardMatch = std::regex_match(s, wildcardRegex);
  bool empty = (s == "");
  return isInteger(s) || match || wildcardMatch || empty;
}
