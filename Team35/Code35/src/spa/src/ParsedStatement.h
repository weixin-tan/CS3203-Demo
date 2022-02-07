#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Statement.h"

#ifndef PARSED_STATEMENT_H
#define PARSED_STATEMENT_H

class ParsedStatement {
public: 
	int stmt_no; // statement number 
	int if_line_no; //if statement number refers to the statement number of if
	int while_line_no; // while statement number refers to the statement number of while
	StatementType statement_type; // what kind of statement it is.
	std::string pattern; // implementation not done
	std::string procedure_name; 
	std::vector<std::string> var_used;
	std::vector<std::string> var_modified;
	std::string procedure_called;
	int preceding; 
};

#endif
