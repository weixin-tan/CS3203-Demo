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
        std::string st6 = "stmt s;\nSelect s such that Parent*(4, s)";
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
        std::list<std::string> expectedResult6 = {"5", "6", "7", "8", "9", "10"};
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

    SECTION("SUCH THAT CLAUSE - Modifies/Uses/Calls(*)") {
        std::string st1 = "assign a; Select a such that Modifies(1, \"x\")";
        std::string st2 = "assign a; Select a such that Uses(_, \"x\")";
        std::string st3 = "assign a; Select a such that Modifies(3, _)";
        std::string st4 = "assign a; Select a such that Uses(_, _)";
        std::string st5 = "assign a; Select a such that Uses(a, \"x\")";
        std::string st6 = "variable v; Select v such that Modifies(9, v)";
        std::string st7 = "print p; variable v; Select p such that Uses(p, v)";
        std::string st8 = "read r; Select r such that Modifies(r, \"z\")";
        std::string st9 = "variable v; Select v such that Uses(6, v)";
        std::string st10 = "variable v; while w; Select w such that Uses(w, v)";
        std::string st11 = "variable v; Select v such that Modifies(\"f\", v)";
        std::string st12 = "procedure p; Select p such that Uses(p, \"x\")";
        std::string st13 = "variable v; procedure p; Select <p, v> such that Uses(p, v)";
        std::string st14 = "procedure p; Select p such that Calls(p, \"g\")";
        std::string st15 = "procedure p; Select p such that Calls*(\"f\", p)";
        std::string st16 = "procedure p1, p2; Select <p1, p2> such that Calls*(p1, p2)";

        std::list<std::string> assigns = {"1", "2", "3", "7", "8", "9", "10"};
        std::list<std::string> empty = {};
        std::list<std::string> expectedResult5 = {"2"};
        std::list<std::string> expectedResult6 = {"y"};
        std::list<std::string> expectedResult7 = {"11", "13"};
        std::list<std::string> expectedResult8 = {"5"};
        std::list<std::string> expectedResult9 = {"z"};
        std::list<std::string> expectedResult10 = {"4"};
        std::list<std::string> expectedResult11 = {"x", "y", "z"};
        std::list<std::string> expectedResult12 = {"f", "g"};
        std::list<std::string> expectedResult13 = {"f x", "f z", "g x"};
        std::list<std::string> expectedResult14 = {"f"};
        std::list<std::string> expectedResult15 = {"g"};
        std::list<std::string> expectedResult16 = {"f g"};

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
        std::list<std::string> result12 = qpsMainLogic.parse(st12);
        std::list<std::string> result13 = qpsMainLogic.parse(st13);
        std::list<std::string> result14 = qpsMainLogic.parse(st14);
        std::list<std::string> result15 = qpsMainLogic.parse(st15);
        std::list<std::string> result16 = qpsMainLogic.parse(st16);

        REQUIRE(compareStringLists(result1, assigns));
        REQUIRE(compareStringLists(result2, empty));
        REQUIRE(compareStringLists(result3, assigns));
        REQUIRE(compareStringLists(result4, empty));
        REQUIRE(compareStringLists(result5, expectedResult5));
        REQUIRE(compareStringLists(result6, expectedResult6));
        REQUIRE(compareStringLists(result7, expectedResult7));
        REQUIRE(compareStringLists(result8, expectedResult8));
        REQUIRE(compareStringLists(result9, expectedResult9));
        REQUIRE(compareStringLists(result10, expectedResult10));
        REQUIRE(compareStringLists(result11, expectedResult11));
        REQUIRE(compareStringLists(result12, expectedResult12));
        REQUIRE(compareStringLists(result13, expectedResult13));
        REQUIRE(compareStringLists(result14, expectedResult14));
        REQUIRE(compareStringLists(result15, expectedResult15));
        REQUIRE(compareStringLists(result16, expectedResult16));
    }
    
    SECTION("PATTERN CLAUSE") {
        std::string p1 = "assign a; Select a pattern a(_, _\"5\"_)";
        std::string p2 = "assign a; Select a pattern a(_, \"5\")";
        std::string p3 = "assign a; Select a pattern a(\"x\", _)";
        std::string p4 = R"(assign a; Select a pattern a("x", "5"))";
        std::string p5 = R"(assign a; Select a pattern a("z", _"0"_))";
        std::string p6 = "assign a; Select a pattern a (_, _)";
        std::string p7 = "variable v; assign a; Select a pattern a (v, _\"100\"_)";
        std::string p8 = "variable v; assign a; Select v pattern a (v, \"5\")";
        std::string p9 = "print p; assign a; variable v; Select p pattern a (v, \"x\")";
        std::string p10 = "variable v; assign a; Select v pattern a (v, _)";
        std::string p11 = "while w; Select BOOLEAN pattern w(_, _, _)";
        std::string p12 = "if ifs; Select BOOLEAN pattern ifs(_. _)";
        std::string p13 = "while w; Select w pattern w(_, _)";
        std::string p14 = "while w; Select w pattern w(\"x\", _)";
        std::string p15 = "variable v; while w; Select v pattern w(v, _)";
        std::string p16 = "if ifs; Select ifs pattern ifs(_, _, _)";
        std::string p17 = "if ifs; Select ifs pattern ifs(\"z\", _, _)";
        std::string p18 = "if ifs; variable v; Select v pattern ifs(v, _, _)";

        std::list<std::string> expectedResult1 = {"1", "3", "8"};
        std::list<std::string> expectedResult2 = {"1", "3", "8"};
        std::list<std::string> expectedResult3 = {"1", "3", "7"};
        std::list<std::string> expectedResult4 = {"1", "3"};
        std::list<std::string> expectedResult5 = {"10"};
        std::list<std::string> expectedResult6 = {"1", "2", "3", "7", "8", "9", "10"};
        std::list<std::string> expectedResult7 = {"7", "9"};
        std::list<std::string> expectedResult8 = {"x", "z"};
        std::list<std::string> expectedResult9 = {"11", "13"};
        std::list<std::string> expectedResult10 = {"x", "y", "z"};
        std::list<std::string> expectedResult11 = {};
        std::list<std::string> expectedResult12 = {};
        std::list<std::string> expectedResult13 = {"4"};
        std::list<std::string> expectedResult14 = {"4"};
        std::list<std::string> expectedResult15 = {"x"};
        std::list<std::string> expectedResult16 = {"6"};
        std::list<std::string> expectedResult17 = {"6"};
        std::list<std::string> expectedResult18 = {"z"};

        std::list<std::string> result1 = qpsMainLogic.parse(p1);
        std::list<std::string> result2 = qpsMainLogic.parse(p2);
        std::list<std::string> result3 = qpsMainLogic.parse(p3);
        std::list<std::string> result4 = qpsMainLogic.parse(p4);
        std::list<std::string> result5 = qpsMainLogic.parse(p5);
        std::list<std::string> result6 = qpsMainLogic.parse(p6);
        std::list<std::string> result7 = qpsMainLogic.parse(p7);
        std::list<std::string> result8 = qpsMainLogic.parse(p8);
        std::list<std::string> result9 = qpsMainLogic.parse(p9);
        std::list<std::string> result10 = qpsMainLogic.parse(p10);
        std::list<std::string> result11 = qpsMainLogic.parse(p11);
        std::list<std::string> result12 = qpsMainLogic.parse(p12);
        std::list<std::string> result13 = qpsMainLogic.parse(p13);
        std::list<std::string> result14 = qpsMainLogic.parse(p14);
        std::list<std::string> result15 = qpsMainLogic.parse(p15);
        std::list<std::string> result16 = qpsMainLogic.parse(p16);
        std::list<std::string> result17 = qpsMainLogic.parse(p17);
        std::list<std::string> result18 = qpsMainLogic.parse(p18);

        REQUIRE(compareStringLists(result1, expectedResult1));
        REQUIRE(compareStringLists(result2, expectedResult2));
        REQUIRE(compareStringLists(result3, expectedResult3));
        REQUIRE(compareStringLists(result4, expectedResult4));
        REQUIRE(compareStringLists(result5, expectedResult5));
        REQUIRE(compareStringLists(result6, expectedResult6));
        REQUIRE(compareStringLists(result7, expectedResult7));
        REQUIRE(compareStringLists(result8, expectedResult8));
        REQUIRE(compareStringLists(result9, expectedResult9));
        REQUIRE(compareStringLists(result10, expectedResult10));
        REQUIRE(compareStringLists(result11, expectedResult11));
        REQUIRE(compareStringLists(result12, expectedResult12));
        REQUIRE(compareStringLists(result13, expectedResult13));
        REQUIRE(compareStringLists(result14, expectedResult14));
        REQUIRE(compareStringLists(result15, expectedResult15));
        REQUIRE(compareStringLists(result16, expectedResult16));
        REQUIRE(compareStringLists(result17, expectedResult17));
        REQUIRE(compareStringLists(result18, expectedResult18));
    }

    SECTION("WITH CLAUSES") {
        std::string w1 = "read r; Select r with 1 = 1";
        std::string w2 = R"(read r; Select r with "a" = "a" )";
        std::string w3 = "read r; Select r with r.varName = \"z\"";
        std::string w4 = "assign a; Select a with a.stmt# = 3";
        std::string w5 = "constant c; Select c with c.value = 100";
        std::string w6 = "procedure p; Select p with p.procName = \"f\"";
        std::string w7 = "call c; procedure p; Select c with c.procName = p.procName";
        std::string w8 = "stmt s; constant c; Select s with s.stmt# = c.value";
        std::string w9 = "Select BOOLEAN with 12 = 12";
        std::string w10 = "assign a; Select a with 3 = a.stmt#";
        std::string w11 = "constant c; Select c with 100 = c.value";
        std::string w12 = "stmt s; Select s with 1 == 1";
        std::string w13 = "stmt s; Select s with s = s";
        std::string w14 = "stmt s; Select BOOLEAN with s = 2";

        std::list<std::string> expectedResult1 = {"5"};
        std::list<std::string> expectedResult2 = {"5"};
        std::list<std::string> expectedResult3 = {"5"};
        std::list<std::string> expectedResult4 = {"3"};
        std::list<std::string> expectedResult5 = {"100"};
        std::list<std::string> expectedResult6 = {"f"};
        std::list<std::string> expectedResult7 = {"12"};
        std::list<std::string> expectedResult8 = {"4", "5", "10"};
        std::list<std::string> expectedResult9 = {"TRUE"};
        std::list<std::string> expectedResult10 = {"3"};
        std::list<std::string> expectedResult11 = {"100"};
        std::list<std::string> expectedResult12 = {};
        std::list<std::string> expectedResult13 = {};
        std::list<std::string> expectedResult14 = {};

        std::list<std::string> result1 = qpsMainLogic.parse(w1);
        std::list<std::string> result2 = qpsMainLogic.parse(w2);
        std::list<std::string> result3 = qpsMainLogic.parse(w3);
        std::list<std::string> result4 = qpsMainLogic.parse(w4);
        std::list<std::string> result5 = qpsMainLogic.parse(w5);
        std::list<std::string> result6 = qpsMainLogic.parse(w6);
        std::list<std::string> result7 = qpsMainLogic.parse(w7);
        std::list<std::string> result8 = qpsMainLogic.parse(w8);
        std::list<std::string> result9 = qpsMainLogic.parse(w9);
        std::list<std::string> result10 = qpsMainLogic.parse(w10);
        std::list<std::string> result11 = qpsMainLogic.parse(w11);
        std::list<std::string> result12 = qpsMainLogic.parse(w12);
        std::list<std::string> result13 = qpsMainLogic.parse(w13);
        std::list<std::string> result14 = qpsMainLogic.parse(w14);

        REQUIRE(compareStringLists(result1, expectedResult1));
        REQUIRE(compareStringLists(result2, expectedResult2));
        REQUIRE(compareStringLists(result3, expectedResult3));
        REQUIRE(compareStringLists(result4, expectedResult4));
        REQUIRE(compareStringLists(result5, expectedResult5));
        REQUIRE(compareStringLists(result6, expectedResult6));
        REQUIRE(compareStringLists(result7, expectedResult7));
        REQUIRE(compareStringLists(result8, expectedResult8));
        REQUIRE(compareStringLists(result9, expectedResult9));
        REQUIRE(compareStringLists(result10, expectedResult10));
        REQUIRE(compareStringLists(result11, expectedResult11));
        REQUIRE(compareStringLists(result12, expectedResult12));
        REQUIRE(compareStringLists(result13, expectedResult13));
        REQUIRE(compareStringLists(result14, expectedResult14));
    }

    SECTION("COMBINATION CLAUSES") {
        std::string com1 = "if ifs; stmt s; assign a; assign a1; Select a1 such that Parent (ifs, a1) pattern a1 (_, _\"100\"_)";
        std::string com2 = "assign a; assign a1; Select a1 such that Follows (a, a1) pattern a (_, \"100\")";
        std::string com3 = "assign a; assign a1; Select a1 such that Follows (a1, a) pattern a (_, _\"5\"_)";
        std::string com4 = "if ifs; stmt s; assign a; assign a1; Select a such that Follows (a1, s) pattern a1 (_, _\"100\"_)";
        std::string com5 = "constant c; variable v; assign a; assign a1; Select c such that Modifies (a, v) pattern a1 (_, \"15\")";
        std::string com6 = "assign a, a1; variable v; Select a such that Uses (a1, v) pattern a (v, _)";
        std::string com7 = "assign a, a1; variable v, v1; Select v such that Uses (a, v) pattern a1 (v, _)";
        std::string com8 = "assign a, a1; variable v, v1; Select v1 such that Modifies (a, v) pattern a1 (v1, _)";
        std::string com9 = R"(assign a; variable v; Select a such that Modifies (a, "y") pattern a (v, _"100"_))";
        std::string com10 = R"(print p; assign a; variable v; Select a such that Uses (p, "x") pattern a (v, _"x"_))";
        std::string com11 = "print p; stmt s; variable v; Select p.stmt# such that Uses(p, v) and Follows(p, s) with s.stmt# = 12";
        std::string com12 = "assign a, a1; stmt s; Select <a, a1> such that Follows(a, a1) with a.stmt# = s.stmt# pattern a(_, \"5\")";
        std::string com13 = "while w; stmt s; Select w such that Parent*(w, s) with s.stmt# = 5";
        std::string com14 = "assign s; while w; Select BOOLEAN such that Parent(w, s) with s.stmt# = 5 and s.stmt# = 6";
        std::string com15 = "stmt s; while w; Select BOOLEAN such that Parent(w, s) with s.stmt# = 5";
        std::string com16 = R"(call c; procedure p1, p2; Select <p1, p2> such that Calls(p1, p2) with p1.procName = "f" and c.procName = "g")";
        std::string com17 = R"(call c; procedure p1, p2; Select <p1, p2> such that Calls(p1, p2) with p1.procName = "f" and c.procName = "f")";
        std::string com18 = R"(assign a, a1; Select <a.stmt#, a> pattern a ("y", _) such that Affects(a1, a) pattern a(_, "x"))";
        std::string com19 = "constant c; stmt s; Select <c, s, c.value, s.stmt#> with c.value = s.stmt# and s.stmt# = c.value such that Parent(4, s)";
        std::string com20 = "print p; stmt s; constant c; Select BOOLEAN such that Parent(s, p) pattern c(_, _)";

        std::list<std::string> expectedResult1 = {"7", "9"};
        std::list<std::string> expectedResult2 = {"8", "10"};
        std::list<std::string> expectedResult3 = {"2", "7"};
        std::list<std::string> expectedResult4 = {"1", "2", "3", "7", "8", "9", "10"};
        std::list<std::string> expectedResult5 = {};
        std::list<std::string> expectedResult6 = {"1", "3", "7"};
        std::list<std::string> expectedResult7 = {"x"};
        std::list<std::string> expectedResult8 = {"x", "y", "z"};
        std::list<std::string> expectedResult9 = {"9"};
        std::list<std::string> expectedResult10 = {"2"};
        std::list<std::string> expectedResult11 = {"11"};
        std::list<std::string> expectedResult12 = {"1 2"};
        std::list<std::string> expectedResult13 = {"4"};
        std::list<std::string> expectedResult14 = {"FALSE"};
        std::list<std::string> expectedResult15 = {"TRUE"};
        std::list<std::string> expectedResult16 = {"f g"};
        std::list<std::string> expectedResult17 = {};
        std::list<std::string> expectedResult18 = {"2 2"};
        std::list<std::string> expectedResult19 = {"5 5 5 5"};
        std::list<std::string> expectedResult20 = {"FALSE"};

        std::list<std::string> result1 = qpsMainLogic.parse(com1);
        std::list<std::string> result2 = qpsMainLogic.parse(com2);
        std::list<std::string> result3 = qpsMainLogic.parse(com3);
        std::list<std::string> result4 = qpsMainLogic.parse(com4);
        std::list<std::string> result5 = qpsMainLogic.parse(com5);
        std::list<std::string> result6 = qpsMainLogic.parse(com6);
        std::list<std::string> result7 = qpsMainLogic.parse(com7);
        std::list<std::string> result8 = qpsMainLogic.parse(com8);
        std::list<std::string> result9 = qpsMainLogic.parse(com9);
        std::list<std::string> result10 = qpsMainLogic.parse(com10);
        std::list<std::string> result11 = qpsMainLogic.parse(com11);
        std::list<std::string> result12 = qpsMainLogic.parse(com12);
        std::list<std::string> result13 = qpsMainLogic.parse(com13);
        std::list<std::string> result14 = qpsMainLogic.parse(com14);
        std::list<std::string> result15 = qpsMainLogic.parse(com15);
        std::list<std::string> result16 = qpsMainLogic.parse(com16);
        std::list<std::string> result17 = qpsMainLogic.parse(com17);
        std::list<std::string> result18 = qpsMainLogic.parse(com18);
        std::list<std::string> result19 = qpsMainLogic.parse(com19);
        std::list<std::string> result20 = qpsMainLogic.parse(com20);

        REQUIRE(compareStringLists(result1, expectedResult1));
        REQUIRE(compareStringLists(result2, expectedResult2));
        REQUIRE(compareStringLists(result3, expectedResult3));
        REQUIRE(compareStringLists(result4, expectedResult4));
        REQUIRE(compareStringLists(result5, expectedResult5));
        REQUIRE(compareStringLists(result6, expectedResult6));
        REQUIRE(compareStringLists(result7, expectedResult7));
        REQUIRE(compareStringLists(result8, expectedResult8));
        REQUIRE(compareStringLists(result9, expectedResult9));
        REQUIRE(compareStringLists(result10, expectedResult10));
        REQUIRE(compareStringLists(result11, expectedResult11));
        REQUIRE(compareStringLists(result12, expectedResult12));
        REQUIRE(compareStringLists(result13, expectedResult13));
        REQUIRE(compareStringLists(result14, expectedResult14));
        REQUIRE(compareStringLists(result15, expectedResult15));
        REQUIRE(compareStringLists(result16, expectedResult16));
        REQUIRE(compareStringLists(result17, expectedResult17));
        REQUIRE(compareStringLists(result18, expectedResult18));
        REQUIRE(compareStringLists(result19, expectedResult19));
        REQUIRE(compareStringLists(result20, expectedResult20));
    }
}
