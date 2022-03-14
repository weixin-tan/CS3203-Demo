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
	static std::string currProcedureName;
	static std::vector<ParsedStatement> finalResults;

	Convertor(PkbSetter* pkb_setter);

	std::vector<std::vector<ParsedStatement>> procedureReader(ProcedureLst procedureList);

	std::vector<ParsedStatement> statementListReader(StmtLst statementList, int containerNumber);
	
	ParsedStatement readStatement(Statement stmt, ContainerType containerType,
		std::stack<int>& nestedStack, int containerNumber);

};