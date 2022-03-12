#include <string>
#include "ParsedStatement.h"

//Defining static const variables here

int ParsedStatement::default_null_stmt_no = -1;
Expr ParsedStatement::default_pattern;
std::string ParsedStatement::default_procedure_name = "";

ParsedStatement::ParsedStatement() {
	this->stmt_no = default_null_stmt_no; // statement number 
	this->if_stmt_no = default_null_stmt_no; //if statement number refers to the statement number of if
	this->while_stmt_no = default_null_stmt_no; // while statement number refers to the statement number of while
	this->statement_type = StatementType::knone; // what kind of statement it is.
	this->pattern = default_pattern; 
	this->procedure_name = default_procedure_name;
	this->var_used;
	this->var_modified;
	this->procedure_called = default_procedure_name;
	this->preceding = default_null_stmt_no;
	this->constant; 
}

ParsedStatement::ParsedStatement(int stmt_no, int if_stmt_no, int while_stmt_no,
	StatementType statement_type, Expr pattern, std::string procedure_name,
	std::vector<std::string> var_used, std::vector<std::string> var_modified, std::vector<std::string> constant,
	std::string procedure_called, int preceding) {

	this->stmt_no = stmt_no;
	this->if_stmt_no = if_stmt_no; //if statement number refers to the statement number of if
	this->while_stmt_no = while_stmt_no; // while statement number refers to the statement number of while
	this->statement_type = statement_type; // what kind of statement it is.
	this->pattern = pattern; // implementation not done
	this->procedure_name = procedure_name;
	this->var_used = var_used;
	this->var_modified = var_modified;
	this->constant = constant;
	this->procedure_called = procedure_called;
	this->preceding = preceding;

}