#include "Tokeniser.h"
#include "ConcreteSyntaxWithValidation.h"
#include "Program.h"
#include "catch.hpp"

TEST_CASE("Basic check 1") {
    Tokeniser tokeniser;
    std::queue<Token> tokenQueue = tokeniser.putInQueue("procedure main { x = y + 1; print z; }");

    ConcreteSyntaxWithValidation concrete;
    Program program = concrete.parseProgram(tokenQueue);

    // manually created Program object
    Program correct_program = Program();
    ProcedureLst correct_procedurelst = ProcedureLst();
    Procedure correct_procedure = Procedure();
    StmtLst correct_stmtLst = StmtLst();
    Statement correct_statement_one = Statement();
    Statement correct_statement_two = Statement();

    correct_statement_one.statementType = StatementType::ASSIGNMENT_STMT;
    correct_statement_one.stmtNo = 1;
    correct_statement_two.statementType = StatementType::PRINT_STMT;
    correct_statement_two.stmtNo = 2;

    correct_stmtLst.SetContainerType(ContainerType::PROCEDURE_CONTAINER);
    correct_stmtLst.setNextStmt(correct_statement_one);
    correct_stmtLst.setNextStmt(correct_statement_two);

    correct_procedure.setProcName("main");
    correct_procedure.setStmtLst(correct_stmtLst);
    correct_procedure.setSize(2);

    correct_procedurelst.setNextProcedure(correct_procedure);

    correct_program.setProcedureLst(correct_procedurelst);

    ProcedureLst procedureLst = program.getProcedureLst();
    REQUIRE(procedureLst.getSize() == 1);

    Procedure procedure = procedureLst.getProcedureAtIndex(0);
    REQUIRE(procedure.getProcName() == "main");
    REQUIRE(procedure.getSize() == 3);

    StmtLst stmtLst = procedure.getStmtLst();
    REQUIRE(stmtLst.getContainerType() == ContainerType::PROCEDURE_CONTAINER);
    REQUIRE(stmtLst.getSize() == 2);

    Statement statement_one = stmtLst.getStmtAtIndex(0);
    REQUIRE(statement_one.stmtNo == 1);
    REQUIRE(statement_one.statementType == StatementType::ASSIGNMENT_STMT);

    Statement statement_two = stmtLst.getStmtAtIndex(1);
    REQUIRE(statement_two.stmtNo == 2);
    REQUIRE(statement_two.statementType == StatementType::PRINT_STMT);
}

// SIMPLE SOURCE CODE
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


