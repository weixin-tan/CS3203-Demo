
#include "Statement.h"
#include "StmtLst.h"
#include <stdio.h>
#include "Convertor.h"
#include "ParsedStatement.h"

#include "TNode.h"
#include "catch.hpp"
using namespace std;
void require(bool b) {
    REQUIRE(b);
}

TEST_CASE("Convertor ") {

	//Creating the statement container
	StmtLst stmt_cont;

	//Creating the statement b = a
	Statement statement1; 
	statement1.stmt_no = 1;
	statement1.statement_type = kassign_stmt;
	statement1.expr = std::vector<std::string>{ "a" };
	statement1.proc_name = "procedure 1";
	statement1.var_name = std::vector<std::string>{ "b" };
	
	// Creating the statement a = a + 1
	Statement statement2;
	statement2.stmt_no = 2;
	statement2.statement_type = kassign_stmt;
	statement2.expr = std::vector<std::string>{ "a" };
	statement2.proc_name = "procedure 1";
	statement2.var_name = std::vector<std::string>{ "a" };

	//place statements into statementlist
	stmt_cont.setNextStmt(statement1);
	stmt_cont.setNextStmt(statement2);
	stmt_cont.SetContainerType(ContainerType::kprocedure);

	//Creating the parsedStatements for the solution

	// Creating the convertor
	Convertor convertor;
	std::vector<ParsedStatement> results = convertor.StatementListReader(stmt_cont, ContainerType::kprocedure);

	ParsedStatement parsed_statement1 = results[0];
	ParsedStatement parsed_statement2 = results[1];

	require(parsed_statement1.stmt_no == 1);
	
	SECTION("Checking if statement number is correct") {

		REQUIRE(parsed_statement1.stmt_no == 1);
		REQUIRE(parsed_statement2.stmt_no == 2);
	};

	SECTION("Checking if the assignment is correct") {
		REQUIRE(parsed_statement1.var_modified[0] == "b");
		REQUIRE(parsed_statement2.var_modified[0] == "a");
	}

	SECTION("Checking if the used is correct") {
		REQUIRE(parsed_statement1.var_used[0] == "a");
		REQUIRE(parsed_statement2.var_used[0] == "a");
	}
	/*
	SECTION("Checking if the procedure is correct") {
		REQUIRE(parsed_statement1.procedure_called == "procedure 1");
		REQUIRE(parsed_statement2.procedure_called == "procedure 1");
	}
	*/

	SECTION("Checking if the preceding is correct") {
		REQUIRE(parsed_statement1.preceding == -1);
		REQUIRE(parsed_statement2.preceding == 1);
	}

	SECTION("Checking if the statement_type is correct") {
		REQUIRE(parsed_statement1.statement_type == kassign_stmt);
		REQUIRE(parsed_statement2.statement_type == kassign_stmt);
	}

}


