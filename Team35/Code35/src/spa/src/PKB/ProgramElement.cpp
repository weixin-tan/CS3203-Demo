#include <cassert>
#include "ProgramElement.h"

const std::string ProgramElement::null_string_value;

// TODO: test suggestion of using std::move
ProgramElement::ProgramElement(ElementType element_type, int integer_value, const std::string& string_value) : element_type(element_type), integer_value(integer_value), string_value(string_value) {}

ProgramElement ProgramElement::createStatement(ElementType element_type, int stmt_no, const std::string& procedure_called) {
  if (!procedure_called.empty())
    assert(element_type == ElementType::Call);
  assert(isStatementType(element_type));
  return {element_type, stmt_no, procedure_called};
}

ProgramElement ProgramElement::createProcedure(const std::string& procedure_name) {
  return {ElementType::Procedure, ProgramElement::null_integer_value, procedure_name};
}

ProgramElement ProgramElement::createConstant(int constant_value) {
  return {ElementType::Constant, constant_value, ProgramElement::null_string_value};
}

ProgramElement ProgramElement::createVariable(const std::string& variable_name) {
  assert(!variable_name.empty());
  return {ElementType::Variable, ProgramElement::null_integer_value, variable_name};
}

bool ProgramElement::operator<(const ProgramElement& r) const {
  if (element_type != r.element_type)
    return element_type < r.element_type;
  if (integer_value != r.integer_value)
    return integer_value < r.integer_value;
  return string_value < r.string_value;
}
