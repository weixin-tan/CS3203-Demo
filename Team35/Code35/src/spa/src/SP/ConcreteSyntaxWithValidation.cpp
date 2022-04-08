#include "ConcreteSyntaxWithValidation.h"

// STATEMENT count
int stmt_count = 1;

// Default constructor
ConcreteSyntaxWithValidation::ConcreteSyntaxWithValidation() {
    stmt_count = 1;
    this->tokenStatementFunctionMap = ConcreteSyntaxWithValidation::initialiseTokenMap();
}

std::map<TokenType, Statement(ConcreteSyntaxWithValidation::*)(std::queue<Token>& tokensQueue)> ConcreteSyntaxWithValidation::initialiseTokenMap() {
    std::map<TokenType, Statement(ConcreteSyntaxWithValidation::*)(std::queue<Token>& tokensQueue)> tokenStatementFunctionMap; 

    tokenStatementFunctionMap.emplace(TokenType::READ_KEYWORD, &ConcreteSyntaxWithValidation::parseRead);
    tokenStatementFunctionMap.emplace(TokenType::PRINT_KEYWORD, &ConcreteSyntaxWithValidation::parsePrint);
    tokenStatementFunctionMap.emplace(TokenType::CALL_KEYWORD, &ConcreteSyntaxWithValidation::parseCall);
    tokenStatementFunctionMap.emplace(TokenType::WHILE_KEYWORD, &ConcreteSyntaxWithValidation::parseWhile);
    tokenStatementFunctionMap.emplace(TokenType::IF_KEYWORD, &ConcreteSyntaxWithValidation::parseIf);
    tokenStatementFunctionMap.emplace(TokenType::NAME, &ConcreteSyntaxWithValidation::parseAssign);

    return tokenStatementFunctionMap;
}

