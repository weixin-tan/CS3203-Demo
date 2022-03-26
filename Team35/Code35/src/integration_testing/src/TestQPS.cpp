#include "catch.hpp"
#include "QPS/PostOptimiser.h"
#include "PKB.h"
#include "QPS/QueryProcessor.h"
#include "QPS/QPSMainLogic.h"

PKB pkb = PKB();
QPSMainLogic* qr = QPSMainLogic::getInstance(pkb.getGetter());
QueryProcessor qp = QueryProcessor();
QPSHandler qh = QPSHandler(pkb.getGetter());
PreOptimiser preOp = PreOptimiser();
PostOptimiser op = PostOptimiser();
ResultProcessor rp = ResultProcessor();


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

bool compareProgramElementLists(std::vector<ProgramElement> lista, std::vector<ProgramElement> listb) {
    bool toReturn = true;
    for (ProgramElement a : lista) {
        bool temp = false;
        for (ProgramElement b : listb) {
            temp = temp || b == a;
        }
        toReturn = toReturn && temp;
    }
    return toReturn;
}


TEST_CASE("test comparision functions"){
    ProgramElement a = ProgramElement::createStatement(ElementType::STATEMENT, 1);
    ProgramElement b = ProgramElement::createStatement(ElementType::STATEMENT, 2);
    ProgramElement c = ProgramElement::createStatement(ElementType::STATEMENT, 1);
    ProgramElement d = ProgramElement::createStatement(ElementType::STATEMENT, 3);
    SECTION("testing compareProgramElementLists function"){
        std::vector<ProgramElement> x1;
        x1.push_back(a);
        std::vector<ProgramElement> y1;
        y1.push_back(b);
        std::vector<ProgramElement> x2;
        x2.push_back(a);
        std::vector<ProgramElement> y2;
        y2.push_back(c);
        std::vector<ProgramElement> x3;
        x3.push_back(a);
        x3.push_back(b);
        std::vector<ProgramElement> y3;
        y3.push_back(b);
        y3.push_back(c);
        std::vector<ProgramElement> x4;
        x4.push_back(a);
        x4.push_back(c);
        std::vector<ProgramElement> y4;
        y4.push_back(b);
        y4.push_back(d);
        std::vector<ProgramElement> x5;
        x5.push_back(a);
        x5.push_back(b);
        std::vector<ProgramElement> y5;
        y5.push_back(c);
        y5.push_back(d);
        REQUIRE(!compareProgramElementLists(x1,y1));
        REQUIRE(compareProgramElementLists(x2,y2));
        REQUIRE(compareProgramElementLists(x3,y3));
        REQUIRE(!compareProgramElementLists(x4,y4));
        REQUIRE(!compareProgramElementLists(x5,y5));
    }
}
//#to-do viv check if this is right bc i have no idea lmao
std::vector<ParsedStatement> pStatements = {
        ParsedStatement(1,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        StatementType::ASSIGNMENT_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {},
                        {"x"},
                        {"5"},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        ParsedStatement::DEFAULT_NULL_STMT_NO),
        ParsedStatement(2,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        StatementType::ASSIGNMENT_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {"x"},
                        {"y"},
                        {},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        1),
        ParsedStatement(3,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        StatementType::ASSIGNMENT_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {},
                        {"x"},
                        {"5"},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        2),
        ParsedStatement(4,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        StatementType::WHILE_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {"x"},
                        {},
                        {"4"},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        3),
        ParsedStatement(5,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        4,
                        StatementType::READ_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {},
                        {"z"},
                        {},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        ParsedStatement::DEFAULT_NULL_STMT_NO),
        ParsedStatement(6,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        4,
                        StatementType::IF_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {"z"},
                        {},
                        {"10"},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        5),
        ParsedStatement(7,
                        6,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        StatementType::ASSIGNMENT_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {},
                        {"x"},
                        {"100"},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        ParsedStatement::DEFAULT_NULL_STMT_NO),
        ParsedStatement(8,
                        6,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        StatementType::ASSIGNMENT_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {},
                        {"z"},
                        {"5"},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        7),
        ParsedStatement(9,
                        6,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        StatementType::ASSIGNMENT_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {},
                        {"y"},
                        {"100"},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        ParsedStatement::DEFAULT_NULL_STMT_NO),
        ParsedStatement(10,
                        6,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        StatementType::ASSIGNMENT_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {},
                        {"z"},
                        {"0"},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        9),
        ParsedStatement(11,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        ParsedStatement::DEFAULT_NULL_STMT_NO,
                        StatementType::PRINT_STMT,
                        ParsedStatement::defaultPattern,
                        "f",
                        {"x"},
                        {},
                        {},
                        ParsedStatement::DEFAULT_PROCEDURE_NAME,
                        4),
};

