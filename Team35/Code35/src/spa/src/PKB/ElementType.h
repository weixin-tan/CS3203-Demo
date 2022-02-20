#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_ELEMENTTYPE_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_ELEMENTTYPE_H_

enum ElementType {
  // statements
  kStatement,
  kRead,
  kPrint,
  kCall,
  kWhile,
  kIf,
  kAssignment,

  // procedures
  kProcedure,

  // singles
  kVariable,
  kConstant,
};

bool isStatementType(const ElementType& element_type);

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_ELEMENTTYPE_H_