// Returns a Program object containing a ProcedureLst object.
// // tokensQueue is a queue of Token objects received from Tokeniser.
Program ConcreteSyntaxWithValidation::parseProgram(std::queue<Token> tokensQueue) {
    Program program;
    ProcedureLst procedureLst;

    while (tokensQueue.size() != 0) {
        try {
            Procedure temp_procedure = ConcreteSyntaxWithValidation::parseProcedure(tokensQueue);
            procedureLst.setNextProcedure(temp_procedure);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
    }

    program.setProcedureLst(procedureLst);
    return program;
}

// Returns a PROCEDURE object that can be further processed by Convertor.
// tokensQueue is a queue of Token objects received from Tokeniser.
Procedure ConcreteSyntaxWithValidation::parseProcedure(std::queue<Token>& tokensQueue) {
    Procedure procedure;

    // procedure_keyword
    if (!isFrontQueueTokenType(tokensQueue, TokenType::PROCEDURE_KEYWORD)) {
        throw std::invalid_argument("Missing procedure keyword.");
    }
    tokensQueue.pop();

    // procedure name
    if (!isFrontQueueTokenType(tokensQueue, TokenType::NAME)) {
        throw std::invalid_argument("Missing procedure name.");
    }
    procedure.setProcName(tokensQueue.front().getId());
    tokensQueue.pop();

    // left_curly
    if (!isFrontQueueTokenType(tokensQueue, TokenType::LEFT_CURLY)) {
        throw std::invalid_argument("Missing left curly for procedure.");
    }
    tokensQueue.pop();

    StmtLst stmtLst;
    try {
        stmtLst = ConcreteSyntaxWithValidation::parseStmtLst(tokensQueue);
    }
    catch (const std::invalid_argument& e){
        throw;
    }
    stmtLst.SetContainerType(ContainerType::PROCEDURE_CONTAINER);
    procedure.setStmtLst(stmtLst);
    procedure.setSize(stmt_count);

    return procedure;
}

// Returns a StmtLst object.
// tokensQueue is a queue of Token objects.
StmtLst ConcreteSyntaxWithValidation::parseStmtLst(std::queue<Token>& tokensQueue) {
    StmtLst stmtLst;

    // right_curly
    if (tokensQueue.empty()) {
        throw std::invalid_argument("Missing right curly for procedure.");
    }

    while ((!tokensQueue.empty()) && (!isFrontQueueTokenType(tokensQueue, TokenType::RIGHT_CURLY))) {

        Statement temp_statement;
        try {
            temp_statement = ConcreteSyntaxWithValidation::parseStmt(tokensQueue);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
        stmtLst.setNextStmt(temp_statement);
    }

    if (tokensQueue.empty()) {
        throw std::invalid_argument("Missing right curly.");
    }

    // remove right_curly
    if ((!tokensQueue.empty()) && (isFrontQueueTokenType(tokensQueue, TokenType::RIGHT_CURLY))) {
        tokensQueue.pop();
    }

    if (stmtLst.getSize() == 0) {
        throw std::invalid_argument("Empty statement list.");
    }

    return stmtLst;
}

// Returns a STATEMENT object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseStmt(std::queue<Token>& tokensQueue) {
    TokenType frontTokenType = tokensQueue.front().getToken();
    try {
        if (frontTokenType != TokenType::READ_KEYWORD && frontTokenType != TokenType::PRINT_KEYWORD
            && frontTokenType != TokenType::CALL_KEYWORD && frontTokenType != TokenType::WHILE_KEYWORD
            && frontTokenType != TokenType::IF_KEYWORD && frontTokenType != TokenType::NAME) {
            throw std::invalid_argument("Invalid statement declaration keyword.");
        }
        return (this->*tokenStatementFunctionMap.at(frontTokenType))(tokensQueue);
    }
    catch (const std::invalid_argument& e) {
        throw;
    }
}

// Returns a STATEMENT object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseAssign(std::queue<Token>& tokensQueue) {
    Statement assignStmt;
    assignStmt.stmtNo = stmt_count;
    stmt_count++;
    assignStmt.statementType = StatementType::ASSIGNMENT_STMT;

    // varName
    std::vector<std::string> result;
    if (!isFrontQueueTokenType(tokensQueue, TokenType::NAME)) {
        throw std::invalid_argument("Missing variable name.");
    }
    result.push_back(tokensQueue.front().getId());
    assignStmt.varName = result;
    tokensQueue.pop();

    // equal sign
    if (!isFrontQueueTokenType(tokensQueue, TokenType::ASSIGN)) {
        throw std::invalid_argument("Missing equal sign.");
    }
    tokensQueue.pop();

    // pass vector of string
    std::vector<std::vector<std::string>> resultString;
    try {
        resultString = ConcreteSyntaxWithValidation::parseExprString(tokensQueue);
    }
    catch (const std::invalid_argument& e) {
        throw;
    }
    assignStmt.expr = resultString[0];
    assignStmt.constant = resultString[1];
    
    // pass Expression class
    ExpressionProcessor ep = ExpressionProcessor();

    try {
        assignStmt.expression = ep.parseExpr(tokensQueue);
    }
    catch (const std::invalid_argument& e) {
        throw;
    }

    // semicolon
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::SEMICOLON))) {
        throw std::invalid_argument("Missing semicolon.");
    }
    tokensQueue.pop();

    return assignStmt;
}

// Returns a vector of vector of strings.
// tokensQueue is a queue of Token objects.
std::vector<std::vector<std::string>> ConcreteSyntaxWithValidation::parseExprString(std::queue<Token> tokensQueue) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> exprVector;
    std::vector<std::string> constVector;
    result.push_back(exprVector);
    result.push_back(constVector);

    // stop when semicolon is reached
    while ((!tokensQueue.empty()) && (!isFrontQueueTokenType(tokensQueue, TokenType::SEMICOLON))) {
        if (isFrontQueueTokenType(tokensQueue, TokenType::NAME)) {
            result[0].push_back(tokensQueue.front().getId());
        }
        else if (isFrontQueueTokenType(tokensQueue, TokenType::INTEGER) || isFrontQueueTokenType(tokensQueue, TokenType::DIGIT)){
            result[1].push_back(tokensQueue.front().getId());
        }
        // other valid symbols in expr
        else if ((isFrontQueueTokenType(tokensQueue, TokenType::LEFT_BRACE)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::RIGHT_BRACE)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::ADD)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::SUBTRACT)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::MULTIPLY)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::DIVIDE)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::MODULO))) {
            ;
        }
        else {
            throw std::invalid_argument("Invalid symbol in expression.");
        }
        tokensQueue.pop();
    }
    return result;
}

