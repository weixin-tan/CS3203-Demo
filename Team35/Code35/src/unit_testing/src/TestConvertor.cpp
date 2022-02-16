#include "Statement.h"
#include "StmtLst.h"
#include <stdio.h>
#include "Convertor.h"
#include "ParsedStatement.h"

#include "PKB/PKB.h"

#include "TNode.h"
#include "catch.hpp"
using namespace std;


TEST_CASE("Case 1") {
	// PKB setter
	PKB pkb;

	

	StmtLst stmt_cont;
	//Creating the statement b = a
	Statement statement1;
	statement1.stmt_no = 1;
	statement1.statement_type = StatementType::kassign_stmt;
	statement1.expr = std::vector<std::string>{ "a" };
	statement1.proc_name = "procedure 1";
	statement1.var_name = std::vector<std::string>{ "b" };

	// Creating the statement a = a + 1
	Statement statement2;
	statement2.stmt_no = 2;
	statement2.statement_type = StatementType::kassign_stmt;
	statement2.expr = std::vector<std::string>{ "a" };
	statement2.proc_name = "procedure 1";
	statement2.var_name = std::vector<std::string>{ "a" };

	//place statements into statementlist
	stmt_cont.setNextStmt(statement1);
	stmt_cont.setNextStmt(statement2);
	stmt_cont.SetContainerType(ContainerType::kprocedure);

	//Creating the procedure
	Procedure procedure1;
	procedure1.setProcName("procedure 1");
	procedure1.setSize(2);
	procedure1.setStmtLst(stmt_cont);

	//Creating the procedure list
	ProcedureLst procedureLst;
	procedureLst.setNextProcedure(procedure1);

	//Creating the parsedStatements for the solution

	// Creating the convertor
	Convertor convertor(pkb.getSetter());
	std::vector<std::vector<ParsedStatement>> resultsLst = convertor.ProcedureReader(procedureLst);

	std::vector<ParsedStatement> results = resultsLst[0];

	ParsedStatement parsed_statement1 = results[0];
	ParsedStatement parsed_statement2 = results[1];

	//"Checking if statement number is correct"
	REQUIRE(parsed_statement1.stmt_no == 1);
	REQUIRE(parsed_statement2.stmt_no == 2);

	//"Checking if the assignment is correct")
	REQUIRE(parsed_statement1.var_modified[0] == "b");
	REQUIRE(parsed_statement2.var_modified[0] == "a");


	//"Checking if the used is correct"
	REQUIRE(parsed_statement1.var_used[0] == "a");
	REQUIRE(parsed_statement2.var_used[0] == "a");
	
	/*
	//"Checking if the procedure is correct"
	REQUIRE(parsed_statement1.procedure_called == "procedure 1");
	REQUIRE(parsed_statement2.procedure_called == "procedure 1");
	
	*/

	//"Checking if the preceding is correct") 
	REQUIRE(parsed_statement1.preceding == -1);
	REQUIRE(parsed_statement2.preceding == 1);
	

	//"Checking if the statement_type is correct") 
	REQUIRE(parsed_statement1.statement_type == StatementType::kassign_stmt);
	REQUIRE(parsed_statement2.statement_type == StatementType::kassign_stmt);
} 

//
	//procedure f {
//  x = 5;  // 1
//  y = x;  // 2
//  x = 5;  // 3
//  while (x > 4) {  // 4
//    read z;  // 5
//    if (z < 10) then {  // 6
//      x = 100;  // 7
//      z = 5;  // 8
//    } else {
//      y = 100;  // 9
//      z = 0;  // 10
//    }
//  }
//}

