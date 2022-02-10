#include "catch.hpp"
#include "PKB.h"
#include "SuchThatHandler.h"
#include "QueryProcessor.h"
#include "QPSMainLogic.h"

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
      ParsedStatement(1, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"}, ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
      ParsedStatement(2, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {"x"}, {"y"}, ParsedStatement::default_procedure_name, 1),
      ParsedStatement(3, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"}, ParsedStatement::default_procedure_name, 2),
      ParsedStatement(4, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kwhile_stmt, ParsedStatement::default_pattern, "f", {"x"}, {}, ParsedStatement::default_procedure_name, 3),
      ParsedStatement(5, ParsedStatement::default_null_stmt_no, 4, StatementType::kread_stmt, ParsedStatement::default_pattern, "f", {}, {"z"}, ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
      ParsedStatement(6, ParsedStatement::default_null_stmt_no, 4, StatementType::kif_stmt, ParsedStatement::default_pattern, "f", {"z"}, {}, ParsedStatement::default_procedure_name, 5),
      ParsedStatement(7, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"}, ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
      ParsedStatement(8, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"z"}, ParsedStatement::default_procedure_name, 7),
      ParsedStatement(9, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"y"}, ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
      ParsedStatement(10, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"z"}, ParsedStatement::default_procedure_name, 9),
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
