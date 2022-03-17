#include<stdio.h>

#ifndef STATEMENTTYPE_H
#define STATEMENTTYPE_H

enum class StatementType {
    ASSIGNMENT_STMT,
    PRINT_STMT,
    CALL_STMT,
    IF_STMT,
    WHILE_STMT,
    READ_STMT,
    PROCEDURE_STMT,
    NONE_STMT
};

#endif