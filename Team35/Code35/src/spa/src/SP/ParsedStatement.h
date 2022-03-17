#ifndef PARSED_STATEMENT_H
#define PARSED_STATEMENT_H

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Statement.h"

class ParsedStatement {
    
public:
    static int DEFAULT_NULL_STMT_NO;
    static Expr defaultPattern;
    static std::string DEFAULT_PROCEDURE_NAME;

    int stmtNo; // statement number 
    int ifStmtNo; //if statement number refers to the statement number of if
    int whileStmtNo; // while statement number refers to the statement number of while
    StatementType statementType; // what kind of statement it is.
    Expr pattern = defaultPattern; 
    std::string procedureName;
    std::vector<std::string> varUsed;
    std::vector<std::string> varModified;
    std::string procedureCalled;
    std::vector<std::string> constant;
    int preceding = -1; 

    ParsedStatement();

    ParsedStatement(int stmtNo, int ifStmtNo, int whileStmtNo,
        StatementType statementType, Expr pattern, std::string procedureName,
        std::vector<std::string> varUsed, std::vector<std::string> varModified, std::vector<std::string> constant,
        std::string procedureCalled, int preceding);
};

#endif
