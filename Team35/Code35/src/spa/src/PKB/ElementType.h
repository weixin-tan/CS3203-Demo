#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_ELEMENTTYPE_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_ELEMENTTYPE_H_

enum class ElementType {
    // statements
    STATEMENT,
    READ,
    PRINT,
    CALL,
    WHILE,
    IF,
    ASSIGNMENT,

    // procedures
    PROCEDURE,

    // singles
    VARIABLE,
    CONSTANT,
};

bool isStatementType(const ElementType& element_type);

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_ELEMENTTYPE_H_
