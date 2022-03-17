#include "ConcreteSyntaxWithValidation.h"

// Statement count
int stmt_count = 1;

// Default constructor
ConcreteSyntaxWithValidation::ConcreteSyntaxWithValidation() {
    stmt_count = 1;
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

// Returns a Procedure object that can be further processed by Convertor.
// tokensQueue is a queue of Token objects received from Tokeniser.
Procedure ConcreteSyntaxWithValidation::parseProcedure(std::queue<Token>& tokensQueue) {
    Procedure procedure;

    // procedure_keyword
    if (tokensQueue.front().getToken() != TokenType::PROCEDURE_KEYWORD) {
        throw std::invalid_argument("Missing procedure keyword.");
    }
    tokensQueue.pop();

    // procedure name
    if (tokensQueue.front().getToken() != TokenType::NAME) {
        throw std::invalid_argument("Missing procedure name.");
    }
    procedure.setProcName(tokensQueue.front().getId());
    tokensQueue.pop();

    // left_curly
    if (tokensQueue.front().getToken() != TokenType::LEFT_CURLY) {
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
    stmtLst.SetContainerType(ContainerType::kprocedure);
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

    while (tokensQueue.front().getToken() != TokenType::RIGHT_CURLY) {
        Statement temp_statement;
        try {
            temp_statement = ConcreteSyntaxWithValidation::parseStmt(tokensQueue);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
        stmtLst.setNextStmt(temp_statement);
    }
    // remove right_curly
    tokensQueue.pop();

    return stmtLst;
}

// Returns a Statement object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseStmt(std::queue<Token>& tokensQueue) {

    // check statement type
    if (tokensQueue.front().getToken() == TokenType::READ_KEYWORD) {
        try {
            return ConcreteSyntaxWithValidation::parseRead(tokensQueue);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
    }
    else if (tokensQueue.front().getToken() == TokenType::PRINT_KEYWORD) {
        try {
            return ConcreteSyntaxWithValidation::parsePrint(tokensQueue);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
    }
    else if (tokensQueue.front().getToken() == TokenType::CALL_KEYWORD) {
        try {
            return ConcreteSyntaxWithValidation::parseCall(tokensQueue);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
    }
    else if (tokensQueue.front().getToken() == TokenType::WHILE_KEYWORD) {
        try {
            return ConcreteSyntaxWithValidation::parseWhile(tokensQueue);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
    }
    else if (tokensQueue.front().getToken() == TokenType::IF_KEYWORD) {
        try {
            return ConcreteSyntaxWithValidation::parseIf(tokensQueue);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
    }
    else if (tokensQueue.front().getToken() == TokenType::NAME) {
        try {
            return ConcreteSyntaxWithValidation::parseAssign(tokensQueue);
        }
        catch (const std::invalid_argument& e) {
            throw;
        }
    }
    else {
        throw std::invalid_argument("Invalid statement declaration keyword.");
    }
}

// start for parsing assign

// Returns a Statement object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseAssign(std::queue<Token>& tokensQueue) {
    Statement assignStmt;
    assignStmt.stmtNo = stmt_count;
    stmt_count++;
    assignStmt.statementType = StatementType::ASSIGNMENT_STMT;

    // varName
    std::vector<std::string> result;
    if (tokensQueue.front().getToken() != TokenType::NAME) {
        throw std::invalid_argument("Missing variable name.");
    }
    result.push_back(tokensQueue.front().getId());
    assignStmt.varName = result;
    tokensQueue.pop();

    // single equal sign
    if (tokensQueue.front().getToken() != TokenType::ASSIGN) {
        throw std::invalid_argument("Missing equal sign.");
    }
    tokensQueue.pop();

    // Iteration 1 only passing vector of string
    std::vector<std::vector<std::string>> resultString;
    try {
        resultString = ConcreteSyntaxWithValidation::parseExprString(tokensQueue);
    }
    catch (const std::invalid_argument& e) {
        throw;
    }
    assignStmt.expr = resultString[0];
    assignStmt.constant = resultString[1];
    
    // Iteration 2 passing Expression class
    ExpressionProcessor ep = ExpressionProcessor();
    assignStmt.expression = ep.parseExpr(tokensQueue);

    // semicolon
    if (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
        throw std::invalid_argument("Missing semicolon.");
    }
    tokensQueue.pop();

    return assignStmt;
}

// for iteration 1.
// Returns a vector of vector of strings.
// tokensQueue is a queue of Token objects.
std::vector<std::vector<std::string>> ConcreteSyntaxWithValidation::parseExprString(std::queue<Token> tokensQueue) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> exprVector;
    std::vector<std::string> constVector;
    result.push_back(exprVector);
    result.push_back(constVector);

    // stop when semicolon is reached
    while (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
        if (tokensQueue.front().getToken() == TokenType::NAME) {
            result[0].push_back(tokensQueue.front().getId());
        }
        else if ((tokensQueue.front().getToken() == TokenType::INTEGER) || (tokensQueue.front().getToken() == TokenType::DIGIT)){
            result[1].push_back(tokensQueue.front().getId());
        }
        // other valid symbols in expr
        else if ((tokensQueue.front().getToken() == TokenType::LEFT_BRACE) ||
            (tokensQueue.front().getToken() == TokenType::RIGHT_BRACE) ||
            (tokensQueue.front().getToken() == TokenType::ADD) ||
            (tokensQueue.front().getToken() == TokenType::SUBTRACT) ||
            (tokensQueue.front().getToken() == TokenType::MULTIPLY) ||
            (tokensQueue.front().getToken() == TokenType::DIVIDE) ||
            (tokensQueue.front().getToken() == TokenType::MODULO)) {
            ;
        }
        else {
            throw std::invalid_argument("Invalid symbol in expression.");
        }
        tokensQueue.pop();
    }
    return result;
}

// end of chaining methods for parsing Assign


// start for parsing While

// Returns a Statement object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseWhile(std::queue<Token>& tokensQueue) {
    Statement whileStmt;
    whileStmt.stmtNo = stmt_count;
    stmt_count++;
    whileStmt.statementType = StatementType::WHILE_STMT;

    // parse condExpr
    // remove while_keyword
    if (tokensQueue.front().getToken() != TokenType::WHILE_KEYWORD) {
        throw std::invalid_argument("Missing while keyword.");
    }
    tokensQueue.pop();

    // remove left_brace
    if (tokensQueue.front().getToken() != TokenType::LEFT_BRACE) {
        throw std::invalid_argument("Missing left brace.");
    }
    tokensQueue.pop();

    // Iteration 1 only passing vector of string
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
    if (tokensQueue.front().getToken() != TokenType::LEFT_CURLY) {
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
    stmtLst.SetContainerType(ContainerType::kwhile);
    std::shared_ptr<StmtLst> p = std::make_shared<StmtLst>(stmtLst);
    whileStmt.whileStmtList = p;
    return whileStmt;
}

// Returns a vector of vector of strings.
// tokensQueue is a queue of Token objects.
// parse condExpr String for Iteration 1
std::vector<std::vector<std::string>> ConcreteSyntaxWithValidation::parseCondExprString(std::queue<Token>& tokensQueue) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> exprVector;
    std::vector<std::string> constVector;
    result.push_back(exprVector);
    result.push_back(constVector);
    int closure = 1;

    // stop when left_brace is reached
    while (closure != 0) {
        if (tokensQueue.front().getToken() == TokenType::LEFT_BRACE) {
            closure++;
        }
        else if (tokensQueue.front().getToken() == TokenType::RIGHT_BRACE) {
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
        else if (tokensQueue.front().getToken() == TokenType::NAME) {
            result[0].push_back(tokensQueue.front().getId());
        }
        else if ((tokensQueue.front().getToken() == TokenType::INTEGER) || (tokensQueue.front().getToken() == TokenType::DIGIT)) {
            result[1].push_back(tokensQueue.front().getId());
        }
        // invalid symbols in condExpr
        else if ((tokensQueue.front().getToken() == TokenType::SPECIALCHAR) ||
            (tokensQueue.front().getToken() == TokenType::SEMICOLON) ||
            (tokensQueue.front().getToken() == TokenType::LEFT_CURLY) ||
            (tokensQueue.front().getToken() == TokenType::RIGHT_CURLY) ||
            (tokensQueue.front().getToken() == TokenType::READ_KEYWORD) ||
            (tokensQueue.front().getToken() == TokenType::PRINT_KEYWORD) ||
            (tokensQueue.front().getToken() == TokenType::CALL_KEYWORD) ||
            (tokensQueue.front().getToken() == TokenType::WHILE_KEYWORD) ||
            (tokensQueue.front().getToken() == TokenType::IF_KEYWORD) ||
            (tokensQueue.front().getToken() == TokenType::PROCEDURE_KEYWORD)) {
            throw std::invalid_argument("Invalid symbol.");
        }

        tokensQueue.pop();
    }
    return result;
}

// not used for Iteration 1.
// Returns a CondExpr object.
// tokensQueue is a queue of Token objects.
// parse condExpr
CondExpr ConcreteSyntaxWithValidation::parseCondExpr(std::queue<Token>& tokensQueue) {
    std::queue<Token> condExprQueue;
    int closure = 1;
    while (closure != 0) {
        if (tokensQueue.front().getToken() == TokenType::LEFT_BRACE) {
            closure++;
        }
        if (tokensQueue.front().getToken() == TokenType::RIGHT_BRACE) {
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
        condExprQueue.push(tokensQueue.front());
        tokensQueue.pop();
    }
    return ConcreteSyntaxWithValidation::parseCondExprRecursion(condExprQueue);
}

// not used for Iteration 1.
// Returns a CondExpr object.
// condExprQueue is a queue of Token objects.
// parse condExpr recursive
CondExpr ConcreteSyntaxWithValidation::parseCondExprRecursion(std::queue<Token>& condExprQueue) {
    CondExpr condExpr;
    std::queue<Token> relExprQueue;
    int closure = 0;
    while (!condExprQueue.empty()) {
        if (condExprQueue.front().getToken() == TokenType::LEFT_BRACE) {
            closure++;
            condExprQueue.pop();
            continue;
        }
        if (condExprQueue.front().getToken() == TokenType::RIGHT_BRACE) {
            closure--;
            condExprQueue.pop();
            continue;
        }
        if (condExprQueue.front().getToken() == TokenType::NOT) {
            continue;
        }
        if ((closure == 0) && ((condExprQueue.front().getToken() == TokenType::AND) || (condExprQueue.front().getToken() == TokenType::OR))) {
            break;
        }
        relExprQueue.push(condExprQueue.front());
        condExprQueue.pop();
    }
    condExpr.setRelExpr(ConcreteSyntaxWithValidation::parseRelExpr(relExprQueue));

    if (!condExprQueue.empty()) {
        condExpr.setOperator(condExprQueue.front().getToken());
        condExprQueue.pop();
        condExpr.setCondExpr(ConcreteSyntaxWithValidation::parseCondExprRecursion(condExprQueue));
    }
    return condExpr;
}

// not used for Iteration 1.
// Returns a RelExpr object.
// relExprQueue is a queue of Token objects.
// parse rel_expr
RelExpr ConcreteSyntaxWithValidation::parseRelExpr(std::queue<Token>& relExprQueue) {
    RelExpr relExpr;
    std::queue<Token> relFactorOneQueue;
    std::queue<Token> relFactorTwoQueue;
    int closure = 0;

    // set rel_factor one
    while (!relExprQueue.empty()) {
        if (relExprQueue.front().getToken() == TokenType::LEFT_BRACE) {
            closure++;
        }
        if (relExprQueue.front().getToken() == TokenType::RIGHT_BRACE) {
            closure--;
        }
        if ((closure == 0) && ((relExprQueue.front().getToken() == TokenType::GREATER) || (relExprQueue.front().getToken() == TokenType::GEQ)
            || (relExprQueue.front().getToken() == TokenType::LESSER) || (relExprQueue.front().getToken() == TokenType::LEQ)
            || (relExprQueue.front().getToken() == TokenType::EQUAL) || (relExprQueue.front().getToken() == TokenType::NOT_EQUAL))) {
            break;
        }
        relFactorOneQueue.push(relExprQueue.front());
        relExprQueue.pop();
    }
    relExpr.setRelFactorOne(ConcreteSyntaxWithValidation::parseRelFactor(relFactorOneQueue));

    relExpr.setOperator(relExprQueue.front().getToken());
    relExprQueue.pop();

    // set rel_factor two
    while (!relExprQueue.empty()) {
        if (relExprQueue.front().getToken() == TokenType::LEFT_BRACE) {
            closure++;
        }
        if (relExprQueue.front().getToken() == TokenType::RIGHT_BRACE) {
            closure--;
        }
        if ((closure == 0) && ((relExprQueue.front().getToken() == TokenType::GREATER) || (relExprQueue.front().getToken() == TokenType::GEQ)
            || (relExprQueue.front().getToken() == TokenType::LESSER) || (relExprQueue.front().getToken() == TokenType::LEQ)
            || (relExprQueue.front().getToken() == TokenType::EQUAL) || (relExprQueue.front().getToken() == TokenType::NOT_EQUAL))) {
            break;
        }
        relFactorTwoQueue.push(relExprQueue.front());
        relExprQueue.pop();
    }
    relExpr.setRelFactorTwo(ConcreteSyntaxWithValidation::parseRelFactor(relFactorTwoQueue));
    return relExpr;
}

// not used for Iteration 1.
// Returns a RelFactor object.
// relFactorQueue is a queue of Token objects.
// parse rel_factor
RelFactor ConcreteSyntaxWithValidation::parseRelFactor(std::queue<Token>& relFactorQueue) {
    RelFactor relFactor;
    ExpressionProcessor ep = ExpressionProcessor();
    if (relFactorQueue.size() != 1) {
        std::stack<Token> exprStack;
        while (!relFactorQueue.empty()) {
            exprStack.push(relFactorQueue.front());
            relFactorQueue.pop();
        }
        relFactor.setExpr(ep.parseExprRecursion(exprStack));
        relFactor.setType(FactorType::EXPR);
    }
    else if (relFactorQueue.front().getToken() == TokenType::NAME) {
        relFactor.setVarName(relFactorQueue.front());
        relFactor.setType(FactorType::VAR);
        relFactorQueue.pop();
    }
    else if (relFactorQueue.front().getToken() == TokenType::INTEGER) {
        relFactor.setConstValue(relFactorQueue.front());
        relFactor.setType(FactorType::CONST);
        relFactorQueue.pop();
    }
    return relFactor;
}

// end for parsing While


// start for parsing If/Else

// Returns a Statement object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseIf(std::queue<Token>& tokensQueue) {
    Statement ifStmt;
    ifStmt.stmtNo = stmt_count;
    stmt_count++;
    ifStmt.statementType = StatementType::IF_STMT;

    // parse condExpr
    // remove if_keyword
    if (tokensQueue.front().getToken() != TokenType::IF_KEYWORD) {
        throw std::invalid_argument("Missing if keyword.");
    }
    tokensQueue.pop();
    // remove left_brace
    if (tokensQueue.front().getToken() != TokenType::LEFT_BRACE) {
        throw std::invalid_argument("Missing left brace.");
    }
    tokensQueue.pop();

    // Iteration 1 only passing vector of string
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
    if (tokensQueue.front().getToken() != TokenType::THEN_KEYWORD) {
        throw std::invalid_argument("Missing then keyword.");
    }
    tokensQueue.pop();
    // remove left_curly
    if (tokensQueue.front().getToken() != TokenType::LEFT_CURLY) {
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
    stmtLst.SetContainerType(ContainerType::kifthen);
    std::shared_ptr<StmtLst> p1 = std::make_shared<StmtLst>(stmtLst);
    ifStmt.ifthenStmtList = p1;

    // parse else stmtLst
    // remove else_keyword
    if (tokensQueue.front().getToken() != TokenType::ELSE_KEYWORD) {
        throw std::invalid_argument("Missing else keyword.");
    }
    tokensQueue.pop();
    // remove left_curly
    if (tokensQueue.front().getToken() != TokenType::LEFT_CURLY) {
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
    stmtLstElse.SetContainerType(ContainerType::kifelse);
    std::shared_ptr<StmtLst> p2 = std::make_shared<StmtLst>(stmtLstElse);
    ifStmt.ifelseStmtList = p2;

    return ifStmt;
}
// end for parsing If/Else
// done

// start for parsing Read

// Returns a Statement object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseRead(std::queue<Token>& tokensQueue) {
    Statement readStmt;
    readStmt.stmtNo = stmt_count;
    stmt_count++;
    readStmt.statementType = StatementType::READ_STMT;
    // remove read_keyword
    if (tokensQueue.front().getToken() != TokenType::READ_KEYWORD) {
        throw std::invalid_argument("Missing read keyword.");
    }
    tokensQueue.pop();
    // Iteration 1 only passing vector of string
    std::vector<std::string> result;
    if (tokensQueue.front().getToken() != TokenType::NAME) {
        throw std::invalid_argument("Missing variable name.");
    }
    result.push_back(tokensQueue.front().getId());
    readStmt.varName = result;
    // remove varName
    tokensQueue.pop();
    // remove semicolon
    if (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
        throw std::invalid_argument("Missing semicolon.");
    }
    tokensQueue.pop();
    return readStmt;
}
// end for parsing Read

// start for parsing Print

// Returns a Statement object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parsePrint(std::queue<Token>& tokensQueue) {
    Statement printStmt;
    printStmt.stmtNo = stmt_count;
    stmt_count++;
    printStmt.statementType = StatementType::PRINT_STMT;
    // remove print_keyword
    if (tokensQueue.front().getToken() != TokenType::PRINT_KEYWORD) {
        throw std::invalid_argument("Missing print keyword.");
    }
    tokensQueue.pop();
    // Iteration 1 only passing vector of string
    std::vector<std::string> result;
    if (tokensQueue.front().getToken() != TokenType::NAME) {
        throw std::invalid_argument("Missing variable name.");
    }
    result.push_back(tokensQueue.front().getId());
    printStmt.varName = result;
    // remove varName
    tokensQueue.pop();
    // remove semicolon
    if (tokensQueue.front().getToken() != TokenType::SEMICOLON) {
        throw std::invalid_argument("Missing semicolon.");
    }
    tokensQueue.pop();
    return printStmt;
}
// end for parsing Print


// start for parsing Call

// not used for Iteration 1
// Returns a Statement object.
// tokensQueue is a queue of Token objects.
Statement ConcreteSyntaxWithValidation::parseCall(std::queue<Token>& tokensQueue) {
    Statement callStmt;
    callStmt.stmtNo = stmt_count;
    stmt_count++;
    callStmt.statementType = StatementType::CALL_STMT;
    // remove call_keyword
    tokensQueue.pop();
    callStmt.procName = tokensQueue.front().getId();
    // remove procName
    tokensQueue.pop();
    // remove semicolon
    tokensQueue.pop();
    return callStmt;
}
// end for parsing Call