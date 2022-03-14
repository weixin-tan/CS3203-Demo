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

    StmtLst stmtCont;
    //Creating the statement b = a
    Statement statement1;
    statement1.stmtNo = 1;
    statement1.statementType = StatementType::kassign_stmt;
    statement1.expr = std::vector<std::string>{ "a" };
    statement1.procName = "procedure 1";
    statement1.varName = std::vector<std::string>{ "b" };

    // Creating the statement a = a + 1
    Statement statement2;
    statement2.stmtNo = 2;
    statement2.statementType = StatementType::kassign_stmt;
    statement2.expr = std::vector<std::string>{ "a" };
    statement2.procName = "procedure 1";
    statement2.varName = std::vector<std::string>{ "a" };

    //place statements into statementlist
    stmtCont.setNextStmt(statement1);
    stmtCont.setNextStmt(statement2);
    stmtCont.SetContainerType(ContainerType::kprocedure);

    //Creating the procedure
    Procedure procedure1;
    procedure1.setProcName("procedure 1");
    procedure1.setSize(2);
    procedure1.setStmtLst(stmtCont);

    //Creating the procedure list
    ProcedureLst procedureLst;
    procedureLst.setNextProcedure(procedure1);

    //Creating the parsedStatements for the solution

    // Creating the convertor
    Convertor convertor(pkb.getSetter());
    std::vector<std::vector<ParsedStatement>> resultsLst = convertor.procedureReader(procedureLst);

    std::vector<ParsedStatement> results = resultsLst[0];

    ParsedStatement parsedStatement1 = results[0];
    ParsedStatement parsedStatement2 = results[1];

    //"Checking if statement number is correct"
    REQUIRE(parsedStatement1.stmtNo == 1);
    REQUIRE(parsedStatement2.stmtNo == 2);

    //"Checking if the assignment is correct")
    REQUIRE(parsedStatement1.varModified[0] == "b");
    REQUIRE(parsedStatement2.varModified[0] == "a");


    //"Checking if the used is correct"
    REQUIRE(parsedStatement1.varUsed[0] == "a");
    REQUIRE(parsedStatement2.varUsed[0] == "a");
    
    /*
    //"Checking if the procedure is correct"
    REQUIRE(parsedStatement1.procedureCalled == "procedure 1");
    REQUIRE(parsedStatement2.procedureCalled == "procedure 1");
    
    */

    //"Checking if the preceding is correct") 
    REQUIRE(parsedStatement1.preceding == -1);
    REQUIRE(parsedStatement2.preceding == 1);
    

    //"Checking if the statementType is correct") 
    REQUIRE(parsedStatement1.statementType == StatementType::kassign_stmt);
    REQUIRE(parsedStatement2.statementType == StatementType::kassign_stmt);
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

    StmtLst stmtCont;
    stmtCont.SetContainerType(ContainerType::kprocedure);

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

    whileStmtLst1.setNextStmt(Statement(5, StatementType::kread_stmt, { "z" }, {}, {}, "f", {}, nullptr, nullptr, nullptr));
    std::shared_ptr<StmtLst> SP_ifThenStmtLst1 = std::make_shared<StmtLst>(ifThenStmtLst1);
    std::shared_ptr<StmtLst> SP_ifElseStmtLst1 = std::make_shared<StmtLst>(ifElseStmtLst1);
    
    whileStmtLst1.setNextStmt(Statement(6, StatementType::kif_stmt, {}, {}, { "z" }, "f", { "10" }, SP_ifThenStmtLst1, SP_ifElseStmtLst1, nullptr));

    std::shared_ptr<StmtLst> SP_whileStmtLst1 = std::make_shared<StmtLst>(whileStmtLst1);


    stmtCont.setNextStmt(Statement(1, StatementType::kassign_stmt, { "x" }, { "5" }, {}, "f", { "5" }, nullptr, nullptr, nullptr));
    stmtCont.setNextStmt(Statement(2, StatementType::kassign_stmt, { "y" }, { "x" }, {}, "f", {}, nullptr, nullptr, nullptr));
    stmtCont.setNextStmt(Statement(3, StatementType::kassign_stmt, { "x" }, { "5" }, {}, "f", { "5" }, nullptr, nullptr, nullptr));
    stmtCont.setNextStmt(Statement(4, StatementType::kwhile_stmt, {}, {}, { "x" }, "f", { "4" }, nullptr, nullptr, SP_whileStmtLst1));

    //Creating the procedure
    Procedure procedure2;
    procedure2.setProcName("f");
    procedure2.setSize(10);
    procedure2.setStmtLst(stmtCont);

    //Creating the procedure list
    ProcedureLst procedureLst1;
    procedureLst1.setNextProcedure(procedure2);

    //Creating the parsedStatements for the solution

    // Creating the convertor
    Convertor convertor(pkb.getSetter());
    std::vector<std::vector<ParsedStatement>> resultsLst = convertor.procedureReader(procedureLst1);

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
    

    //"Checking if the used is correct"
    REQUIRE(parsedStatement2.varUsed[0] == "x");
    REQUIRE(parsedStatement4.varUsed[0] == "x");
    REQUIRE(parsedStatement6.varUsed[0] == "z");


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

