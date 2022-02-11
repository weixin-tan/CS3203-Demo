#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Convertor.h"
#include "Statement.h"
#include "StatementType.h"
#include <PKB/PkbSetter.h>
#include "Procedure.h"

std::string Convertor::curr_procedure = "no_procedure";

Convertor::Convertor(PkbSetter* pkb_setter) {
	this->pkb_setter = pkb_setter;
}

// Reads the procedurelist and calls a statemnet list reader for each procedure in the list.
void Convertor::ProcedureReader(std::vector<Procedure> procedurelist) {
	for (int i = 0; i < procedurelist.size(); i++) {
		curr_procedure = procedurelist[i].getProcName();
		StatementListReader(procedurelist[i].getStmtLst(), -1);
		
	}
}

// FOR TESTING, THE RETURN TYPE IS SUPPOSED TO BE VOID (TO SIMULATE PASSING TO PKB)
// Identifes the type of statementlist, and also includes the line number of the container. 
// For Procedures, since there are no line numbers, line number will be -1. 
// Creates a stack for the line numbers of the statement container..
std::vector<ParsedStatement> Convertor::StatementListReader(StmtLst statement_list, int container_number) {
	// Gets the container type. If it is a procedure, then the current procedure(static) will
	// be that procedure. 
	ContainerType containertype = statement_list.GetContainerType();

	//creating new stack and pushing in -1 line number. 
	nestedstack.push(-1);

	// FOR TESTING(TEMPORARY)
	std::vector<ParsedStatement> results; 


// Reading every statement in the container. 
	for (int i = 0; i < statement_list.getSize(); i++) {
		results.push_back(this->readStatement(statement_list.getStmtAtIndex(i), containertype, nestedstack, container_number));
	}

	// TODO: Sending the statement list to the PKB - should be removed 
	for (const ParsedStatement i : results) {
		pkb_setter->insertStmt(i);
	}

	return results;
}

// Reads and converts the statement into a parsedStatement object.
// stmt parameter refers to statement
// ContainerType refers to the type of statementlist. 
// nestedstack refers to the stack of line numbers for that particular container. 
// container_num refers to the statement number of the container. 

// returns a parsed statement. Note that due to recursive return calls, the order of the returned statements
// would not follow the same format as the SIMPLE source code. Hence, a vector could be used to store and sort
// based on statement number. 
ParsedStatement Convertor::readStatement(Statement stmt, ContainerType containertype,
	std::stack<int> &nestedstack, int container_num) {

	ParsedStatement current_statement; 
	current_statement.stmt_no = stmt.stmt_no;
	current_statement.procedure_name = curr_procedure;
	current_statement.preceding = nestedstack.top();
	current_statement.statement_type = stmt.statement_type;

	//populate container type (parent)
	switch (containertype) {
	case ContainerType::kifthen:
	case ContainerType::kifelse:
		current_statement.if_stmt_no = container_num;
		break;
	case ContainerType::kwhile:
		current_statement.while_stmt_no = container_num;
		break;
	}

	//push the value into the stack
	nestedstack.push(stmt.stmt_no);

	//Check the statement types and extract required values
	switch (stmt.statement_type) {
	case StatementType::kassign_stmt: 
		current_statement.var_modified = stmt.var_name;
		current_statement.var_used = stmt.expr;
		break;
		//Pattern recogniser here
	case StatementType::kread_stmt:
		current_statement.var_modified = stmt.var_name;
		break;
	case StatementType::kprint_stmt:
		current_statement.var_used = stmt.expr;
		break;
	case StatementType::kif_stmt:
		current_statement.var_used = stmt.cond_expr;

		// In this case, if statement will have 2 statement lists (if and then) 
		this->StatementListReader(*stmt.ifthen_stmt_list, stmt.stmt_no);
		this->StatementListReader(*stmt.ifelse_stmt_list, stmt.stmt_no);
		break;
	
	case StatementType::kwhile_stmt:
		current_statement.var_used = stmt.cond_expr;
		//In this case, this statement will a while statement list. 
		this->StatementListReader(*stmt.while_stmt_list, stmt.stmt_no);
		break;

	case StatementType::kcall_stmt:
		current_statement.procedure_called = stmt.proc_name;
		break;
	
		//Recursively read the inner statement stack. 	
	}

	return current_statement; 
}



