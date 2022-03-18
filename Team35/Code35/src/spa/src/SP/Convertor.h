#ifndef SPA_CONVERTOR_H
#define SPA_CONVERTOR_H
#include <iostream>
#include <stack>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <functional>
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

	std::map<StatementType, ParsedStatement(Convertor::*)(Statement, ParsedStatement*)> statementFunctionMap;

	Convertor(PkbSetter* pkb_setter);

	std::vector<std::vector<ParsedStatement>> procedureReader(ProcedureLst procedureList);

	std::vector<ParsedStatement> statementListReader(StmtLst statementList, int containerNumber);
	
	ParsedStatement readStatement(Statement stmt, ContainerType containerType,
		std::stack<int>& nestedStack, int containerNumber);


	std::map<StatementType, ParsedStatement(Convertor::*)(Statement, ParsedStatement*)> initialiseMap();

	ParsedStatement extractAssignmentStatement(Statement stmt, ParsedStatement *current_statement);
	ParsedStatement extractCallStatement(Statement current_statement, ParsedStatement *stmt);
	ParsedStatement extractWhileStatement(Statement current_statement, ParsedStatement *stmt);
	ParsedStatement extractIfStatement(Statement current_statement, ParsedStatement *stmt);
	ParsedStatement extractPrintStatement(Statement current_statement, ParsedStatement *stmt);
	ParsedStatement extractReadStatement(Statement current_statement, ParsedStatement *stmt);




};
#endif