#include "EntityToElementConverter.h"

EntityToElementConverter::EntityToElementConverter() = default;

ElementType EntityToElementConverter::extractElementType(const Entity& e) {
  switch (e.eType) {
    case EntityType::STATEMENT:
      return ElementType::STATEMENT;
    case EntityType::ASSIGNMENT:
      return ElementType::ASSIGNMENT;
    case EntityType::VARIABLE:
      return ElementType::VARIABLE;
    case EntityType::IF:
      return ElementType::IF;
    case EntityType::WHILE:
      return ElementType::WHILE;
    case EntityType::PROCEDURE:
      return ElementType::PROCEDURE;
    case EntityType::READ:
      return ElementType::READ;
    case EntityType::PRINT:
      return ElementType::PRINT;
    case EntityType::CALL:
      return ElementType::CALL;
    case EntityType::CONSTANT:
      return ElementType::CONSTANT;
    case EntityType::BOOLEAN:
      assert(false); //Not in Basic SPA
    default:
      assert(false);
  }
}

ProgramElement EntityToElementConverter::fixedEntityConverter(const Entity &e) {
  switch (e.eType) {
    case EntityType::FIXED_STRING:
      return ProgramElement::createVariable(e.name);
    case EntityType::FIXED_INTEGER:
      return ProgramElement::createStatement(ElementType::STATEMENT, std::stoi(e.name));
    case EntityType::FIXED_STRING_WITHIN_WILDCARD:
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
