#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Statement.h"

#ifndef PARSED_STATEMENT_H
#define PARSED_STATEMENT_H

class ParsedStatement {
	
public:
	static int default_null_stmt_no;
	static Expr default_pattern;
	static std::string default_procedure_name;

	int stmt_no; // statement number 
	int if_stmt_no; //if statement number refers to the statement number of if
	int while_stmt_no; // while statement number refers to the statement number of while
	StatementType statement_type; // what kind of statement it is.
	Expr pattern = default_pattern; 
	std::string procedure_name;
	std::vector<std::string> var_used;
	std::vector<std::string> var_modified;
	std::string procedure_called;
	std::vector<std::string> constant;
	int preceding = -1; 

	ParsedStatement();

	ParsedStatement(int stmt_no, int if_stmt_no, int while_stmt_no,
		StatementType statement_type, Expr pattern, std::string procedure_name,
		std::vector<std::string> var_used, std::vector<std::string> var_modified, std::vector<std::string> constant,
		std::string procedure_called, int preceding);
};

#endif