TEST_CASE("Integration Testing") {
    std::vector<std::vector<ParsedStatement>> stmtlsts;
    std::vector<ParsedStatement> stmtlst;

    for (const ParsedStatement& parsedStatement : pStatements)
        stmtlst.push_back(parsedStatement);

    stmtlsts.push_back(stmtlst);
    pkb.getSetter()->insertStmts(stmtlsts);
    std::vector<ProgramElement> statements;

    ProgramElement s1 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 1);
    ProgramElement s2 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 2);
    ProgramElement s3 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 3);
    ProgramElement s4 = ProgramElement::createStatement(ElementType::WHILE, 4);
    ProgramElement s5 = ProgramElement::createStatement(ElementType::READ, 5, "z");
    ProgramElement s6 = ProgramElement::createStatement(ElementType::IF, 6);
    ProgramElement s7 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 7);
    ProgramElement s8 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 8);
    ProgramElement s9 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 9);
    ProgramElement s10 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 10);
    ProgramElement s11 = ProgramElement::createStatement(ElementType::PRINT, 11, "x");

    statements.push_back(s1);
    statements.push_back(s2);
    statements.push_back(s3);
    statements.push_back(s4);
    statements.push_back(s5);
    statements.push_back(s6);
    statements.push_back(s7);
    statements.push_back(s8);
    statements.push_back(s9);
    statements.push_back(s10);
    statements.push_back(s11);

    std::vector<ProgramElement> variables;
    ProgramElement vx = ProgramElement::createVariable("x");
    ProgramElement vy = ProgramElement::createVariable("y");
    ProgramElement vz = ProgramElement::createVariable("z");
    variables.push_back(vx);
    variables.push_back(vy);
    variables.push_back(vz);

    std::vector<ProgramElement> constants;
    ProgramElement c0 = ProgramElement::createConstant("0");
    ProgramElement c100 = ProgramElement::createConstant("100");
    ProgramElement c5 = ProgramElement::createConstant("5");
    ProgramElement c10 = ProgramElement::createConstant("10");
    ProgramElement c4 = ProgramElement::createConstant("4");
    constants.push_back(c0);
    constants.push_back(c100);
    constants.push_back(c5);
    constants.push_back(c10);
    constants.push_back(c4);

    std::vector<ProgramElement> assignments;
    ProgramElement a1 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 1);
    ProgramElement a2 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 2);
    ProgramElement a3 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 3);
    ProgramElement a7 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 7);
    ProgramElement a8 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 8);
    ProgramElement a9 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 9);
    ProgramElement a10 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 10);
    assignments.push_back(a1);
    assignments.push_back(a2);
    assignments.push_back(a3);
    assignments.push_back(a7);
    assignments.push_back(a8);
    assignments.push_back(a9);
    assignments.push_back(a10);

    std::vector<ProgramElement> prints;
    ProgramElement print1 = ProgramElement::createStatement(ElementType::PRINT, 11, "x");
    prints.push_back(print1);

    std::vector<ProgramElement> reads;
    ProgramElement r1 = ProgramElement::createStatement(ElementType::READ, 5, "z");
    reads.push_back(r1);

    std::vector<ProgramElement> whiles;
    ProgramElement w1 = ProgramElement::createStatement(ElementType::WHILE, 4);
    whiles.push_back(w1);

    std::vector<ProgramElement> ifs;
    ProgramElement if1 = ProgramElement::createStatement(ElementType::IF, 6);
    ifs.push_back(if1);

    std::vector<ProgramElement> procedures;
    ProgramElement prod1 = ProgramElement::createProcedure("f");

    SECTION("NO CLAUSES") {
        std::string nc1 = "variable v; Select v";
        std::string nc2 = "constant c; Select c";
        std::string nc3 = "assign a; Select a";
        std::string nc4 = "print p; Select p";

        std::vector<ProgramElement> result1 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                nc1)))));
        std::vector<ProgramElement> result2 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                nc2)))));
        std::vector<ProgramElement> result3 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                nc3)))));
        std::vector<ProgramElement> result4 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                nc4)))));

        REQUIRE(compareProgramElementLists(result1, variables));
        REQUIRE(compareProgramElementLists(result2, constants));
        REQUIRE(compareProgramElementLists(result3, assignments));
        REQUIRE(compareProgramElementLists(result4, prints));

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

        std::vector<ProgramElement> empty = {};
        std::vector<ProgramElement> expectedResult2;
        expectedResult2.push_back(a7);
        expectedResult2.push_back(a8);
        expectedResult2.push_back(a9);
        expectedResult2.push_back(a10);
        std::vector<ProgramElement> expectedResult5;
        expectedResult5.push_back(s4);
        expectedResult5.push_back(s6);
        std::vector<ProgramElement> expectedResult7;
        expectedResult7.push_back(a1);
        expectedResult7.push_back(a2);
        expectedResult7.push_back(a7);
        expectedResult7.push_back(a9);

        std::vector<ProgramElement> result1 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st1)))));
        std::vector<ProgramElement> result2 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st2)))));
        std::vector<ProgramElement> result3 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st3)))));
        std::vector<ProgramElement> result4 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st4)))));
        std::vector<ProgramElement> result5 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st5)))));
        std::vector<ProgramElement> result6 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st6)))));
        std::vector<ProgramElement> result7 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st7)))));
        std::vector<ProgramElement> result8 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st8)))));
        std::vector<ProgramElement> result9 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st9)))));
        std::vector<ProgramElement> result10 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st10)))));
        std::vector<ProgramElement> result11 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st11)))));
        std::vector<ProgramElement> result12 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st12)))));

        REQUIRE(compareProgramElementLists(result1, prints));
        REQUIRE(compareProgramElementLists(result2, expectedResult2));
        REQUIRE(compareProgramElementLists(result3, prints));
        REQUIRE(compareProgramElementLists(result4, reads));
        REQUIRE(compareProgramElementLists(result5, expectedResult5));
        REQUIRE(compareProgramElementLists(result6, statements));
        REQUIRE(compareProgramElementLists(result7, expectedResult7));
        REQUIRE(compareProgramElementLists(result8, constants));
        REQUIRE(compareProgramElementLists(result9, reads));
        REQUIRE(compareProgramElementLists(result10, whiles));
        REQUIRE(compareProgramElementLists(result11, reads));
        REQUIRE(compareProgramElementLists(result12, empty));

    }

    SECTION("SUCH THAT CLAUSE - Modifies/Uses") {
        std::string st1 = "assign a; Select a such that Modifies(1, _)";
        std::string st2 = "assign a; Select a such that Uses(1, _)";
        std::string st3 = "assign a; Select a such that Modifies(a, \"x\")";
        std::string st4 = "while w; Select w such that Modifies(1, \"x\")";
        std::string st5 = "read r; Select r such that Modifies(r, \"z\")";
        std::string st6 = "print p; Select p such that Uses(p, \"z\")";
        std::string st7 = "while w; Select w such that Uses(w, \"x\")";
        std::string st8 = "if ifs; Select ifs such that Modifies(ifs, \"x\")";
        std::string st9 = "stmt s; variable v; Select v such that Uses(s, v)";
        std::string st10 = "constant c; variable v; Select c such that Uses(2, v)";
        std::string st11 = "assign a; constant c; Select c such that Modifies(a, \"w\")";
        std::string st12 = "assign a; Select a such that Modifies(a, _)";
        std::string st13 = "stmt s; Select s such that Modifies(s, \"z\")";

        std::vector<ProgramElement> expectedResult3;
        expectedResult3.push_back(a1);
        expectedResult3.push_back(a3);
        expectedResult3.push_back(a7);
        std::vector<ProgramElement> expectedResult8;
        expectedResult8.push_back(ProgramElement::createStatement(ElementType::IF, 6));
        std::vector<ProgramElement> expectedResult9;
        expectedResult9.push_back(vx);
        expectedResult9.push_back(vz);
        std::vector<ProgramElement> expectedResult13;
        expectedResult13.push_back(s4);
        expectedResult13.push_back(s5);
        expectedResult13.push_back(s6);
        expectedResult13.push_back(s8);
        expectedResult13.push_back(s10);

        std::vector<ProgramElement> empty = {};
        std::vector<ProgramElement> result1 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st1)))));
        std::vector<ProgramElement> result2 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st2)))));
        std::vector<ProgramElement> result3 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st3)))));
        std::vector<ProgramElement> result4 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st4)))));
        std::vector<ProgramElement> result5 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st5)))));
        std::vector<ProgramElement> result6 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st6)))));
        std::vector<ProgramElement> result7 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st7)))));
        std::vector<ProgramElement> result8 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st8)))));
        std::vector<ProgramElement> result9 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st9)))));
        std::vector<ProgramElement> result10 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st10)))));
        std::vector<ProgramElement> result11 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st11)))));
        std::vector<ProgramElement> result12 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st12)))));
        std::vector<ProgramElement> result13 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                st13)))));

        REQUIRE(compareProgramElementLists(result1, assignments));
        REQUIRE(compareProgramElementLists(result2, assignments));
        REQUIRE(compareProgramElementLists(result3, expectedResult3));
        REQUIRE(compareProgramElementLists(result4, whiles));
        REQUIRE(compareProgramElementLists(result5, reads));
        REQUIRE(compareProgramElementLists(result6, empty));
        REQUIRE(compareProgramElementLists(result7, whiles));
        REQUIRE(compareProgramElementLists(result8, expectedResult8));
        REQUIRE(compareProgramElementLists(result9, expectedResult9));
        REQUIRE(compareProgramElementLists(result10, constants));
        REQUIRE(compareProgramElementLists(result11, empty));
        REQUIRE(compareProgramElementLists(result12, assignments));
        REQUIRE(compareProgramElementLists(result13, expectedResult13));

    }

    SECTION("PATTERN CLAUSE") {
        std::string p1 = "assign a; Select a pattern a(_, _\"5\"_)";
        std::string p2 = "assign a; Select a pattern a(\"x\", _)";
        std::string p3 = "assign a; Select a pattern a(\"y\", _\"x\"_)";
        std::string p4 = "assign a; Select a pattern a(_, _)";
        std::string p5 = "assign a; assign a1; Select a1 pattern a(_, _)";
        std::string p6 = "assign a; assign a1; Select a1 pattern a(_, _\"5\"_)";
        std::string p7 = "assign a; assign a1; Select a1 pattern a(\"b\", _)";
        std::string p8 = "assign a; assign a1; Select a1 pattern a(_, _\"b\"_)";
        std::string p9 = "assign a; Select a pattern a(\"b\", _)";
        std::string p10 = "assign a; while w; Select w pattern a(_, _\"5\"_)";
        std::string p11 = "assign a; variable v; Select v pattern a (v, _)";
        std::string p12 = "assign a; variable v; Select a pattern a (v, _)";

        std::vector<ProgramElement> empty = {};
        std::vector<ProgramElement> expectedResult1;
        expectedResult1.push_back(a1);
        expectedResult1.push_back(a3);
        expectedResult1.push_back(a8);
        std::vector<ProgramElement> expectedResult2;
        expectedResult2.push_back(a1);
        expectedResult2.push_back(a3);
        expectedResult2.push_back(a7);
        std::vector<ProgramElement> expectedResult3;
        expectedResult3.push_back(a2);

        std::vector<ProgramElement> result1 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p1)))));
        std::vector<ProgramElement> result2 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p2)))));
        std::vector<ProgramElement> result3 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p3)))));
        std::vector<ProgramElement> result4 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p4)))));
        std::vector<ProgramElement> result5 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p5)))));
        std::vector<ProgramElement> result6 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p6)))));
        std::vector<ProgramElement> result7 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p7)))));
        std::vector<ProgramElement> result8 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p8)))));
        std::vector<ProgramElement> result9 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p9)))));
        std::vector<ProgramElement> result10 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p10)))));
        std::vector<ProgramElement> result11 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p11)))));
        std::vector<ProgramElement> result12 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                p12)))));

        REQUIRE(compareProgramElementLists(result1, expectedResult1));
        REQUIRE(compareProgramElementLists(result2, expectedResult2));
        REQUIRE(compareProgramElementLists(result3, expectedResult3));
        REQUIRE(compareProgramElementLists(result4, assignments));
        REQUIRE(compareProgramElementLists(result5, assignments));
        REQUIRE(compareProgramElementLists(result6, assignments));
        REQUIRE(compareProgramElementLists(result7, empty));
        REQUIRE(compareProgramElementLists(result8, empty));
        REQUIRE(compareProgramElementLists(result9, empty));
        REQUIRE(compareProgramElementLists(result10, whiles));
        REQUIRE(compareProgramElementLists(result11, variables));
        REQUIRE(compareProgramElementLists(result12, assignments));
    }

    SECTION("COMBINATION CLAUSES") {

        std::string com1 =
                "if ifs; stmt s; assign a; assign a1; Select a1 such that Parent (ifs, a1) pattern a1 (_, _\"100\"_)";
        std::string com2 = "assign a; assign a1; Select a1 such that Follows (a, a1) pattern a (_, _\"100\"_)";
        std::string com3 = "assign a; assign a1; Select a1 such that Follows (a1, a) pattern a (_, _\"5\"_)";
        std::string com4 =
                "if ifs; stmt s; assign a; assign a1; Select a such that Follows (a1, s) pattern a1 (_, _\"100\"_)";
        std::string com5 =
                "constant c; variable v; assign a; assign a1; Select c such that Modifies (a, v) pattern a1 (_, _\"15\"_)";
        std::string com6 = "assign a, a1; variable v; Select a such that Uses (a1, v) pattern a (v, _)";
        std::string com7 = "assign a, a1; variable v, v1; Select v such that Uses (a, v) pattern a1 (v, _)";
        std::string com8 = "assign a, a1; variable v, v1; Select v1 such that Modifies (a, v) pattern a1 (v1, _)";
        std::string com9 = "assign a; variable v; Select a such that Modifies (a, \"y\") pattern a (v, _\"100\"_)";
        std::string com10 = "print p; assign a; variable v; Select a such that Uses (p, \"x\") pattern a (v, _\"x\"_)";

        std::vector<ProgramElement> empty = {};
        std::vector<ProgramElement> expectedResult1;
        expectedResult1.push_back(a7);
        expectedResult1.push_back(a9);
        std::vector<ProgramElement> expectedResult2;
        expectedResult2.push_back(a8);
        expectedResult2.push_back(a10);
        std::vector<ProgramElement> expectedResult3;
        expectedResult3.push_back(a2);
        expectedResult3.push_back(a7);
        std::vector<ProgramElement> expectedResult6;
        expectedResult6.push_back(a1);
        expectedResult6.push_back(a3);
        expectedResult6.push_back(a7);
        std::vector<ProgramElement> expectedResult7;
        expectedResult7.push_back(vx);
        std::vector<ProgramElement> expectedResult9;
        expectedResult9.push_back(a9);
        std::vector<ProgramElement> expectedResult10;
        expectedResult10.push_back(a2);

        std::vector<ProgramElement> result1 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                com1)))));
        std::vector<ProgramElement> result2 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                com2)))));
        std::vector<ProgramElement> result3 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                com3)))));
        std::vector<ProgramElement> result4 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                com4)))));
        std::vector<ProgramElement> result5 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                com5)))));
        std::vector<ProgramElement> result6 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                com6)))));
        std::vector<ProgramElement> result7 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                com7)))));
        std::vector<ProgramElement> result8 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                com8)))));
        std::vector<ProgramElement> result9 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                com9)))));
        std::vector<ProgramElement> result10 = rp.processResults(op.optimise(qh.processClause(preOp.optimise(qp.parsePQL(
                com10)))));

        REQUIRE(compareProgramElementLists(result1, expectedResult1));
        REQUIRE(compareProgramElementLists(result2, expectedResult2));
        REQUIRE(compareProgramElementLists(result3, expectedResult3));
        REQUIRE(compareProgramElementLists(result4, assignments));
        REQUIRE(compareProgramElementLists(result5, empty));
        REQUIRE(compareProgramElementLists(result6, expectedResult6));
        REQUIRE(compareProgramElementLists(result7, expectedResult7));
        REQUIRE(compareProgramElementLists(result8, variables));
        REQUIRE(compareProgramElementLists(result9, expectedResult9));
        REQUIRE(compareProgramElementLists(result10, expectedResult10));
    }
}

