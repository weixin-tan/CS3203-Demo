#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PROGRAMELEMENT_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PROGRAMELEMENT_H_

#include "ElementType.h"
#include <string>

// TODO: split into proper subclasses
class ProgramElement {
 public:
  static const int null_integer_value = -1;
  static const std::string null_string_value;

 public:
  const ElementType element_type;
  const int integer_value;
  const std::string string_value;

 private:
  ProgramElement(ElementType element_type, int integer_value, const std::string& string_value);

 public:
  static ProgramElement createStatement(ElementType element_type, int stmt_no);
  static ProgramElement createProcedure(const std::string& procedure_name);
  static ProgramElement createVariable(const std::string& variable_name);
  static ProgramElement createConstant(const std::string& constant_value);  // use string to avoid overflow

  std::string toString() const;

  bool operator<(const ProgramElement& r) const;
  bool operator==(const ProgramElement& r) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PROGRAMELEMENT_H_
