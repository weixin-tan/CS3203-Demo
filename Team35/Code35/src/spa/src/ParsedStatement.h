#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Statement.h"

#ifndef PARSED_STATEMENT_H
#define PARSED_STATEMENT_H

class ParsedStatement {
public: 
	int stmt_no; 
	int if_line_no;
	int while_line_no;
	StatementType statement_type;
	std::string pattern; // implementation not done
	std::string procedure_name;
	std::vector<std::string> var_used;
	std::vector<std::string> var_modified;
	std::string procedure_called;
	int preceding; 
};

#endif

