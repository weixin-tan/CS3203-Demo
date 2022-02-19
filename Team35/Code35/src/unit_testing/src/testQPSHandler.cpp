#include "catch.hpp"
#include "PKB.h"
#include "QueryProcessor.h"
#include "QPSMainLogic.h"
#include "EntityToElementConverter.h"

TEST_CASE("QPS Handler") {
    PKB pkb = PKB();
    QPSMainLogic* qr = QPSMainLogic::getInstance(pkb.getGetter());
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
//  print x; // 11
//}

    std::vector<ParsedStatement> statements = {
            ParsedStatement(1, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"}, {"5"},  ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
            ParsedStatement(2, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {"x"}, {"y"}, {}, ParsedStatement::default_procedure_name, 1),
            ParsedStatement(3, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"}, {"5"}, ParsedStatement::default_procedure_name, 2),
            ParsedStatement(4, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kwhile_stmt, ParsedStatement::default_pattern, "f", {"x"}, {}, {"4"}, ParsedStatement::default_procedure_name, 3),
            ParsedStatement(5, ParsedStatement::default_null_stmt_no, 4, StatementType::kread_stmt, ParsedStatement::default_pattern, "f", {}, {"z"}, {},ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
            ParsedStatement(6, ParsedStatement::default_null_stmt_no, 4, StatementType::kif_stmt, ParsedStatement::default_pattern, "f", {"z"}, {}, {"10"}, ParsedStatement::default_procedure_name, 5),
            ParsedStatement(7, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"}, {"100"}, ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
            ParsedStatement(8, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"z"}, {"5"}, ParsedStatement::default_procedure_name, 7),
            ParsedStatement(9, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"y"}, {"100"}, ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
            ParsedStatement(10, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"z"}, {"0"}, ParsedStatement::default_procedure_name, 9),
            ParsedStatement(11, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kread_stmt, ParsedStatement::default_pattern, "f", {"x"}, {}, {}, ParsedStatement::default_procedure_name, 4),
    };

    for (const ParsedStatement& parsed_statement : statements)
        pkb.getSetter()->insertStmt(parsed_statement);
    for (const ParsedStatement& parsed_statement : statements)
        pkb.getSetter()->insertStmt(parsed_statement);
    SECTION("HANDLER") {
        std::string noneOne = "assign a;\n Select a";
        std::string noneTwo = "stmt s;\n Select s";
        std::string noneThree = "variable v;\n Select v";
        std::string noneFour = "if i;\n Select i";
        std::string noneFive = "while w;\n Select w";
        std::string noneSix = "read r;\n Select r";
        std::string noneSeven = "print p;\n Select p";
        std::string noneEight = "constant c;\n Select c";
        std::string noneNine = "procedure p;\n Select p";

        std::string modifiesOne = "assign a;\n Select a such that Modifies (a, \"x\")";
        std::string modifiesTwo = "read r;\n Select r such that Modifies (r, \"x\")";
        std::string modifiesThree = "if i;\n Select i such that Modifies (i, \"x\")";
        std::string modifiesFour = "while w;\n Select w such that Modifies (w, \"x\")";
        std::string modifiesFive = "procedure p;\n Select p such that Modifies (p, \"x\")";
        std::string modifiesSix = "assign a;\n Select a such that Modifies (a, \"z\")";
        std::string modifiesSeven = "stmt s;\n Select s such that Modifies (s, \"z\")";
        std::string modifiesEight = "assign a;\n Select a such that Modifies (1, \"x\")";
        std::string modifiesNine = "assign a;\n Select a such that Modifies (a, _)";
        std::string modifiesTen = "variable v;\n Select v such that Modifies (1, v)";
        std::string modifiesInvalidOne = "assign a;\n Select a such that Modifies (_, \"x\")";
        std::string modifiesInvalidTwo = "assign a;\n Select a such that Modifies (1, a)";
        std::string modifiesInvalidThree = "print p;\n Select p such that Modifies (p, _)";

        std::string usesOne = "assign a;\n Select a such that Uses (a, \"x\")";
        std::string usesTwo = "print p;\n Select p such that Uses (p, \"x\")";
        std::string usesThree = "if i;\n Select i such that Uses (i, \"x\")";
        std::string usesFour = "while w;\n Select w such that Uses (w, \"x\")";
        std::string usesFive = "procedure p;\n Select p such that Uses (p, \"x\")";
        std::string usesSix = "stmt s;\n Select s such that Uses (s, \"x\")";
        std::string usesSeven = "assign a;\n Select a such that Uses (2, \"x\")";
        std::string usesEight = "assign a;\n Select a such that Uses (a, _)";
        std::string usesNine = "variable v;\n Select v such that Uses (2, v)";
        std::string usesInvalidOne = "assign a;\n Select a such that Uses (_, \"x\")";
        std::string usesInvalidTwo = "assign a;\n Select a such that Uses (1, a)";

        std::string followsOne = "assign a;\n Select a such that Follows (a, _)";
        std::string followsTwo = "assign a;\n Select a such that Follows (_, a)";


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

        //vector<Clause> c = qp.parsePQL(modifiesOne);



        std::string t = "assign a; variable v;\n Select v such that Follows (a, _)";
        std::string t1 = "stmt s; Select s";
        std::list<std::string> s4 = qr->parse(t1);
        cout << "asd";
        for (std::string s : s4) {
          cout << "four: " << s << "\n";
        }


        REQUIRE(1 == 1);
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


        Clause c2;

        Entity e1;
        e1.eType = EntityType::Assignment;
        e1.name = "1";

        Entity e2;
        e2.eType = EntityType::Assignment;
        e2.name = "2";

        Entity e3;
        e3.eType = EntityType::Assignment;
        e3.name = "3";

        Entity e4;
        e4.eType = EntityType::Assignment;
        e4.name = "4";

        std::vector<Entity> r1;
        std::vector<Entity> r2;
        r1.push_back(e1);
        r1.push_back(e2);
        r1.push_back(e3);
        r2.push_back(e2);
        r2.push_back(e3);
        r2.push_back(e4);

        Result rez1 = Result(r1);
        Result rez2 = Result(r2);

        std::vector<Result> rezList;
        rezList.push_back(rez1);
        rezList.push_back(rez2);


        ResultProcessor rp = ResultProcessor();
        Result fin = rp.processResults(rezList);

        cout << "start:\n";
        for (Entity e: fin.getResultEntities()) {
            cout << e.toString() << "1\n";
        }
        cout << fin.getResultEntities().size();
            */

    }


}
