#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "ParsedStatement.h"
#include "Statement.h"
#include <stack>



class Convertor {


public:
	Convertor();

	static std::string curr_procedure;
	std::stack<int> nestedstack;

	void Convertor::ProcedureReader(std::vector<StatementContainer> procedurelist);

	std::vector<ParsedStatement> Convertor::StatementListReader(StatementContainer stmtcontainer, int container_number);

	ParsedStatement Convertor::readStatement(Statement stmt, ContainerType containertype,
		std::stack<int>& nestedstack, int container_number);

};