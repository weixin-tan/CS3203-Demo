#ifndef SPA_CONVERTOR_H
#define SPA_CONVERTOR_H
#include <iostream>
#include <stack>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>
#include "Statement.h"
#include "StatementType.h"
#include "StmtLst.h"
#include "ParsedStatement.h"
#include "Procedure.h"
#include "ProcedureLst.h"
#include <../PKB/PkbSetter.h>

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
#endif