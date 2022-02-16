#include "catch.hpp"
#include "PKB/PKB.h"
#include "SP.h"

#include <iostream>
#include <string>
#include <filesystem>


TEST_CASE("SP round 0 iteration test") {

	using std::filesystem::current_path;

	PKB pkb = PKB();
	Convertor convertor = Convertor(pkb.getSetter());
	Tokeniser tokeniser = Tokeniser();
	ConcreteSyntaxWithValidation concrete = ConcreteSyntaxWithValidation();

	std::ifstream t("iter0.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	ProcedureLst procedureLst;
	procedureLst.setNextProcedure(concrete.parseProcedure(tokeniser.putInQueue(buffer.str())));
	std::vector<std::vector<ParsedStatement>> results = convertor.ProcedureReader(procedureLst);

	std::vector<ParsedStatement> result = results[0];

	ParsedStatement parsed_statement1 = result[0];
	ParsedStatement parsed_statement2 = result[1];
	ParsedStatement parsed_statement3 = result[2];

	//"Checking if statement number is correct"

	REQUIRE(parsed_statement1.stmt_no == 1);
	REQUIRE(parsed_statement2.stmt_no == 2);
	REQUIRE(parsed_statement3.stmt_no == 3);

	//"Checking if the assignment is correct"
	REQUIRE(parsed_statement1.var_modified[0] == "xx");
	REQUIRE(parsed_statement2.var_modified[0] == "whatever");
	REQUIRE(parsed_statement3.var_modified[0] == "xx");


	//"Checking if the used is correct"
	REQUIRE(parsed_statement1.var_used.size() == 0);
	REQUIRE(parsed_statement2.var_used[0] == "xx");
	REQUIRE(parsed_statement3.var_used[0] == "whatever");
	

	//"Checking if the preceding is correct"
	REQUIRE(parsed_statement1.preceding == -1);
	REQUIRE(parsed_statement2.preceding == 1);
	REQUIRE(parsed_statement3.preceding == 2);

	//"Checking if the statement_type is correct"
	REQUIRE(parsed_statement1.statement_type == StatementType::kassign_stmt);
	REQUIRE(parsed_statement2.statement_type == StatementType::kassign_stmt);
	REQUIRE(parsed_statement3.statement_type == StatementType::kassign_stmt);


	//"Checking if the procedure is correct") 
	/*
	REQUIRE(parsed_statement1.procedure_called == "f");
	REQUIRE(parsed_statement2.procedure_called == "f");
	REQUIRE(parsed_statement3.procedure_called == "f");
	*/
}