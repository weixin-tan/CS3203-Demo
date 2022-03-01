#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "ParsedStatement.h"
#include "Statement.h"
#include <stack>
#include "StmtLst.h"
#include "Procedure.h"
#include "ProcedureLst.h"
#include <PKB/PkbSetter.h>
#include <stdexcept>

class Convertor {


public:

	PkbSetter* pkb_setter;
	static std::string curr_procedure_name;
	static std::vector<ParsedStatement> finalResults;

	Convertor(PkbSetter* pkb_setter);

	std::vector<std::vector<ParsedStatement>> ProcedureReader(ProcedureLst procedurelist);

	std::vector<ParsedStatement> StatementListReader(StmtLst statement_list, int container_number);
	
	ParsedStatement readStatement(Statement stmt, ContainerType containertype,
		std::stack<int>& nestedstack, int container_number);

};