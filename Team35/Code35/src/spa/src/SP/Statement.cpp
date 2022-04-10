#include "Statement.h"

Statement::Statement() {

}

Statement::Statement(int stmtNo, StatementType statementType,
                     std::vector<std::string> varName, std::vector<std::string> expr,
                     std::vector<std::string> condExpr, std::string procName,
                     std::vector<std::string> constant, std::shared_ptr<StmtLst> ifthenStmtList,
                     std::shared_ptr<StmtLst> ifelseStmtList, std::shared_ptr<StmtLst> whileStmtList) {

    this->stmtNo = stmtNo;
    this->statementType = statementType;
    this->varName = varName;
    this->expr = expr;
    this->condExpr = condExpr;
    this->procName = procName;
    this->constant = constant;
    this->ifthenStmtList = ifthenStmtList;
    this->ifelseStmtList = ifelseStmtList;
    this->whileStmtList = whileStmtList;

}