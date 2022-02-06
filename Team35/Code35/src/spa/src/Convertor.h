#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "ParsedStatement.h"
#include "Statement.h"
#include <stack>
#include <PKB/PkbSetter.h>

class Convertor {
	
public:

	PkbSetter* pkb_setter;
	static std::string curr_procedure;
	std::stack<int> nestedstack;

	Convertor::Convertor(PkbSetter* pkb_setter);

	void Convertor::ProcedureReader(std::vector<StatementContainer> procedurelist);

	std::vector<ParsedStatement> Convertor::StatementListReader(StatementContainer stmtcontainer, int container_number);

	ParsedStatement Convertor::readStatement(Statement stmt, ContainerType containertype,
		std::stack<int>& nestedstack, int container_number);

};