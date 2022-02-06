#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "ParsedStatement.h"
#include "Statement.h"
#include <stack>
#include "StmtLst.h"
#include "Procedure.h"

class Convertor {


public:

	static std::string curr_procedure;
	std::stack<int> nestedstack;

	void Convertor::ProcedureReader(std::vector<Procedure> procedurelist);

	std::vector<ParsedStatement> Convertor::StatementListReader(StmtLst statement_list, int container_number);
	
	ParsedStatement Convertor::readStatement(Statement stmt, ContainerType containertype,
		std::stack<int>& nestedstack, int container_number);

};