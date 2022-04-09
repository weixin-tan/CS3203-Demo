#include "catch.hpp"
#include "PKB.h"
#include "QPS/QueryProcessor.h"
#include "QPS/QPSMainLogic.h"


PKB pkb = PKB();
QPSMainLogic qpsMainLogic = QPSMainLogic(pkb.getGetter());

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
//  call g; // 12
//}
//procedure g {
//  print x;
//}

bool compareStringLists(std::list<std::string> listA, std::list<std::string> listB) {
    std::set<std::string> firstSet;
    std::set<std::string> secondSet;

    for (const auto& s : listA) {
        firstSet.insert(s);
    }
    for (const auto& s : listB) {
        secondSet.insert(s);
    }

    return firstSet == secondSet;
}

ExpressionProcessor ep;
std::vector<ParsedStatement> stmtList1 = {

                {
                                ParsedStatement(1,
                                                ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                StatementType::ASSIGNMENT_STMT,
                                                ep.stringToExpr("5"),
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
                                                ep.stringToExpr("x"),
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
                                                ep.stringToExpr("5"),
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
                                                ep.stringToExpr("x > 4"),
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
                                                ep.stringToExpr("z > 10"),
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
                                                ep.stringToExpr("100"),
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
                                                ep.stringToExpr("5"),
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
                                                ep.stringToExpr("100"),
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
                                                ep.stringToExpr("0"),
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
                                ParsedStatement(12,
                                                ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                StatementType::CALL_STMT,
                                                Expr(),
                                                "f",
                                                {},
                                                {},
                                                {},
                                                "g",
                                                11)
                }
};

std::vector<ParsedStatement> stmtList2 = {ParsedStatement(13,
                                                          ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                          ParsedStatement::DEFAULT_NULL_STMT_NO,
                                                          StatementType::PRINT_STMT,
                                                          Expr(),
                                                          "g",
                                                          {"x"},
                                                          {},
                                                          {},
                                                          ParsedStatement::DEFAULT_PROCEDURE_NAME,
                                                          ParsedStatement::DEFAULT_NULL_STMT_NO
                                                          )};


TEST_CASE("Integration Testing") {
    std::vector<std::vector<ParsedStatement>> stmtlsts = {stmtList1, stmtList2};
    pkb.getSetter()->insertStmts(stmtlsts);

    SECTION("NO CLAUSES") {
        std::string nc1 = "variable v; Select v";
        std::string nc2 = "constant c; Select c";
        std::string nc3 = "assign a; Select a";
        std::string nc4 = "print p; Select p";
        std::string nc5 = "print p; constant c; Select <p, c>";
        std::string nc6 = "read r; Select r.varName";

        std::list<std::string> result1 = qpsMainLogic.parse(nc1);
        std::list<std::string> result2 = qpsMainLogic.parse(nc2);
        std::list<std::string> result3 = qpsMainLogic.parse(nc3);
        std::list<std::string> result4 = qpsMainLogic.parse(nc4);
        std::list<std::string> result5 = qpsMainLogic.parse(nc5);
        std::list<std::string> result6 = qpsMainLogic.parse(nc6);

        std::list<std::string> expectedResult1 = {"x", "y", "z"};
        std::list<std::string> expectedResult2 = {"0", "4", "5", "10", "100"};
        std::list<std::string> expectedResult3 = {"1", "2", "3", "7", "8", "9", "10"};
        std::list<std::string> expectedResult4 = {"11", "13"};
        std::list<std::string> expectedResult5 = {"11 0", "11 4", "11 5", "11 10", "11 100",
                                                  "13 0", "13 4", "13 5", "13 10", "13 100"};
        std::list<std::string> expectedResult6 = {"z"};

        REQUIRE(compareStringLists(result1, expectedResult1));
        REQUIRE(compareStringLists(result2, expectedResult2));
        REQUIRE(compareStringLists(result3, expectedResult3));
        REQUIRE(compareStringLists(result4, expectedResult4));
        REQUIRE(compareStringLists(result5, expectedResult5));
        REQUIRE(compareStringLists(result6, expectedResult6));

    }

    SECTION("SUCH THAT CLAUSES - Follows(*)/Parent(*)/Next(*)/Affects(*)") {
        std::string st1 = "stmt s;\nSelect s such that Affects* (1, 2)";
        std::string st2 = "stmt s;\nSelect s such that Affects(_, 2)";
        std::string st3 = "stmt s;\nSelect s such that Next*(2, _)";
        std::string st4 = "stmt s;\nSelect s such that Follows*(_, _)";
        std::string st5 = "stmt s;\nSelect s such that Next(s, 8)";
        std::string st6 = "stmt s;\nSelect s such that Parent*(s, 7)";
        std::string st7 = "stmt s1, s2;\nSelect s2 such that Parent(s1, s2)";
        std::string st8 = "stmt s;\nSelect s such that Follows(_, s)";
        std::string st9 = "stmt s;\nSelect s such that Parent(s, _)";
        std::string st10 = "variable v;\nSelect v such that Follows(_, _)";
        std::string st11 = "while w; call c;\n Select c such that Affects(w, c)";

        std::list<std::string> result1 = qpsMainLogic.parse(st1);
        std::list<std::string> result2 = qpsMainLogic.parse(st2);
        std::list<std::string> result3 = qpsMainLogic.parse(st3);
        std::list<std::string> result4 = qpsMainLogic.parse(st4);
        std::list<std::string> result5 = qpsMainLogic.parse(st5);
        std::list<std::string> result6 = qpsMainLogic.parse(st6);
        std::list<std::string> result7 = qpsMainLogic.parse(st7);
        std::list<std::string> result8 = qpsMainLogic.parse(st8);
        std::list<std::string> result9 = qpsMainLogic.parse(st9);
        std::list<std::string> result10 = qpsMainLogic.parse(st10);
        std::list<std::string> result11 = qpsMainLogic.parse(st11);

        std::list<std::string> allStmts = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"};
        std::list<std::string> expectedResult5 = {"7"};
        std::list<std::string> expectedResult6 = {"4", "6"};
        std::list<std::string> expectedResult7 = {"5", "6", "7", "8", "9", "10"};
        std::list<std::string> expectedResult8 = {"2", "3", "4", "6", "8", "10", "11", "12"};
        std::list<std::string> expectedResult9 = {"4", "6"};
        std::list<std::string> expectedResult10 = {"x", "y", "z"};
        std::list<std::string> expectedResult11 = {};

        REQUIRE(compareStringLists(result1, allStmts));
        REQUIRE(compareStringLists(result2, allStmts));
        REQUIRE(compareStringLists(result3, allStmts));
        REQUIRE(compareStringLists(result4, allStmts));
        REQUIRE(compareStringLists(result5, expectedResult5));
        REQUIRE(compareStringLists(result6, expectedResult6));
        REQUIRE(compareStringLists(result7, expectedResult7));
        REQUIRE(compareStringLists(result8, expectedResult8));
        REQUIRE(compareStringLists(result9, expectedResult9));
        REQUIRE(compareStringLists(result10, expectedResult10));
        REQUIRE(compareStringLists(result11, expectedResult11));

    }
    /*
    SECTION("SUCH THAT CLAUSE - Modifies/Uses/Calls(*)") {
        std::string st1 = "assign a; Select a such that Modifies(1, \"x\")";
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
    /*
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
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << "Time taken by function: "
        << duration.count() << " microseconds" << endl;
}
*/
}
