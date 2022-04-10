#include "Convertor.h"

std::string Convertor::currProcedureName = "no_procedure";
std::vector<ParsedStatement> Convertor::finalResults;


Convertor::Convertor(PkbSetter* pkb_setter) {
    this->pkb_setter = pkb_setter;
    this->statementFunctionMap = Convertor::initialiseMap();
}

// Reads the procedurelist and calls a statemnet list reader for each procedure in the list.
std::vector<std::vector<ParsedStatement>> Convertor::procedureReader(ProcedureLst procedurelist) {
    std::vector<std::vector<ParsedStatement>> results;

    for (int i = 0; i < procedurelist.getSize(); i++) {
        Procedure curr_procedure = procedurelist.getProcedureAtIndex(i);
        currProcedureName = curr_procedure.getProcName();

        results.push_back(statementListReader(procedurelist.getProcedureAtIndex(i).getStmtLst(), -1));

        // TODO: Sending the statement list to the PKB - should be removed 
        ///for (const ParsedStatement i : finalResults) {
        //	pkb_setter->insertStmt(i);
        //}

        //resetting the final results
        finalResults = std::vector<ParsedStatement>();
    }

    pkb_setter->insertStmts(results);
    return results;
}

// Identifes the type of statementlist, and also includes the line number of the container. 
// For Procedures, since there are no line numbers, line number will be -1. 
// Creates a stack for the line numbers of the statement container..
std::vector<ParsedStatement> Convertor::statementListReader(StmtLst statementList, int containerNumber) {
    // Gets the container type. IF it is a procedure, then the current procedure(static) will
    // be that procedure. 
    ContainerType containerType = statementList.getContainerType();

    //creating new stack and pushing in -1 line number. 
    std::stack<int> nestedstack;
    nestedstack.push(-1);

// Reading every statement in the container. 
    for (int i = 0; i < statementList.getSize(); i++) {
        finalResults.push_back(this->readStatement(statementList.getStmtAtIndex(i), containerType, nestedstack, containerNumber));
    }

    return finalResults;
}

// Reads and converts the statement into a parsedStatement object.
// stmt parameter refers to statement
// ContainerType refers to the type of statementlist. 
// nestedStack refers to the stack of line numbers for that particular container. 
// containerNum refers to the statement number of the container. 

// returns a parsed statement. Note that due to recursive return calls, the order of the returned statements
// would not follow the same format as the SIMPLE source code. Hence, a vector could be used to store and sort
// based on statement number. 
ParsedStatement Convertor::readStatement(Statement stmt, ContainerType containerType,
    std::stack<int> &nestedStack, int containerNum) {

    ParsedStatement current_statement; 
    current_statement.stmtNo = stmt.stmtNo;
    current_statement.procedureName = currProcedureName;
    current_statement.preceding = nestedStack.top();
    current_statement.statementType = stmt.statementType;

    //populate container type (parent)
    switch (containerType) {
    case ContainerType::IF_THEN_CONTAINER:
    case ContainerType::IF_ELSE_CONTAINER:
        current_statement.ifStmtNo = containerNum;
        break;
    case ContainerType::WHILE_CONTAINER:
        current_statement.whileStmtNo = containerNum;
        break;
    case ContainerType::PROCEDURE_CONTAINER:
        break;
    default:
        throw std::invalid_argument("no such container type");
        break;
    }

    //push the value into the stack
    nestedStack.push(stmt.stmtNo);

    //Check the statement types and extract required values
    try {
        current_statement = (this->*statementFunctionMap.at(stmt.statementType))(stmt, &current_statement);
    } catch(std::out_of_range& e){
        throw std::invalid_argument("no such statement type");
    }

    return current_statement; 
}


ParsedStatement Convertor::extractAssignmentStatement(Statement stmt, ParsedStatement *current_statement) {
    current_statement->varModified = stmt.varName;
    current_statement->varUsed = stmt.expr;
    current_statement->constant = stmt.constant;
    current_statement->pattern = stmt.expression;
    return *current_statement; 
}

ParsedStatement Convertor::extractIfStatement(Statement stmt, ParsedStatement *current_statement) {
    
    current_statement->varUsed = stmt.condExpr;
    current_statement->constant = stmt.constant;
    current_statement->pattern = stmt.expression;

    // In this case, if statement will have 2 statement lists (if and then) 
    this->statementListReader(*stmt.ifthenStmtList.get(), stmt.stmtNo);
    this->statementListReader(*stmt.ifelseStmtList.get(), stmt.stmtNo);
    return *current_statement;
}

ParsedStatement Convertor::extractCallStatement(Statement stmt, ParsedStatement *current_statement) {
    current_statement->procedureCalled = stmt.procName;
    return *current_statement;
}

ParsedStatement Convertor::extractWhileStatement(Statement stmt, ParsedStatement* current_statement) {
    current_statement->varUsed = stmt.condExpr;
    current_statement->constant = stmt.constant;
    current_statement->pattern = stmt.expression;
    //In this case, this statement will a while statement list. 
    this->statementListReader(*stmt.whileStmtList.get(), stmt.stmtNo);
    return *current_statement;
}

ParsedStatement Convertor::extractPrintStatement(Statement stmt, ParsedStatement* current_statement) {
    current_statement->varUsed = stmt.varName;
    return *current_statement;
}

ParsedStatement Convertor::extractReadStatement(Statement stmt, ParsedStatement* current_statement) {
    current_statement->varModified = stmt.varName;
    return *current_statement;
}

std::map<StatementType, ParsedStatement(Convertor::*)(Statement, ParsedStatement*)> Convertor::initialiseMap() {
    std::map<StatementType, ParsedStatement(Convertor::*)(Statement, ParsedStatement*)> statementFunctionMap;

    statementFunctionMap[StatementType::ASSIGNMENT_STMT] = &Convertor::extractAssignmentStatement;
    statementFunctionMap[StatementType::IF_STMT] = &Convertor::extractIfStatement;
    statementFunctionMap[StatementType::WHILE_STMT] = &Convertor::extractWhileStatement;
    statementFunctionMap[StatementType::PRINT_STMT] = &Convertor::extractPrintStatement;
    statementFunctionMap[StatementType::READ_STMT] = &Convertor::extractReadStatement;
    statementFunctionMap[StatementType::CALL_STMT] = &Convertor::extractCallStatement;

    return statementFunctionMap;

}



