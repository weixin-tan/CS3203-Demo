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
    std::queue<Token> tokenQueue = tokeniser.putInQueue(buffer.str());
    Procedure parsedProcedure = concrete.parseProcedure(tokenQueue);
    procedureLst.setNextProcedure(parsedProcedure);
    std::vector<std::vector<ParsedStatement>> resultsLst = convertor.procedureReader(procedureLst);

    std::vector<ParsedStatement> result = resultsLst[0];

    ParsedStatement parsedStatement1 = result[0];
    ParsedStatement parsedStatement2 = result[1];
    ParsedStatement parsedStatement3 = result[2];

    //"Checking if statement number is correct"

    REQUIRE(parsedStatement1.stmtNo == 1);
    REQUIRE(parsedStatement2.stmtNo == 2);
    REQUIRE(parsedStatement3.stmtNo == 3);

    //"Checking if the assignment is correct"
    REQUIRE(parsedStatement1.varModified[0] == "xx");
    REQUIRE(parsedStatement2.varModified[0] == "whatever");
    REQUIRE(parsedStatement3.varModified[0] == "xx");


    //"Checking if the used is correct"
    REQUIRE(parsedStatement1.varUsed.size() == 0);
    REQUIRE(parsedStatement2.varUsed[0] == "xx");
    REQUIRE(parsedStatement3.varUsed[0] == "whatever");
    

    //"Checking if the preceding is correct"
    REQUIRE(parsedStatement1.preceding == -1);
    REQUIRE(parsedStatement2.preceding == 1);
    REQUIRE(parsedStatement3.preceding == 2);

    //"Checking if the statementType is correct"
    REQUIRE(parsedStatement1.statementType == StatementType::kassign_stmt);
    REQUIRE(parsedStatement2.statementType == StatementType::kassign_stmt);
    REQUIRE(parsedStatement3.statementType == StatementType::kassign_stmt);


    //"Checking if the procedure is correct") 
    /*
    REQUIRE(parsedStatement1.procedureCalled == "f");
    REQUIRE(parsedStatement2.procedureCalled == "f");
    REQUIRE(parsedStatement3.procedureCalled == "f");
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
    std::vector<std::vector<ParsedStatement>> resultsLst = convertor.procedureReader(procedureLst);

    std::vector<ParsedStatement> results = resultsLst[0];

    //sorting the parsed statements as they are NOT inserted into order. 
    std::sort(results.begin(),
        results.end(),
        [](ParsedStatement& lhs, ParsedStatement& rhs)
        {
            return lhs.stmtNo < rhs.stmtNo;
        });


    //Creating the parsedStatements for the solution

    ParsedStatement parsedStatement1 = results[0];
    ParsedStatement parsedStatement2 = results[1];
    ParsedStatement parsedStatement3 = results[2];
    ParsedStatement parsedStatement4 = results[3];
    ParsedStatement parsedStatement5 = results[4];
    ParsedStatement parsedStatement6 = results[5];
    ParsedStatement parsedStatement7 = results[6];
    ParsedStatement parsedStatement8 = results[7];
    ParsedStatement parsedStatement9 = results[8];
    ParsedStatement parsedStatement10 = results[9];
    ParsedStatement parsedStatement11 = results[10];


    //"Checking if statement number is correct")
    REQUIRE(parsedStatement1.stmtNo == 1);
    REQUIRE(parsedStatement2.stmtNo == 2);
    REQUIRE(parsedStatement3.stmtNo == 3);
    REQUIRE(parsedStatement4.stmtNo == 4);
    REQUIRE(parsedStatement5.stmtNo == 5);
    REQUIRE(parsedStatement6.stmtNo == 6);
    REQUIRE(parsedStatement7.stmtNo == 7);
    REQUIRE(parsedStatement8.stmtNo == 8);
    REQUIRE(parsedStatement9.stmtNo == 9);
    REQUIRE(parsedStatement10.stmtNo == 10);
    REQUIRE(parsedStatement11.stmtNo == 11);


    //Checking if the if statement is correct"
    REQUIRE(parsedStatement7.ifStmtNo == 6);
    REQUIRE(parsedStatement8.ifStmtNo == 6);
    REQUIRE(parsedStatement9.ifStmtNo == 6);
    REQUIRE(parsedStatement10.ifStmtNo == 6);

    //"Checking if the while statement is correct"
    REQUIRE(parsedStatement5.whileStmtNo == 4);
    REQUIRE(parsedStatement6.whileStmtNo == 4);


    //"Checking if statement type is correct"
    REQUIRE(parsedStatement1.statementType == StatementType::kassign_stmt);
    REQUIRE(parsedStatement2.statementType == StatementType::kassign_stmt);
    REQUIRE(parsedStatement3.statementType == StatementType::kassign_stmt);
    REQUIRE(parsedStatement4.statementType == StatementType::kwhile_stmt);
    REQUIRE(parsedStatement5.statementType == StatementType::kread_stmt);
    REQUIRE(parsedStatement6.statementType == StatementType::kif_stmt);
    REQUIRE(parsedStatement7.statementType == StatementType::kassign_stmt);
    REQUIRE(parsedStatement8.statementType == StatementType::kassign_stmt);
    REQUIRE(parsedStatement9.statementType == StatementType::kassign_stmt);
    REQUIRE(parsedStatement10.statementType == StatementType::kassign_stmt);
    REQUIRE(parsedStatement11.statementType == StatementType::kprint_stmt);


    //"Checking if the used is correct"
    REQUIRE(parsedStatement2.varUsed[0] == "x");
    REQUIRE(parsedStatement4.varUsed[0] == "x");
    REQUIRE(parsedStatement6.varUsed[0] == "z");
    REQUIRE(parsedStatement11.varUsed[0] == "z");


    //"Checking if the procedure is correct")
    REQUIRE(parsedStatement1.procedureName == "f");
    REQUIRE(parsedStatement2.procedureName == "f");
    REQUIRE(parsedStatement3.procedureName == "f");
    REQUIRE(parsedStatement5.procedureName == "f");
    REQUIRE(parsedStatement4.procedureName == "f");
    REQUIRE(parsedStatement6.procedureName == "f");
    REQUIRE(parsedStatement7.procedureName == "f");
    REQUIRE(parsedStatement8.procedureName == "f");
    REQUIRE(parsedStatement9.procedureName == "f");
    REQUIRE(parsedStatement10.procedureName == "f");
    REQUIRE(parsedStatement11.procedureName == "f");

    // Checking if the variables modified is correct.
    REQUIRE(parsedStatement1.varModified[0] == "x");
    REQUIRE(parsedStatement2.varModified[0] == "y");
    REQUIRE(parsedStatement3.varModified[0] == "x");
    REQUIRE(parsedStatement5.varModified[0] == "z");
    REQUIRE(parsedStatement7.varModified[0] == "x");
    REQUIRE(parsedStatement8.varModified[0] == "z");
    REQUIRE(parsedStatement9.varModified[0] == "y");
    REQUIRE(parsedStatement10.varModified[0] == "z");

    //"Checking if the preceding is correct") {
    REQUIRE(parsedStatement1.preceding == ParsedStatement::DEFAULT_NULL_STMT_NO);
    REQUIRE(parsedStatement2.preceding == 1);
    REQUIRE(parsedStatement3.preceding == 2);
    REQUIRE(parsedStatement4.preceding == 3);
    REQUIRE(parsedStatement5.preceding == ParsedStatement::DEFAULT_NULL_STMT_NO);
    REQUIRE(parsedStatement6.preceding == 5);
    REQUIRE(parsedStatement7.preceding == ParsedStatement::DEFAULT_NULL_STMT_NO);
    REQUIRE(parsedStatement8.preceding == 7);
    REQUIRE(parsedStatement9.preceding == ParsedStatement::DEFAULT_NULL_STMT_NO);
    REQUIRE(parsedStatement10.preceding == 9);
    REQUIRE(parsedStatement11.preceding == 4);

    // CHecking if the constant is correct
    REQUIRE(parsedStatement1.constant[0] == "5");
    REQUIRE(parsedStatement3.constant[0] == "5");
    REQUIRE(parsedStatement4.constant[0] == "4");
    REQUIRE(parsedStatement6.constant[0] == "10");
    REQUIRE(parsedStatement7.constant[0] == "100");
    REQUIRE(parsedStatement8.constant[0] == "5");
    REQUIRE(parsedStatement9.constant[0] == "100");
    REQUIRE(parsedStatement10.constant[0] == "0");
}



TEST_CASE("SP Forbidden word") {

    //procedure main{
    //a = procedure;
    //a = if;
    //if = while;
    //while (while < procedure) {
    //	if = (then + print) - 3 + call;
    //}
    //print print;
    //print x;
    //
    //if (if < read) then {
    //	read read;
    //}
    //else {
    //  print read;
    //}
    //}

    using std::filesystem::current_path;

    PKB pkb = PKB();
    Convertor convertor = Convertor(pkb.getSetter());
    Tokeniser tokeniser = Tokeniser();
    ConcreteSyntaxWithValidation concrete = ConcreteSyntaxWithValidation();

    std::ifstream t("iter1forbidden.txt");
    std::stringstream buffer;
    buffer << t.rdbuf();
    ProcedureLst procedureLst;
    // Creating the queue of tokens
    std::queue<Token> tokenQueue = tokeniser.putInQueue(buffer.str());
    Procedure parsedProcedure = concrete.parseProcedure(tokenQueue);
    procedureLst.setNextProcedure(parsedProcedure);
    std::vector<std::vector<ParsedStatement>> resultsLst = convertor.procedureReader(procedureLst);

    std::vector<ParsedStatement> results = resultsLst[0];

    //sorting the parsed statements as they are NOT inserted into order. 
    std::sort(results.begin(),
        results.end(),
        [](ParsedStatement& lhs, ParsedStatement& rhs)
        {
            return lhs.stmtNo < rhs.stmtNo;
        });

    ParsedStatement parsedStatement1 = results[0];
    ParsedStatement parsedStatement2 = results[1];
    ParsedStatement parsedStatement3 = results[2];
    ParsedStatement parsedStatement4 = results[3];
    ParsedStatement parsedStatement5 = results[4];
    ParsedStatement parsedStatement6 = results[5];
    ParsedStatement parsedStatement7 = results[6];
    ParsedStatement parsedStatement8 = results[7];
    ParsedStatement parsedStatement9 = results[8];
    ParsedStatement parsedStatement10 = results[9];
    
    REQUIRE(parsedStatement1.varModified[0] == "a");
    REQUIRE(parsedStatement2.varModified[0] == "a");
    REQUIRE(parsedStatement3.varModified[0] == "if");
    REQUIRE(parsedStatement5.varModified[0] == "if");
    REQUIRE(parsedStatement9.varModified[0] == "read");


    REQUIRE(parsedStatement1.varUsed[0] == "procedure");
    REQUIRE(parsedStatement2.varUsed[0] == "if");
    REQUIRE(parsedStatement3.varUsed[0] == "while");
    REQUIRE(parsedStatement4.varUsed[0] == "while");
    REQUIRE(parsedStatement4.varUsed[1] == "procedure");
    REQUIRE(parsedStatement5.varUsed[0] == "then");
    REQUIRE(parsedStatement5.varUsed[1] == "print");
    REQUIRE(parsedStatement5.varUsed[2] == "call");
    REQUIRE(parsedStatement6.varUsed[0] == "print");
    REQUIRE(parsedStatement7.varUsed[0] == "x");
    REQUIRE(parsedStatement8.varUsed[0] == "if");
    REQUIRE(parsedStatement8.varUsed[1] == "read");
    REQUIRE(parsedStatement10.varUsed[0] == "read");


    


}




