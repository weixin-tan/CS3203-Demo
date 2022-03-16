#include "catch.hpp"
#include "PKB.h"
#include "QueryProcessor.h"
#include "QPSMainLogic.h"
#include "EntityToElementConverter.h"

PKB pkb = PKB();
QPSMainLogic* qr = QPSMainLogic::getInstance(pkb.getGetter());
QueryProcessor qp = QueryProcessor();
auto qh = QPSHandler(pkb.getGetter());
ResultProcessor rp = ResultProcessor();

//TODO: Commented out test cases are those that failed since Expression Processor is pushed. DO remember to uncomment them!
//      once they are resolved

//SETUP:
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
//  print x; // 11
//}

std::vector<ParsedStatement> pStatements = {
  ParsedStatement(1, ParsedStatement::DEFAULT_NULL_STMT_NO, ParsedStatement::DEFAULT_NULL_STMT_NO, StatementType::kassign_stmt, ParsedStatement::defaultPattern, "f", {}, {"x"}, {"5"},  ParsedStatement::DEFAULT_PROCEDURE_NAME, ParsedStatement::DEFAULT_NULL_STMT_NO),
  ParsedStatement(2, ParsedStatement::DEFAULT_NULL_STMT_NO, ParsedStatement::DEFAULT_NULL_STMT_NO, StatementType::kassign_stmt, ParsedStatement::defaultPattern, "f", {"x"}, {"y"}, {}, ParsedStatement::DEFAULT_PROCEDURE_NAME, 1),
  ParsedStatement(3, ParsedStatement::DEFAULT_NULL_STMT_NO, ParsedStatement::DEFAULT_NULL_STMT_NO, StatementType::kassign_stmt, ParsedStatement::defaultPattern, "f", {}, {"x"}, {"5"}, ParsedStatement::DEFAULT_PROCEDURE_NAME, 2),
  ParsedStatement(4, ParsedStatement::DEFAULT_NULL_STMT_NO, ParsedStatement::DEFAULT_NULL_STMT_NO, StatementType::kwhile_stmt, ParsedStatement::defaultPattern, "f", {"x"}, {}, {"4"}, ParsedStatement::DEFAULT_PROCEDURE_NAME, 3),
  ParsedStatement(5, ParsedStatement::DEFAULT_NULL_STMT_NO, 4, StatementType::kread_stmt, ParsedStatement::defaultPattern, "f", {}, {"z"}, {},ParsedStatement::DEFAULT_PROCEDURE_NAME, ParsedStatement::DEFAULT_NULL_STMT_NO),
  ParsedStatement(6, ParsedStatement::DEFAULT_NULL_STMT_NO, 4, StatementType::kif_stmt, ParsedStatement::defaultPattern, "f", {"z"}, {}, {"10"}, ParsedStatement::DEFAULT_PROCEDURE_NAME, 5),
  ParsedStatement(7, 6, ParsedStatement::DEFAULT_NULL_STMT_NO, StatementType::kassign_stmt, ParsedStatement::defaultPattern, "f", {}, {"x"}, {"100"}, ParsedStatement::DEFAULT_PROCEDURE_NAME, ParsedStatement::DEFAULT_NULL_STMT_NO),
  ParsedStatement(8, 6, ParsedStatement::DEFAULT_NULL_STMT_NO, StatementType::kassign_stmt, ParsedStatement::defaultPattern, "f", {}, {"z"}, {"5"}, ParsedStatement::DEFAULT_PROCEDURE_NAME, 7),
  ParsedStatement(9, 6, ParsedStatement::DEFAULT_NULL_STMT_NO, StatementType::kassign_stmt, ParsedStatement::defaultPattern, "f", {}, {"y"}, {"100"}, ParsedStatement::DEFAULT_PROCEDURE_NAME, ParsedStatement::DEFAULT_NULL_STMT_NO),
  ParsedStatement(10, 6, ParsedStatement::DEFAULT_NULL_STMT_NO, StatementType::kassign_stmt, ParsedStatement::defaultPattern, "f", {}, {"z"}, {"0"}, ParsedStatement::DEFAULT_PROCEDURE_NAME, 9),
  ParsedStatement(11, ParsedStatement::DEFAULT_NULL_STMT_NO, ParsedStatement::DEFAULT_NULL_STMT_NO, StatementType::kprint_stmt, ParsedStatement::defaultPattern, "f", {"x"}, {}, {}, ParsedStatement::DEFAULT_PROCEDURE_NAME, 4),
};


