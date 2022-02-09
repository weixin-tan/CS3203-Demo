#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "ParsedStatement.h"
#include "Statement.h"
#include <stack>
#include "StmtLst.h"
#include "Procedure.h"

#include <PKB/PkbSetter.h>

class Convertor {


public:

	PkbSetter* pkb_setter;
	static std::string curr_procedure;
	std::stack<int> nestedstack;

	Convertor::Convertor(PkbSetter* pkb_setter);

	void Convertor::ProcedureReader(std::vector<Procedure> procedurelist);

	std::vector<ParsedStatement> Convertor::StatementListReader(StmtLst statement_list, int container_number);
	
	ParsedStatement Convertor::readStatement(Statement stmt, ContainerType containertype,
		std::stack<int>& nestedstack, int container_number);

};