// Returns a STATEMENT object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseWhile(std::queue<Token>& tokensQueue) {
    Statement whileStmt;
    whileStmt.stmtNo = stmt_count;
    stmt_count++;
    whileStmt.statementType = StatementType::WHILE_STMT;

    // parse condExpr
    // remove while_keyword
    if (!isFrontQueueTokenType(tokensQueue, TokenType::WHILE_KEYWORD)) {
        throw std::invalid_argument("Missing while keyword.");
    }
    tokensQueue.pop();

    // remove left_brace
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::LEFT_BRACE))) {
        throw std::invalid_argument("Missing left brace.");
    }
    tokensQueue.pop();

    // pass vector of string
    std::vector<std::vector<std::string>> resultString;
    try {
        resultString = ConcreteSyntaxWithValidation::parseCondExprString(tokensQueue);
    }
    catch (const std::invalid_argument& e) {
        throw;
    }
    whileStmt.condExpr = resultString[0];
    // pass constant
    whileStmt.constant = resultString[1];

    // parse stmtLst
    // remove left_curly
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::LEFT_CURLY))) {
        throw std::invalid_argument("Missing left curly.");
    }
    tokensQueue.pop();

    StmtLst stmtLst;
    try {
        stmtLst = ConcreteSyntaxWithValidation::parseStmtLst(tokensQueue);
    }
    catch (const std::invalid_argument& e) {
        throw;
    }
    stmtLst.SetContainerType(ContainerType::WHILE_CONTAINER);
    std::shared_ptr<StmtLst> p = std::make_shared<StmtLst>(stmtLst);
    whileStmt.whileStmtList = p;
    return whileStmt;
}

// Returns a vector of vector of strings.
// tokensQueue is a queue of Token objects.
// parse condExpr String
std::vector<std::vector<std::string>> ConcreteSyntaxWithValidation::parseCondExprString(std::queue<Token>& tokensQueue) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> exprVector;
    std::vector<std::string> constVector;
    result.push_back(exprVector);
    result.push_back(constVector);
    int closure = 1;

    // stop when final right_brace is reached
    while (closure != 0) {

        if (tokensQueue.empty()) {
            throw std::invalid_argument("Incomplete conditional expression.");
            break;
        }

        if (isFrontQueueTokenType(tokensQueue, TokenType::LEFT_BRACE)) {
            closure++;
        }
        else if (isFrontQueueTokenType(tokensQueue, TokenType::RIGHT_BRACE))  {
            if (closure == 1) {
                closure--;
                // remove right_brace
                tokensQueue.pop();
                break;
            }
            else {
                closure--;
            }
        }
        else if (isFrontQueueTokenType(tokensQueue, TokenType::NAME)) {
            result[0].push_back(tokensQueue.front().getId());
        }
        else if (isFrontQueueTokenType(tokensQueue, TokenType::INTEGER) || isFrontQueueTokenType(tokensQueue, TokenType::DIGIT)) {
            result[1].push_back(tokensQueue.front().getId());
        }
        // invalid symbols in condExpr
        else if ((isFrontQueueTokenType(tokensQueue, TokenType::SPECIALCHAR)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::SEMICOLON)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::LEFT_CURLY)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::RIGHT_CURLY)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::READ_KEYWORD)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::PRINT_KEYWORD)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::CALL_KEYWORD)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::WHILE_KEYWORD)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::IF_KEYWORD)) ||
            (isFrontQueueTokenType(tokensQueue, TokenType::PROCEDURE_KEYWORD))) {
            throw std::invalid_argument("Invalid symbol.");
        }

        tokensQueue.pop();
    }
    return result;
}

