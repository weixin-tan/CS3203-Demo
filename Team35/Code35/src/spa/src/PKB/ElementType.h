#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_ELEMENTTYPE_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_ELEMENTTYPE_H_

enum ElementType {
  // statements
  Statement,
  Read,
  Print,
  Call,
  While,
  If,
  Assignment,

  // procedures
  Procedure,

  // singles
  Variable,
  Constant,
};

bool isStatementType(const ElementType& element_type);

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_ELEMENTTYPE_H_
