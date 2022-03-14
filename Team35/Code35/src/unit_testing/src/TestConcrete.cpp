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

    correct_statement_one.statementType = StatementType::kassign_stmt;
    correct_statement_one.stmtNo = 1;
    correct_statement_two.statementType = StatementType::kprint_stmt;
    correct_statement_two.stmtNo = 2;

    correct_stmtLst.SetContainerType(ContainerType::kprocedure);
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
    REQUIRE(stmtLst.getContainerType() == ContainerType::kprocedure);
    REQUIRE(stmtLst.getSize() == 2);

    Statement statement_one = stmtLst.getStmtAtIndex(0);
    REQUIRE(statement_one.stmtNo == 1);
    REQUIRE(statement_one.statementType == StatementType::kassign_stmt);

    Statement statement_two = stmtLst.getStmtAtIndex(1);
    REQUIRE(statement_two.stmtNo == 2);
    REQUIRE(statement_two.statementType == StatementType::kprint_stmt);
}