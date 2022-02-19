#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <vector>
#include <memory>
#include "StatementType.h"
#include "StmtLst.h"

class StmtLst;

class Statement {
public:
	Statement();

	Statement(int stmt_no, StatementType statement_type,
		std::vector<std::string> var_name, std::vector<std::string> expr,
		std::vector<std::string> cond_expr, std::string proc_name,
		std::vector<std::string> constant, std::shared_ptr<StmtLst> ifthen_stmt_list,
		std::shared_ptr<StmtLst> ifelse_stmt_list, std::shared_ptr<StmtLst> while_stmt_list);

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

	//for Constant variables
	std::vector<std::string> constant; 

	// for then, else, while statements. 
	std::shared_ptr<StmtLst> ifthen_stmt_list;
	std::shared_ptr<StmtLst> ifelse_stmt_list;
	std::shared_ptr<StmtLst> while_stmt_list;

};
#endif
