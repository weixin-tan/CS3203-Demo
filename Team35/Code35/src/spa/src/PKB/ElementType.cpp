#include "ElementType.h"

bool isStatementType(const ElementType& element_type) {
  return element_type == Statement
      || element_type == Read
      || element_type == Print
      || element_type == Call
      || element_type == While
      || element_type == If
      || element_type == Assignment;
}