//Short stubs for incorrect error handling
TEST_CASE("Incorrect convertor statement type 1") {
    // PKB setter
    PKB pkb;

    StmtLst stmtCont;
    stmtCont.SetContainerType(ContainerType::kprocedure);

    stmtCont.setNextStmt(Statement(1, StatementType::kassign_stmt, { "x" }, { "5" }, {}, "f", { "5" }, nullptr, nullptr, nullptr));
    stmtCont.setNextStmt(Statement(2, StatementType::kassign_stmt, { "y" }, { "x" }, {}, "f", {}, nullptr, nullptr, nullptr));
    stmtCont.setNextStmt(Statement(3, StatementType::kprocedure_stmt, { "x" }, { "5" }, {}, "f", { "5" }, nullptr, nullptr, nullptr));

    //Creating the procedure
    Procedure procedure3;
    procedure3.setProcName("f");
    procedure3.setSize(3);
    procedure3.setStmtLst(stmtCont);

    //Creating the procedure list
    ProcedureLst procedureLst2;
    procedureLst2.setNextProcedure(procedure3);

    // Creating the convertor
    Convertor convertor(pkb.getSetter());

    //Checking if the procedure error is thrown. 
    REQUIRE_THROWS_WITH(convertor.procedureReader(procedureLst2), "procedure stmt type");
    
}

TEST_CASE("Incorrect convertor statement type 2") {
    // PKB setter
    PKB pkb;

    StmtLst stmtCont;
    stmtCont.SetContainerType(ContainerType::kprocedure);

    stmtCont.setNextStmt(Statement(1, StatementType::kassign_stmt, { "x" }, { "5" }, {}, "f", { "5" }, nullptr, nullptr, nullptr));
    stmtCont.setNextStmt(Statement(2, StatementType::kassign_stmt, { "y" }, { "x" }, {}, "f", {}, nullptr, nullptr, nullptr));
    stmtCont.setNextStmt(Statement(3, StatementType::knone, { "x" }, { "5" }, {}, "f", { "5" }, nullptr, nullptr, nullptr));

    //Creating the procedure
    Procedure procedure3;
    procedure3.setProcName("f");
    procedure3.setSize(3);
    procedure3.setStmtLst(stmtCont);

    //Creating the procedure list
    ProcedureLst procedureLst2;
    procedureLst2.setNextProcedure(procedure3);

    // Creating the convertor
    Convertor convertor(pkb.getSetter());

    //Checking if the procedure error is thrown. 
    REQUIRE_THROWS_WITH(convertor.procedureReader(procedureLst2), "none stmt type");

}

TEST_CASE("Incorrect convertor statement type 3 - in nested container") {
    // PKB setter
    PKB pkb;

    StmtLst stmtCont;
    stmtCont.SetContainerType(ContainerType::kprocedure);

    //while statement list
    StmtLst whileStmtLst1;
    whileStmtLst1.SetContainerType(ContainerType::kwhile);
    whileStmtLst1.setNextStmt(Statement(4, StatementType::knone, { "x" }, { "5" }, {}, "f", { "5" }, nullptr, nullptr, nullptr));

    std::shared_ptr<StmtLst> SP_whileStmtLst2 = std::make_shared<StmtLst>(whileStmtLst1);

    stmtCont.setNextStmt(Statement(1, StatementType::kassign_stmt, { "x" }, { "5" }, {}, "f", { "5" }, nullptr, nullptr, nullptr));
    stmtCont.setNextStmt(Statement(2, StatementType::kassign_stmt, { "y" }, { "x" }, {}, "f", {}, nullptr, nullptr, nullptr));
    stmtCont.setNextStmt(Statement(3, StatementType::kwhile_stmt, {}, {}, { "x" }, "f", { "4" }, nullptr, nullptr, SP_whileStmtLst2));


    //Creating the procedure
    Procedure procedure3;
    procedure3.setProcName("f");
    procedure3.setSize(3);
    procedure3.setStmtLst(stmtCont);

    //Creating the procedure list
    ProcedureLst procedureLst2;
    procedureLst2.setNextProcedure(procedure3);

    // Creating the convertor
    Convertor convertor(pkb.getSetter());

    //Checking if the procedure error is thrown. 
    REQUIRE_THROWS_WITH(convertor.procedureReader(procedureLst2), "none stmt type");

}