// Returns a STATEMENT object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseIf(std::queue<Token>& tokensQueue) {
    Statement ifStmt;
    ifStmt.stmtNo = stmt_count;
    stmt_count++;
    ifStmt.statementType = StatementType::IF_STMT;

    // parse condExpr
    // remove if_keyword
    if (!isFrontQueueTokenType(tokensQueue, TokenType::IF_KEYWORD)) {
        throw std::invalid_argument("Missing if keyword.");
    }
    tokensQueue.pop();
    // remove left_brace
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::LEFT_BRACE))) {
        throw std::invalid_argument("Missing left brace.");
    }
    tokensQueue.pop();

    // pass vector of string
    std::vector<std::vector<std::string>> resultString;
    try {
        resultString = ConcreteSyntaxWithValidation::parseCondExprString(tokensQueue);
    }
    catch (const std::invalid_argument& e) {
        throw;
    }
    ifStmt.condExpr = resultString[0];
    // pass constant
    ifStmt.constant = resultString[1];

    // parse then stmtLst
    // remove then_keyword
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::THEN_KEYWORD))) {
        throw std::invalid_argument("Missing then keyword.");
    }
    tokensQueue.pop();
    // remove left_curly
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::LEFT_CURLY))) {
        throw std::invalid_argument("Missing left curly.");
    }
    tokensQueue.pop();

    StmtLst stmtLst;
    try {
        stmtLst = ConcreteSyntaxWithValidation::parseStmtLst(tokensQueue);
    }
    catch (const std::invalid_argument& e) {
        throw;
    }
    // set then stmtLst
    stmtLst.SetContainerType(ContainerType::IF_THEN_CONTAINER);
    std::shared_ptr<StmtLst> p1 = std::make_shared<StmtLst>(stmtLst);
    ifStmt.ifthenStmtList = p1;

    // parse else stmtLst
    // remove else_keyword
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::ELSE_KEYWORD))) {
        throw std::invalid_argument("Missing else keyword.");
    }
    tokensQueue.pop();
    // remove left_curly
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::LEFT_CURLY))) {
        throw std::invalid_argument("Missing left curly.");
    }
    tokensQueue.pop();

    StmtLst stmtLstElse;
    try {
        stmtLstElse = ConcreteSyntaxWithValidation::parseStmtLst(tokensQueue);
    }
    catch (const std::invalid_argument& e) {
        throw;
    }
    // set else stmtLst
    stmtLstElse.SetContainerType(ContainerType::IF_ELSE_CONTAINER);
    std::shared_ptr<StmtLst> p2 = std::make_shared<StmtLst>(stmtLstElse);
    ifStmt.ifelseStmtList = p2;

    return ifStmt;
}

// Returns a STATEMENT object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseRead(std::queue<Token>& tokensQueue) {
    Statement readStmt;
    readStmt.stmtNo = stmt_count;
    stmt_count++;
    readStmt.statementType = StatementType::READ_STMT;
    // remove read_keyword
    if (!isFrontQueueTokenType(tokensQueue, TokenType::READ_KEYWORD)) {
        throw std::invalid_argument("Missing read keyword.");
    }
    tokensQueue.pop();
    // pass vector of string
    std::vector<std::string> result;
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::NAME))) {
        throw std::invalid_argument("Missing variable name.");
    }
    result.push_back(tokensQueue.front().getId());
    readStmt.varName = result;
    // remove varName
    tokensQueue.pop();
    // remove semicolon
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::SEMICOLON))) {
        throw std::invalid_argument("Missing semicolon.");
    }
    tokensQueue.pop();
    return readStmt;
}

// Returns a STATEMENT object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parsePrint(std::queue<Token>& tokensQueue) {
    Statement printStmt;
    printStmt.stmtNo = stmt_count;
    stmt_count++;
    printStmt.statementType = StatementType::PRINT_STMT;
    // remove print_keyword
    if (!isFrontQueueTokenType(tokensQueue, TokenType::PRINT_KEYWORD)) {
        throw std::invalid_argument("Missing print keyword.");
    }
    tokensQueue.pop();
    // pass vector of string
    std::vector<std::string> result;
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::NAME))) {
        throw std::invalid_argument("Missing variable name.");
    }
    result.push_back(tokensQueue.front().getId());
    printStmt.varName = result;
    // remove varName
    tokensQueue.pop();
    // remove semicolon
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::SEMICOLON))) {
        throw std::invalid_argument("Missing semicolon.");
    }
    tokensQueue.pop();
    return printStmt;
}

// Returns a STATEMENT object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseCall(std::queue<Token>& tokensQueue) {
    Statement callStmt;
    callStmt.stmtNo = stmt_count;
    stmt_count++;
    callStmt.statementType = StatementType::CALL_STMT;
    // remove call_keyword
    if (!isFrontQueueTokenType(tokensQueue, TokenType::CALL_KEYWORD)) {
        throw std::invalid_argument("Missing call keyword.");
    }
    tokensQueue.pop();
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::NAME))) {
        throw std::invalid_argument("Missing procedure call name.");
    }
    callStmt.procName = tokensQueue.front().getId();
    // remove procName
    tokensQueue.pop();
    // remove semicolon
    if ((tokensQueue.empty()) || (!isFrontQueueTokenType(tokensQueue, TokenType::SEMICOLON))) {
        throw std::invalid_argument("Missing semicolon.");
    }
    tokensQueue.pop();
    return callStmt;
}

// Helper functions
bool ConcreteSyntaxWithValidation::isFrontQueueTokenType(std::queue<Token>& tokensQueue, TokenType tokenType) {
    return tokensQueue.front().getToken() == tokenType;
}