TEST_CASE("QPS Handler and Result Formatter") { //Work in progress
    std::vector<std::vector<ParsedStatement>> stmtlsts; 
    std::vector<ParsedStatement> stmtlst;
    for (const ParsedStatement& parsedStatement : pStatements)
        stmtlst.push_back(parsedStatement);
        

    stmtlsts.push_back(stmtlst);
    pkb.getSetter()->insertStmts(stmtlsts);
    std::set<ProgramElement> statements;

    ProgramElement s1 = ProgramElement::createStatement(ElementType::STATEMENT, 1);
    ProgramElement s2 = ProgramElement::createStatement(ElementType::STATEMENT, 2);
    ProgramElement s3 = ProgramElement::createStatement(ElementType::STATEMENT, 3);
    ProgramElement s4 = ProgramElement::createStatement(ElementType::STATEMENT, 4);
    ProgramElement s5 = ProgramElement::createStatement(ElementType::STATEMENT, 5);
    ProgramElement s6 = ProgramElement::createStatement(ElementType::STATEMENT, 6);
    ProgramElement s7 = ProgramElement::createStatement(ElementType::STATEMENT, 7);
    ProgramElement s8 = ProgramElement::createStatement(ElementType::STATEMENT, 8);
    ProgramElement s9 = ProgramElement::createStatement(ElementType::STATEMENT, 9);
    ProgramElement s10 = ProgramElement::createStatement(ElementType::STATEMENT, 10);
    ProgramElement s11 = ProgramElement::createStatement(ElementType::STATEMENT, 11);
    statements.insert(s1);
    statements.insert(s2);
    statements.insert(s3);
    statements.insert(s4);
    statements.insert(s5);
    statements.insert(s6);
    statements.insert(s7);
    statements.insert(s8);
    statements.insert(s9);
    statements.insert(s10);
    statements.insert(s11);

    std::set<ProgramElement> variables;
    ProgramElement vx = ProgramElement::createVariable("x");
    ProgramElement vy = ProgramElement::createVariable("y");
    ProgramElement vz = ProgramElement::createVariable("z");
    variables.insert(vx);
    variables.insert(vy);
    variables.insert(vz);

    std::set<ProgramElement> constants;
    ProgramElement c0 = ProgramElement::createConstant("0");
    ProgramElement c100 = ProgramElement::createConstant("100");
    ProgramElement c5 = ProgramElement::createConstant("5");
    ProgramElement c10 = ProgramElement::createConstant("10");
    ProgramElement c4 = ProgramElement::createConstant("4");
    constants.insert(c0);
    constants.insert(c100);
    constants.insert(c5);
    constants.insert(c10);
    constants.insert(c4);


    std::set<ProgramElement> assignments;
    ProgramElement a1 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 1);
    ProgramElement a2 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 2);
    ProgramElement a3 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 3);
    ProgramElement a7 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 7);
    ProgramElement a8 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 8);
    ProgramElement a9 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 9);
    ProgramElement a10 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 10);
    assignments.insert(a1);
    assignments.insert(a2);
    assignments.insert(a3);
    assignments.insert(a7);
    assignments.insert(a8);
    assignments.insert(a9);
    assignments.insert(a10);


    std::set<ProgramElement> prints;
    ProgramElement print1 = ProgramElement::createStatement(ElementType::PRINT, 11);
    prints.insert(print1);

    std::set<ProgramElement> reads;
    ProgramElement r1 = ProgramElement::createStatement(ElementType::READ, 5);
    reads.insert(r1);

    std::set<ProgramElement> whiles;
    ProgramElement w1 = ProgramElement::createStatement(ElementType::WHILE, 4);
    whiles.insert(w1);

    SECTION("NO CLAUSES") {
        std::string nc1 = "variable v; Select v";
        std::string nc2 = "constant c; Select c";
        std::string nc3 = "assign a; Select a";
        std::string nc4 = "print p; Select p";

        std::set<ProgramElement> result1 = rp.processResults(qh.processClause(qp.parsePQL(nc1)));
        std::set<ProgramElement> result2 = rp.processResults(qh.processClause(qp.parsePQL(nc2)));
        std::set<ProgramElement> result3 = rp.processResults(qh.processClause(qp.parsePQL(nc3)));
        std::set<ProgramElement> result4 = rp.processResults(qh.processClause(qp.parsePQL(nc4)));
        REQUIRE(result1 == variables);
        REQUIRE(result2 == constants);
        REQUIRE(result3 == assignments);
        //REQUIRE(result4 == prints);
    }

    SECTION("SUCH THAT CLAUSE - Follows/Follows*/Parent/Parent*") {
        std::string st1 = "if ifs; print p;\nSelect p such that Parent (4, ifs)";
        std::string st2 = "while w; assign a;\nSelect a such that Parent* (w, a)";
        std::string st3 = "print p; Select p such that Follows* (_, p)";
        std::string st4 = "read r; Select r such that Follows* (r, _)";
        std::string st5 = "stmt s; Select s such that Parent*(s, 7)";
        std::string st6 = "stmt s; Select s such that Follows(_, _)";
        std::string st7 = "assign a; assign a1; Select a such that Follows(a, a1)";
        std::string st8 = "constant c; Select c such that Follows*(1, 11)";
        std::string st9 = "read r; Select r such that Follows(5, _)";
        std::string st10 = "while w; Select w such that Parent*(4, 7)";
        std::string st11 = "read r; Select r such that Follows(_, 5)";
        std::string st12 = "assign a; Select a such that Follows*(7, 9)";

        std::set<ProgramElement> empty = {};
        std::set<ProgramElement> expectedResult2;
        expectedResult2.insert(a7);
        expectedResult2.insert(a8);
        expectedResult2.insert(a9);
        expectedResult2.insert(a10);
        std::set<ProgramElement> expectedResult5;
        expectedResult5.insert(s4);
        expectedResult5.insert(s6);
        std::set<ProgramElement> expectedResult7;
        expectedResult7.insert(a1);
        expectedResult7.insert(a2);
        expectedResult7.insert(a7);
        expectedResult7.insert(a9);


        std::set<ProgramElement> result1 = rp.processResults(qh.processClause(qp.parsePQL(st1)));
        std::set<ProgramElement> result2 = rp.processResults(qh.processClause(qp.parsePQL(st2)));
        std::set<ProgramElement> result3 = rp.processResults(qh.processClause(qp.parsePQL(st3)));
        std::set<ProgramElement> result4 = rp.processResults(qh.processClause(qp.parsePQL(st4)));
        std::set<ProgramElement> result5 = rp.processResults(qh.processClause(qp.parsePQL(st5)));
        std::set<ProgramElement> result6 = rp.processResults(qh.processClause(qp.parsePQL(st6)));
        std::set<ProgramElement> result7 = rp.processResults(qh.processClause(qp.parsePQL(st7)));
        std::set<ProgramElement> result8 = rp.processResults(qh.processClause(qp.parsePQL(st8)));
        std::set<ProgramElement> result9 = rp.processResults(qh.processClause(qp.parsePQL(st9)));
        std::set<ProgramElement> result10 = rp.processResults(qh.processClause(qp.parsePQL(st10)));
        std::set<ProgramElement> result11 = rp.processResults(qh.processClause(qp.parsePQL(st11)));
        std::set<ProgramElement> result12 = rp.processResults(qh.processClause(qp.parsePQL(st12)));

        //REQUIRE(result1 == prints);
        REQUIRE(result2 == expectedResult2);
        //REQUIRE(result3 == prints);
        //REQUIRE(result4 == reads);
        //REQUIRE(result5 == expectedResult5);
        //REQUIRE(result6 == statements);
        //REQUIRE(result7 == expectedResult7);
        //REQUIRE(result8 == constants);
        //REQUIRE(result9 == reads);
        //REQUIRE(result10 == whiles);
        //REQUIRE(result11 == empty);
        //REQUIRE(result12 == empty);
    }

    SECTION("SUCH THAT CLAUSE - Modifies/Uses") {
        std::string st1 = "assign a; Select a such that Modifies(1, _)";
        std::string st2 = "assign a; Select a such that Uses(1, _)";
        std::string st3 = "assign a; Select a such that Modifies(a, \"x\")";
        std::string st4 = "while w; Select w such that Modifies(1, \"x\")";
        std::string st5 = "read r; Select r such that Modifies(r, \"z\")";
        std::string st6 = "print p; Select p such that Uses(r, \"z\")";
        std::string st7 = "while w; Select w such that Uses(w, \"x\")";
        std::string st8 = "if ifs; Select ifs such that Modifies(ifs, \"x\")";
        std::string st9 = "stmt s; variable v; Select v such that Uses(s, v)";
        std::string st10 = "constant c; variable v; Select c such that Uses(2, v)";
        std::string st11 = "constant c; Select c such that Modifies(a, \"1x1\")";
        std::string st12 = "assign a; Select a such that Modifies(a, _)";
        std::string st13 = "stmt s; Select s such that Modifies(s, \"z\")";

        std::set<ProgramElement> expectedResult3;
        expectedResult3.insert(a1);
        expectedResult3.insert(a3);
        expectedResult3.insert(a7);
        std::set<ProgramElement> expectedResult8;
        expectedResult8.insert(ProgramElement::createStatement(ElementType::IF, 6));
        std::set<ProgramElement> expectedResult9;
        expectedResult9.insert(vx);
        expectedResult9.insert(vz);
        std::set<ProgramElement> expectedResult13;
        expectedResult13.insert(s4);
        expectedResult13.insert(s5);
        expectedResult13.insert(s6);
        expectedResult13.insert(s8);
        expectedResult13.insert(s10);

        std::set<ProgramElement> empty = {};
        std::set<ProgramElement> result1 = rp.processResults(qh.processClause(qp.parsePQL(st1)));
        std::set<ProgramElement> result2 = rp.processResults(qh.processClause(qp.parsePQL(st2)));
        std::set<ProgramElement> result3 = rp.processResults(qh.processClause(qp.parsePQL(st3)));
        std::set<ProgramElement> result4 = rp.processResults(qh.processClause(qp.parsePQL(st4)));
        std::set<ProgramElement> result5 = rp.processResults(qh.processClause(qp.parsePQL(st5)));
        std::set<ProgramElement> result6 = rp.processResults(qh.processClause(qp.parsePQL(st6)));
        std::set<ProgramElement> result7 = rp.processResults(qh.processClause(qp.parsePQL(st7)));
        std::set<ProgramElement> result8 = rp.processResults(qh.processClause(qp.parsePQL(st8)));
        std::set<ProgramElement> result9 = rp.processResults(qh.processClause(qp.parsePQL(st9)));
        std::set<ProgramElement> result10 = rp.processResults(qh.processClause(qp.parsePQL(st10)));
        std::set<ProgramElement> result11 = rp.processResults(qh.processClause(qp.parsePQL(st11)));
        std::set<ProgramElement> result12 = rp.processResults(qh.processClause(qp.parsePQL(st12)));
        std::set<ProgramElement> result13 = rp.processResults(qh.processClause(qp.parsePQL(st13)));

        REQUIRE(result1 == assignments);
        REQUIRE(result2 == empty);
        REQUIRE(result3 == expectedResult3);
        REQUIRE(result4 == whiles);
        //REQUIRE(result5 == reads);
        REQUIRE(result6 == empty);
        REQUIRE(result7 == whiles);
        REQUIRE(result8 == expectedResult8);
        REQUIRE(result9 == expectedResult9);
        REQUIRE(result10 == constants);
        REQUIRE(result11 == empty);
        REQUIRE(result12 == assignments);
        //REQUIRE(result13 == expectedResult13);
    }

    SECTION("PATTERN CLAUSE") {
        std::string p1 = "assign a; Select a pattern a(_, _\"5\"_)";
        std::string p2 = "assign a; Select a pattern a(\"x\", _)";
        std::string p3 = "assign a; Select a pattern a(\"y\", _\"x\"_)";
        std::string p4 = "assign a; Select a pattern a(_, _)";
        std::string p5 = "assign a; assign a1; Select a1 pattern a(_, _)";
        std::string p6 = "assign a; assign a1; Select a1 pattern a(_, _\"5\"_)";
        std::string p7 = "assign a; assign a1; Select a1 pattern a(\"b\", _)";
        std::string p8 = "assign a; assign a1; Select a1 pattern a(_, _\"b\")";
        std::string p9 = "assign a; Select a pattern a(\"b\", _)";
        std::string p10 = "assign a; while w; Select w pattern a(_, _\"5\"_)";
        std::string p11 = "assign a; variable v; Select v pattern a (v, _)";
        std::string p12 = "assign a; variable v; Select a pattern a (v, _)";

        std::set<ProgramElement> empty = {};
        std::set<ProgramElement> expectedResult1;
        expectedResult1.insert(a1);
        expectedResult1.insert(a3);
        expectedResult1.insert(a8);
        std::set<ProgramElement> expectedResult2;
        expectedResult2.insert(a1);
        expectedResult2.insert(a3);
        expectedResult2.insert(a7);
        std::set<ProgramElement> expectedResult3;
        expectedResult3.insert(a2);

        std::set<ProgramElement> result1 = rp.processResults(qh.processClause(qp.parsePQL(p1)));
        std::set<ProgramElement> result2 = rp.processResults(qh.processClause(qp.parsePQL(p2)));
        std::set<ProgramElement> result3 = rp.processResults(qh.processClause(qp.parsePQL(p3)));
        std::set<ProgramElement> result4 = rp.processResults(qh.processClause(qp.parsePQL(p4)));
        std::set<ProgramElement> result5 = rp.processResults(qh.processClause(qp.parsePQL(p5)));
        std::set<ProgramElement> result6 = rp.processResults(qh.processClause(qp.parsePQL(p6)));
        std::set<ProgramElement> result7 = rp.processResults(qh.processClause(qp.parsePQL(p7)));
        std::set<ProgramElement> result8 = rp.processResults(qh.processClause(qp.parsePQL(p8)));
        std::set<ProgramElement> result9 = rp.processResults(qh.processClause(qp.parsePQL(p9)));
        std::set<ProgramElement> result10 = rp.processResults(qh.processClause(qp.parsePQL(p10)));
        std::set<ProgramElement> result11 = rp.processResults(qh.processClause(qp.parsePQL(p11)));
        std::set<ProgramElement> result12 = rp.processResults(qh.processClause(qp.parsePQL(p12)));

        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResult2);
        REQUIRE(result3 == expectedResult3);
        REQUIRE(result4 == assignments);
        REQUIRE(result5 == assignments);
        REQUIRE(result6 == assignments);
        REQUIRE(result7 == empty);
        REQUIRE(result8 == empty);
        REQUIRE(result9 == empty);
        REQUIRE(result10 == whiles);
        REQUIRE(result11 == variables);
        REQUIRE(result12 == assignments);
    }

    SECTION("COMBINATION CLAUSES") {
        std::string com1 = "if ifs; stmt s; assign a; assign a1; Select a1 such that Parent (ifs, a1) pattern a1 (_, _\"100\"_)";
        std::string com2 = "assign a; assign a1; Select a1 such that Follows (a, a1) pattern a (_, _\"100\"_)";
        std::string com3 = "assign a; assign a1; Select a1 such that Follows (a1, a) pattern a (_, _\"5\"_)";
        std::string com4 = "if ifs; stmt s; assign a; assign a1; Select a such that Follows (a1, s) pattern a1 (_, _\"100\"_)";
        std::string com5 = "constant c; variable v; assign a; assign a1; Select c such that Modifies (a, v) pattern a1 (_, _\"15\"_)";
        std::string com6 = "assign a, a1; variable v; Select a such that Uses (a1, v) pattern a (v, _)";
        std::string com7 = "assign a, a1; variable v, v1; Select v such that Uses (a, v) pattern a1 (v, _)";
        std::string com8 = "assign a, a1; variable v, v1; Select v1 such that Modifies (a, v) pattern a1 (v1, _)";
        std::string com9 = "assign a; variable v; Select a such that Modifies (a, \"y\") pattern a (v, _\"100\"_)";
        std::string com10 = "print p; assign a; variable v; Select a such that Uses (p, \"x\") pattern a (v, _\"x\"_)";

        std::set<ProgramElement> empty = {};
        std::set<ProgramElement> expectedResult1;
        expectedResult1.insert(a7);
        expectedResult1.insert(a9);
        std::set<ProgramElement> expectedResult2;
        expectedResult2.insert(a8);
        expectedResult2.insert(a10);
        std::set<ProgramElement> expectedResult3;
        expectedResult3.insert(a2);
        expectedResult3.insert(a7);
        std::set<ProgramElement> expectedResult6;
        expectedResult6.insert(a1);
        expectedResult6.insert(a3);
        expectedResult6.insert(a7);
        std::set<ProgramElement> expectedResult7;
        expectedResult7.insert(vx);
        std::set<ProgramElement> expectedResult9;
        expectedResult9.insert(a9);
        std::set<ProgramElement> expectedResult10;
        expectedResult10.insert(a2);

        std::set<ProgramElement> result1 = rp.processResults(qh.processClause(qp.parsePQL(com1)));
        std::set<ProgramElement> result2 = rp.processResults(qh.processClause(qp.parsePQL(com2)));
        std::set<ProgramElement> result3 = rp.processResults(qh.processClause(qp.parsePQL(com3)));
        std::set<ProgramElement> result4 = rp.processResults(qh.processClause(qp.parsePQL(com4)));
        std::set<ProgramElement> result5 = rp.processResults(qh.processClause(qp.parsePQL(com5)));
        std::set<ProgramElement> result6 = rp.processResults(qh.processClause(qp.parsePQL(com6)));
        std::set<ProgramElement> result7 = rp.processResults(qh.processClause(qp.parsePQL(com7)));
        std::set<ProgramElement> result8 = rp.processResults(qh.processClause(qp.parsePQL(com8)));
        std::set<ProgramElement> result9 = rp.processResults(qh.processClause(qp.parsePQL(com9)));
        std::set<ProgramElement> result10 = rp.processResults(qh.processClause(qp.parsePQL(com10)));

        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResult2);
        REQUIRE(result3 == expectedResult3);
        REQUIRE(result4 == assignments);
        REQUIRE(result5 == empty);
        REQUIRE(result6 == expectedResult6);
        REQUIRE(result7 == expectedResult7);
        REQUIRE(result8 == variables);
        REQUIRE(result9 == expectedResult9);
        REQUIRE(result10 == expectedResult10);

    }
}
