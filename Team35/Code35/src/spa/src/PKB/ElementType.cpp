#include "ElementType.h"

bool isStatementType(const ElementType& element_type) {
  return element_type == ElementType::kStatement
      || element_type == ElementType::kRead
      || element_type == ElementType::kPrint
      || element_type == ElementType::kCall
      || element_type == ElementType::kWhile
      || element_type == ElementType::kIf
      || element_type == ElementType::kAssignment;
}
