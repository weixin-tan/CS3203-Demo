#include<stdio.h>
#include <iostream>
#include <string>
#include <vector> 
#include "StatementType.h"
#include "StatementContainer.h"

#ifndef STATEMENT_H
#define STATEMENT_H

class StatementContainer;

class Statement {
public:
	Statement();

	int stmt_no;

	StatementType statement_type;
	
	// For read, print and assignment statements
	std::vector<std::string> var_name;
	
	// For assignment statements
	std::vector<std::string> expr;

	// for while, if statements
	std::vector<std::string> cond_expr;

	// for call statements
	std::string proc_name;

	// for then, else, while statements. 
	StatementContainer *ifthen_stmt_list;
	StatementContainer *ifelse_stmt_list;
	StatementContainer *while_stmt_list;

};






#endif

