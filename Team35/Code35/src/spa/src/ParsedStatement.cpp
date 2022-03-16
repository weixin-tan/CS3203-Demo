#include "ParsedStatement.h"

//Defining static const variables here

int ParsedStatement::DEFAULT_NULL_STMT_NO = -1;
Expr ParsedStatement::defaultPattern;
std::string ParsedStatement::DEFAULT_PROCEDURE_NAME = "";

ParsedStatement::ParsedStatement() {
    this->stmtNo = DEFAULT_NULL_STMT_NO; // statement number 
    this->ifStmtNo = DEFAULT_NULL_STMT_NO; //if statement number refers to the statement number of if
    this->whileStmtNo = DEFAULT_NULL_STMT_NO; // while statement number refers to the statement number of while
    this->statementType = StatementType::knone; // what kind of statement it is.
    this->pattern = defaultPattern; 
    this->procedureName = DEFAULT_PROCEDURE_NAME;
    this->varUsed;
    this->varModified;
    this->procedureCalled = DEFAULT_PROCEDURE_NAME;
    this->preceding = DEFAULT_NULL_STMT_NO;
    this->constant; 
}

ParsedStatement::ParsedStatement(int stmtNo, int ifStmtNo, int whileStmtNo,
    StatementType statementType, Expr pattern, std::string procedureName,
    std::vector<std::string> varUsed, std::vector<std::string> varModified, std::vector<std::string> constant,
    std::string procedureCalled, int preceding) {

    this->stmtNo = stmtNo;
    this->ifStmtNo = ifStmtNo; //if statement number refers to the statement number of if
    this->whileStmtNo = whileStmtNo; // while statement number refers to the statement number of while
    this->statementType = statementType; // what kind of statement it is.
    this->pattern = pattern; // implementation not done
    this->procedureName = procedureName;
    this->varUsed = varUsed;
    this->varModified = varModified;
    this->constant = constant;
    this->procedureCalled = procedureCalled;
    this->preceding = preceding;

}
