#include "Statement.h"

Statement::Statement() {

}

Statement::Statement(int stmt_no, StatementType statement_type,
	std::vector<std::string> var_name, std::vector<std::string> expr,
	std::vector<std::string> cond_expr, std::string proc_name,
	std::vector<std::string> constant, StmtLst ifthen_stmt_list,
	StmtLst ifelse_stmt_list, StmtLst while_stmt_list) {
	
	this->stmt_no = stmt_no;
	this->statement_type = statement_type;
	this->var_name = var_name;
	this->expr = expr;
	this->cond_expr = cond_expr;
	this->proc_name = proc_name;
	this->constant = constant;
	this->ifthen_stmt_list = ifthen_stmt_list;
	this->ifelse_stmt_list = ifelse_stmt_list;
	this->while_stmt_list = while_stmt_list;

}