TEST_CASE("Basic check 2 - while, if etc. ") {
    Tokeniser tokeniser;
    std::queue<Token> tokenQueue = tokeniser.putInQueue("procedure f {x = 5; y = x; x = 5; while( x>4 ){ read z; if (z<10) then {x = 100; z = 5;} else {y = 100; z = 0; }}}");

    ConcreteSyntaxWithValidation concrete;
    Program program = concrete.parseProgram(tokenQueue);

    ProcedureLst procedureLst = program.getProcedureLst();
    REQUIRE(procedureLst.getSize() == 1);

    Procedure procedure = procedureLst.getProcedureAtIndex(0);
    REQUIRE(procedure.getProcName() == "f");
    REQUIRE(procedure.getSize() == 11); //dbl check this

    StmtLst stmtLst = procedure.getStmtLst();
    REQUIRE(stmtLst.getContainerType() == ContainerType::PROCEDURE_CONTAINER);
    REQUIRE(stmtLst.getSize() == 4);

    Statement statement1 = stmtLst.getStmtAtIndex(0);
    REQUIRE(statement1.stmtNo == 1);
    REQUIRE(statement1.statementType == StatementType::ASSIGNMENT_STMT);

    Statement statement2 = stmtLst.getStmtAtIndex(1);
    REQUIRE(statement2.stmtNo == 2);
    REQUIRE(statement2.statementType == StatementType::ASSIGNMENT_STMT);

    Statement statement3 = stmtLst.getStmtAtIndex(2);
    REQUIRE(statement3.stmtNo == 3);
    REQUIRE(statement3.statementType == StatementType::ASSIGNMENT_STMT);

    Statement statement4 = stmtLst.getStmtAtIndex(3);
    REQUIRE(statement4.stmtNo == 4);
    REQUIRE(statement4.statementType == StatementType::WHILE_STMT);

    StmtLst whileStmtLst;
    whileStmtLst = *statement4.whileStmtList.get();
    
    REQUIRE(whileStmtLst.getContainerType() == ContainerType::WHILE_CONTAINER);
    REQUIRE(whileStmtLst.getSize() == 2);

    Statement statement5 = whileStmtLst.getStmtAtIndex(0);
    REQUIRE(statement5.stmtNo == 5);
    REQUIRE(statement5.statementType == StatementType::READ_STMT);

    Statement statement6 = whileStmtLst.getStmtAtIndex(1);
    REQUIRE(statement6.stmtNo == 6);
    REQUIRE(statement6.statementType == StatementType::IF_STMT);

    StmtLst ifthenStmtLst;
    ifthenStmtLst = *statement6.ifthenStmtList.get();

    REQUIRE(ifthenStmtLst.getContainerType() == ContainerType::IF_THEN_CONTAINER);
    REQUIRE(ifthenStmtLst.getSize() == 2);

    Statement statement7 = ifthenStmtLst.getStmtAtIndex(0);
    REQUIRE(statement7.stmtNo == 7);
    REQUIRE(statement7.statementType == StatementType::ASSIGNMENT_STMT);

    Statement statement8 = ifthenStmtLst.getStmtAtIndex(1);
    REQUIRE(statement8.stmtNo == 8);
    REQUIRE(statement8.statementType == StatementType::ASSIGNMENT_STMT);

    StmtLst ifelseStmtLst;
    ifelseStmtLst = *statement6.ifelseStmtList.get();

    REQUIRE(ifelseStmtLst.getContainerType() == ContainerType::IF_ELSE_CONTAINER);
    REQUIRE(ifelseStmtLst.getSize() == 2);

    Statement statement9 = ifelseStmtLst.getStmtAtIndex(0);
    REQUIRE(statement9.stmtNo == 9);
    REQUIRE(statement9.statementType == StatementType::ASSIGNMENT_STMT);

    Statement statement10 = ifelseStmtLst.getStmtAtIndex(1);
    REQUIRE(statement10.stmtNo == 10);
    REQUIRE(statement10.statementType == StatementType::ASSIGNMENT_STMT);


}


TEST_CASE("Negative Test cases: Missing braces/curlies") {
    Tokeniser tokeniser;
    ConcreteSyntaxWithValidation concrete;

    std::queue<Token> tokenQueueMissingRightProc = tokeniser.putInQueue("procedure f {x = 5; y = x; x = 5; while( x>4 ){ read z; if (z<10) then {x = 100; z = 5;} else {y = 100; z = 0; }}");
    REQUIRE_THROWS_WITH(concrete.parseProgram(tokenQueueMissingRightProc), "Missing right curly.");

    std::queue<Token> tokenQueueMissingRightthen = tokeniser.putInQueue("procedure f {x = 5; y = x; x = 5; while( x>4 ){ read z; if (z<10) then {x = 100; z = 5; else {y = 100; z = 0; }}}");
    REQUIRE_THROWS_WITH(concrete.parseProgram(tokenQueueMissingRightthen), "Invalid statement declaration keyword.");

    std::queue<Token> tokenQueueMissingLeftCurlyIf = tokeniser.putInQueue("procedure f {x = 5; y = x; x = 5; while( x>4 ){read z; if (z<10) then x = 100; z = 5}; else {y = 100; z = 0; }}}");
    REQUIRE_THROWS_WITH(concrete.parseProgram(tokenQueueMissingLeftCurlyIf), "Missing left curly.");

    std::queue<Token> tokenQueueMissingLeftCurlyProc = tokeniser.putInQueue("procedure f x = 5; y = x; x = 5; while( x>4 ) read z; if (z<10) then {x = 100; z = 5}; else {y = 100; z = 0; }}}");
    REQUIRE_THROWS_WITH(concrete.parseProgram(tokenQueueMissingLeftCurlyProc), "Missing left curly for procedure.");

    std::queue<Token> tokenQueueMissingLeftCurlyWhile = tokeniser.putInQueue("procedure f {x = 5; y = x; x = 5; while( x>4 ) read z; if (z<10) then { x = 100; z = 5}; else {y = 100; z = 0; }}}");
    REQUIRE_THROWS_WITH(concrete.parseProgram(tokenQueueMissingLeftCurlyWhile), "Missing left curly.");

}


