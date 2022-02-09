#include "catch.hpp"
#include "PKB.h"
#include "SuchThatHandler.h"
#include "QueryProcessor.h"
#include "QPSMainLogic.h"

#define NULL_STMT_NO (-1)

TEST_CASE("QPS Handler") {
    PKB pkb = PKB();
    QPSMainLogic* qr = QPSMainLogic::getInstance(pkb.getGetter());
    SuchThatHandler st = SuchThatHandler(pkb.getGetter());
    QueryProcessor qp = QueryProcessor();


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

    std::vector<ParsedStatement> statements = {
            {1, NULL_STMT_NO, NULL_STMT_NO, StatementType::kassign_stmt, "", "f", {}, {"x"}, "", NULL_STMT_NO},
            {2, NULL_STMT_NO, NULL_STMT_NO, StatementType::kassign_stmt, "", "f", {"x"}, {"y"}, "", 1},
            {3, NULL_STMT_NO, NULL_STMT_NO, StatementType::kassign_stmt, "", "f", {}, {"x"}, "", 2},
            {4, NULL_STMT_NO, NULL_STMT_NO, StatementType::kwhile_stmt, "", "f", {"x"}, {}, "", 3},
            {5, NULL_STMT_NO, 4, StatementType::kread_stmt, "", "f", {}, {"z"}, "", NULL_STMT_NO},
            {6, NULL_STMT_NO, 4, StatementType::kif_stmt, "", "f", {"z"}, {}, "", 5},
            {7, 6, NULL_STMT_NO, StatementType::kassign_stmt, "", "f", {}, {"x"}, "", NULL_STMT_NO},
            {8, 6, NULL_STMT_NO, StatementType::kassign_stmt, "", "f", {}, {"z"}, "", 7},
            {9, 6, NULL_STMT_NO, StatementType::kassign_stmt, "", "f", {}, {"y"}, "", NULL_STMT_NO},
            {10, 6, NULL_STMT_NO, StatementType::kassign_stmt, "", "f", {}, {"z"}, "", 9},
    };
    for (const ParsedStatement& parsed_statement : statements)
        pkb.getSetter()->insertStmt(parsed_statement);

    SECTION("HANDLER") {
        std::string one = "assign a;\n Select a";
        std::string two = "assign a;\n Select a such that Modifies (1, \"x\")";
        std::string three = "assign a;\n Select a such that Modifies (a, \"x\")";
        std::string four = "variable v;\n Select v such that Modifies (2, v)";

        /*
        std::list<std::string> s1 =qr->parse(one);
        for (std::string s : s1) {
            cout << "one: " << s << "\n";
        }

        std::list<std::string> s2 = qr->parse(two);
        for (std::string s : s2) {
            cout << "two: " << s << "\n";
        }

        std::list<std::string> s3 = qr->parse(three);
        for (std::string s : s3) {
            cout << "three: " << s << "\n";
        }
        */

        std::list<std::string> s4 = qr->parse(four);
        for (std::string s : s4) {
            cout << "four: " << s << "\n";
        }
        /*
        std::vector<Clause> c1 = qp.parsePQL(one);
        for (Clause c : c1) {
            cout << c.toString() << "\n";
        }

        std::vector<Result> r = st.processClause(c1);
        cout << r.size() << '\n';
//        for (Result r1 : r) {
//
//        }
        */

        Clause c2;



    }


}
