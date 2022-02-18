//
// Created by viv on 18/2/2022.
//

#include <cassert>
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
      default:
        assert(false);
  }
}

