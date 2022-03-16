#include "ElementType.h"

bool isStatementType(const ElementType& element_type) {
  return element_type == ElementType::STATEMENT
      || element_type == ElementType::READ
      || element_type == ElementType::PRINT
      || element_type == ElementType::CALL
      || element_type == ElementType::WHILE
      || element_type == ElementType::IF
      || element_type == ElementType::ASSIGNMENT;
}
