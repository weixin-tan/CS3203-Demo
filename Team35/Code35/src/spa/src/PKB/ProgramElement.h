#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PROGRAMELEMENT_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PROGRAMELEMENT_H_

#include "ElementType.h"
#include <string>

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
  static ProgramElement createStatement(ElementType element_type, int stmt_no, const std::string& procedure_called);
  static ProgramElement createProcedure(const std::string& procedure_name);
  static ProgramElement createVariable(const std::string& variable_name);
  static ProgramElement createConstant(int constant_value);

  bool operator<(const ProgramElement& r) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PROGRAMELEMENT_H_
