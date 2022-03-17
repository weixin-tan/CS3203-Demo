#include "Convertor.h"

std::string Convertor::currProcedureName = "no_procedure";
std::vector<ParsedStatement> Convertor::finalResults;
Convertor::Convertor(PkbSetter* pkb_setter) {
    this->pkb_setter = pkb_setter;
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

// FOR TESTING, THE RETURN TYPE IS SUPPOSED TO BE VOID (TO SIMULATE PASSING TO PKB)
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
    case ContainerType::kifthen:
    case ContainerType::kifelse:
        current_statement.ifStmtNo = containerNum;
        break;
    case ContainerType::kwhile:
        current_statement.whileStmtNo = containerNum;
        break;
    case ContainerType::kprocedure:
        break;
    default:
        throw std::invalid_argument("no such container type");
        break;
    }

    //push the value into the stack
    nestedStack.push(stmt.stmtNo);

    //Check the statement types and extract required values
    switch (stmt.statementType) {
    case StatementType::kassign_stmt: 
        current_statement.varModified = stmt.varName;
        current_statement.varUsed = stmt.expr;
        current_statement.constant = stmt.constant;
        current_statement.pattern = stmt.expression;
        break;
        //PATTERN recogniser here
    case StatementType::kread_stmt:
        current_statement.varModified = stmt.varName;
        break;
    case StatementType::kprint_stmt:
        current_statement.varUsed = stmt.varName;
        break;
    case StatementType::kif_stmt:
        current_statement.varUsed = stmt.condExpr;
        current_statement.constant = stmt.constant;
        current_statement.pattern = stmt.expression;

        // In this case, if statement will have 2 statement lists (if and then) 
        this->statementListReader(*stmt.ifthenStmtList.get(), stmt.stmtNo);
        this->statementListReader(*stmt.ifelseStmtList.get(), stmt.stmtNo);
        break;
    
    case StatementType::kwhile_stmt:
        current_statement.varUsed = stmt.condExpr;
        current_statement.constant = stmt.constant;
        current_statement.pattern = stmt.expression;
        //In this case, this statement will a while statement list. 
        this->statementListReader(*stmt.whileStmtList.get(), stmt.stmtNo);
        break;

    case StatementType::kcall_stmt:
        current_statement.procedureCalled = stmt.procName;
        break;
    
        //Recursively read the inner statement stack. 	
    case StatementType::kprocedure_stmt:
        throw std::invalid_argument("procedure stmt type");
        break;

    case StatementType::knone:
        throw std::invalid_argument("none stmt type");
        break;
    default:
        throw std::invalid_argument("no such statement type");
        break;
    }

    return current_statement; 
}



