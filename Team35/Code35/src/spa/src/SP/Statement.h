#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <vector>
#include <memory>
#include "StatementType.h"
#include "StmtLst.h"
#include "Expr.h"

class StmtLst;

class Statement {
public:
    Statement();

    Statement(int stmtNo, StatementType statementType,
        std::vector<std::string> varName, std::vector<std::string> expr,
        std::vector<std::string> condExpr, std::string procName,
        std::vector<std::string> constant, std::shared_ptr<StmtLst> ifthenStmtList,
        std::shared_ptr<StmtLst> ifelseStmtList, std::shared_ptr<StmtLst> whileStmtList);

    int stmtNo;

    StatementType statementType;
    
    // For read, print and assignment statements
    std::vector<std::string> varName;
    
    // For assignment statements
    std::vector<std::string> expr;
    Expr expression;

    // for while, if statements
    std::vector<std::string> condExpr;
    Expr condExpression;

    // for call statements
    std::string procName;

    //for Constant variables
    std::vector<std::string> constant; 

    // for then, else, while statements. 
    std::shared_ptr<StmtLst> ifthenStmtList;
    std::shared_ptr<StmtLst> ifelseStmtList;
    std::shared_ptr<StmtLst> whileStmtList;

};
#endif
