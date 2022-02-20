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

TEST_CASE("SP round 1 basic iteration test") {

	using std::filesystem::current_path;

	PKB pkb = PKB();
	Convertor convertor = Convertor(pkb.getSetter());
	Tokeniser tokeniser = Tokeniser();
	ConcreteSyntaxWithValidation concrete = ConcreteSyntaxWithValidation();

	std::ifstream t("iter1.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	ProcedureLst procedureLst;
	// Creating the queue of tokens
	std::queue<Token> tokenQueue = tokeniser.putInQueue(buffer.str());
	Procedure parsedProcedure = concrete.parseProcedure(tokenQueue);
	procedureLst.setNextProcedure(parsedProcedure);
	std::vector<std::vector<ParsedStatement>> resultsLst = convertor.ProcedureReader(procedureLst);

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
	ParsedStatement parsed_statement11 = results[10];


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
	REQUIRE(parsed_statement11.stmt_no == 11);


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
	REQUIRE(parsed_statement11.statement_type == StatementType::kprint_stmt);


	//"Checking if the used is correct"
	REQUIRE(parsed_statement2.var_used[0] == "x");
	REQUIRE(parsed_statement4.var_used[0] == "x");
	REQUIRE(parsed_statement6.var_used[0] == "z");
	REQUIRE(parsed_statement11.var_used[0] == "z");


	//"Checking if the procedure is correct")
	REQUIRE(parsed_statement1.procedure_name == "f");
	REQUIRE(parsed_statement2.procedure_name == "f");
	REQUIRE(parsed_statement3.procedure_name == "f");
	REQUIRE(parsed_statement5.procedure_name == "f");
	REQUIRE(parsed_statement4.procedure_name == "f");
	REQUIRE(parsed_statement6.procedure_name == "f");
	REQUIRE(parsed_statement7.procedure_name == "f");
	REQUIRE(parsed_statement8.procedure_name == "f");
	REQUIRE(parsed_statement9.procedure_name == "f");
	REQUIRE(parsed_statement10.procedure_name == "f");
	REQUIRE(parsed_statement11.procedure_name == "f");

	// Checking if the variables modified is correct.
	REQUIRE(parsed_statement1.var_modified[0] == "x");
	REQUIRE(parsed_statement2.var_modified[0] == "y");
	REQUIRE(parsed_statement3.var_modified[0] == "x");
	REQUIRE(parsed_statement5.var_modified[0] == "z");
	REQUIRE(parsed_statement7.var_modified[0] == "x");
	REQUIRE(parsed_statement8.var_modified[0] == "z");
	REQUIRE(parsed_statement9.var_modified[0] == "y");
	REQUIRE(parsed_statement10.var_modified[0] == "z");

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
	REQUIRE(parsed_statement11.preceding == 4);

	// CHecking if the constant is correct
	REQUIRE(parsed_statement1.constant[0] == "5");
	REQUIRE(parsed_statement3.constant[0] == "5");
	REQUIRE(parsed_statement4.constant[0] == "4");
	REQUIRE(parsed_statement6.constant[0] == "10");
	REQUIRE(parsed_statement7.constant[0] == "100");
	REQUIRE(parsed_statement8.constant[0] == "5");
	REQUIRE(parsed_statement9.constant[0] == "100");
	REQUIRE(parsed_statement10.constant[0] == "0");
}

TEST_CASE("SP Forbidden word") {

	using std::filesystem::current_path;

	PKB pkb = PKB();
	Convertor convertor = Convertor(pkb.getSetter());
	Tokeniser tokeniser = Tokeniser();
	ConcreteSyntaxWithValidation concrete = ConcreteSyntaxWithValidation();

	std::ifstream t("test.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	ProcedureLst procedureLst;
	// Creating the queue of tokens
	std::queue<Token> tokenQueue = tokeniser.putInQueue(buffer.str());
	Procedure parsedProcedure = concrete.parseProcedure(tokenQueue);
	procedureLst.setNextProcedure(parsedProcedure);
	std::vector<std::vector<ParsedStatement>> resultsLst = convertor.ProcedureReader(procedureLst);

	std::vector<ParsedStatement> results = resultsLst[0];

	//sorting the parsed statements as they are NOT inserted into order. 
	std::sort(results.begin(),
		results.end(),
		[](ParsedStatement& lhs, ParsedStatement& rhs)
		{
			return lhs.stmt_no < rhs.stmt_no;
		});

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
	
	REQUIRE(parsed_statement1.var_modified[0] == "a");
	REQUIRE(parsed_statement2.var_modified[0] == "a");
	REQUIRE(parsed_statement3.var_modified[0] == "if");
	REQUIRE(parsed_statement5.var_modified[0] == "if");
	REQUIRE(parsed_statement9.var_modified[0] == "read");


	REQUIRE(parsed_statement1.var_used[0] == "procedure");
	REQUIRE(parsed_statement2.var_used[0] == "if");
	REQUIRE(parsed_statement3.var_used[0] == "while");
	REQUIRE(parsed_statement4.var_used[0] == "while");
	REQUIRE(parsed_statement4.var_used[1] == "procedure");
	REQUIRE(parsed_statement5.var_used[0] == "then");
	REQUIRE(parsed_statement5.var_used[1] == "print");
	REQUIRE(parsed_statement5.var_used[2] == "call");
	REQUIRE(parsed_statement6.var_used[0] == "print");
	REQUIRE(parsed_statement7.var_used[0] == "x");
	REQUIRE(parsed_statement8.var_used[0] == "if");
	REQUIRE(parsed_statement8.var_used[1] == "read");
	REQUIRE(parsed_statement10.var_used[0] == "read");


	


}