TEST_CASE("Case 2") {
	// PKB setter
	PKB pkb;

	StmtLst stmt_cont;
	stmt_cont.SetContainerType(ContainerType::kprocedure);

	//ifthenstmt list
	StmtLst ifThenStmtLst1;
	ifThenStmtLst1.SetContainerType(ContainerType::kifthen);

	ifThenStmtLst1.setNextStmt(Statement(7, StatementType::kassign_stmt, { "x" }, {}, {}, "f", { "100" }, nullptr, nullptr, nullptr));
	ifThenStmtLst1.setNextStmt(Statement(8, StatementType::kassign_stmt, { "z" }, {}, {}, "f", { "5" }, nullptr, nullptr, nullptr));

	//ifelsestmt list
	StmtLst ifElseStmtLst1;
	ifElseStmtLst1.SetContainerType(ContainerType::kifelse);

	ifElseStmtLst1.setNextStmt(Statement(9, StatementType::kassign_stmt, { "y" }, {}, {}, "f", { "100" }, nullptr, nullptr, nullptr));
	ifElseStmtLst1.setNextStmt(Statement(10, StatementType::kassign_stmt, { "z" }, {}, {}, "f", { "0" }, nullptr, nullptr, nullptr));

	//while statement list
	StmtLst whileStmtLst1;
	whileStmtLst1.SetContainerType(ContainerType::kwhile);

	whileStmtLst1.setNextStmt(Statement(5, StatementType::kread_stmt, { "x" }, {}, {}, "f", {}, nullptr, nullptr, nullptr));
	whileStmtLst1.setNextStmt(Statement(6, StatementType::kif_stmt, {}, {}, { "z" }, "f", { "10" }, &ifThenStmtLst1, &ifElseStmtLst1, nullptr));

	stmt_cont.setNextStmt(Statement(1, StatementType::kassign_stmt, { "x" }, { "5" }, {}, "f", { "5" }, nullptr, nullptr, nullptr));
	stmt_cont.setNextStmt(Statement(2, StatementType::kassign_stmt, { "y" }, { "x" }, {}, "f", {}, nullptr, nullptr, nullptr));
	stmt_cont.setNextStmt(Statement(3, StatementType::kassign_stmt, { "x" }, { "5" }, {}, "f", { "5" }, nullptr, nullptr, nullptr));
	stmt_cont.setNextStmt(Statement(4, StatementType::kwhile_stmt, {}, {}, { "x" }, "f", { "4" }, nullptr, nullptr, &whileStmtLst1));

	//Creating the procedure
	Procedure procedure2;
	procedure2.setProcName("procedure 2");
	procedure2.setSize(10);
	procedure2.setStmtLst(stmt_cont);

	//Creating the procedure list
	ProcedureLst procedureLst1;
	procedureLst1.setNextProcedure(procedure2);

	//Creating the parsedStatements for the solution

	// Creating the convertor
	Convertor convertor(pkb.getSetter());
	std::vector<std::vector<ParsedStatement>> resultsLst = convertor.ProcedureReader(procedureLst1);

	std::vector<ParsedStatement> results = resultsLst[0];

	//sorting the parsed statements as they are NOT inserted into order. 
	std::sort(results.begin(),
		results.end(),
		[](ParsedStatement& lhs, ParsedStatement& rhs)
		{
			return lhs.stmt_no < rhs.stmt_no;
		});

	//Creating the parsedStatements for the solution

	ParsedStatement parsed_statement1 = results[0];
	ParsedStatement parsed_statement2 = results[1];
	ParsedStatement parsed_statement3 = results[2];
	ParsedStatement parsed_statement4 = results[3];
	ParsedStatement parsed_statement5 = results[4];
	ParsedStatement parsed_statement6 = results[5];
	ParsedStatement parsed_statement7 = results[6];
	ParsedStatement parsed_statement8 = results[7];
	ParsedStatement parsed_statement9 = results[8];
	ParsedStatement parsed_statement10 = results[9];



	//"Checking if statement number is correct")
	REQUIRE(parsed_statement1.stmt_no == 1);
	REQUIRE(parsed_statement2.stmt_no == 2);
	REQUIRE(parsed_statement3.stmt_no == 3);
	REQUIRE(parsed_statement4.stmt_no == 4);
	REQUIRE(parsed_statement5.stmt_no == 5);
	REQUIRE(parsed_statement6.stmt_no == 6);
	REQUIRE(parsed_statement7.stmt_no == 7);
	REQUIRE(parsed_statement8.stmt_no == 8);
	REQUIRE(parsed_statement9.stmt_no == 9);
	REQUIRE(parsed_statement10.stmt_no == 10);

	//Checking if the if statement is correct"
	REQUIRE(parsed_statement7.if_stmt_no == 6);
	REQUIRE(parsed_statement8.if_stmt_no == 6);
	REQUIRE(parsed_statement9.if_stmt_no == 6);
	REQUIRE(parsed_statement10.if_stmt_no == 6);
	
	//"Checking if the while statement is correct"
	REQUIRE(parsed_statement5.while_stmt_no == 4);
	REQUIRE(parsed_statement6.while_stmt_no == 4);


	//"Checking if statement type is correct"
	REQUIRE(parsed_statement1.statement_type == StatementType::kassign_stmt);
	REQUIRE(parsed_statement2.statement_type == StatementType::kassign_stmt);
	REQUIRE(parsed_statement3.statement_type == StatementType::kassign_stmt);
	REQUIRE(parsed_statement4.statement_type == StatementType::kwhile_stmt);
	REQUIRE(parsed_statement5.statement_type == StatementType::kread_stmt);
	REQUIRE(parsed_statement6.statement_type == StatementType::kif_stmt);
	REQUIRE(parsed_statement7.statement_type == StatementType::kassign_stmt);
	REQUIRE(parsed_statement8.statement_type == StatementType::kassign_stmt);
	REQUIRE(parsed_statement9.statement_type == StatementType::kassign_stmt);
	REQUIRE(parsed_statement10.statement_type == StatementType::kassign_stmt);
	

	//"Checking if the used is correct"
	REQUIRE(parsed_statement2.var_used[0] == "x");
	REQUIRE(parsed_statement4.var_used[0] == "x");
	REQUIRE(parsed_statement6.var_used[0] == "z");


	/*
	SECTION("Checking if the procedure is correct") {
		REQUIRE(parsed_statement1.procedure_called == "procedure 1");
		REQUIRE(parsed_statement2.procedure_called == "procedure 1");
	}
	*/

	//"Checking if the preceding is correct") {
	REQUIRE(parsed_statement1.preceding == ParsedStatement::default_null_stmt_no);
	REQUIRE(parsed_statement2.preceding == 1);
	REQUIRE(parsed_statement3.preceding == 2);
	REQUIRE(parsed_statement4.preceding == 3);
	REQUIRE(parsed_statement5.preceding == ParsedStatement::default_null_stmt_no);
	REQUIRE(parsed_statement6.preceding == 5);
	REQUIRE(parsed_statement7.preceding == ParsedStatement::default_null_stmt_no);
	REQUIRE(parsed_statement8.preceding == 7);
	REQUIRE(parsed_statement9.preceding == ParsedStatement::default_null_stmt_no);
	REQUIRE(parsed_statement10.preceding == 9);
	

}

