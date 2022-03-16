#include <cassert>
#include <regex>
#include "EntityToElementConverter.h"

EntityToElementConverter::EntityToElementConverter() = default;

ElementType EntityToElementConverter::extractElementType(const Entity& e) {
  switch (e.eType) {
    case EntityType::Statement:
      return ElementType::STATEMENT;
    case EntityType::Assignment:
      return ElementType::ASSIGNMENT;
    case EntityType::Variable:
      return ElementType::VARIABLE;
    case EntityType::If:
      return ElementType::IF;
    case EntityType::While:
      return ElementType::WHILE;
    case EntityType::Procedure:
      return ElementType::PROCEDURE;
    case EntityType::Read:
      return ElementType::READ;
    case EntityType::Print:
      return ElementType::PRINT;
    case EntityType::Call:
      return ElementType::CALL;
    case EntityType::Constant:
      return ElementType::CONSTANT;
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
      return ProgramElement::createStatement(ElementType::STATEMENT, std::stoi(e.name));
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
