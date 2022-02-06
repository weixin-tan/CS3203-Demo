#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <vector> 
#include "StatementType.h"
#include "StmtLst.h"


class StmtLst;

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
	StmtLst* ifthen_stmt_list;
	StmtLst* ifelse_stmt_list;
	StmtLst* while_stmt_list;

};






#endif

