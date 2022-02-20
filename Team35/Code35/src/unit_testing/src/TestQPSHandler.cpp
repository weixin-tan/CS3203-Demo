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
  ParsedStatement(11, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kprint_stmt, ParsedStatement::default_pattern, "f", {"x"}, {}, {}, ParsedStatement::default_procedure_name, 4),
};


TEST_CASE("QPS Handler") {
    for (const ParsedStatement& parsed_statement : statements)
        pkb.getSetter()->insertStmt(parsed_statement);

    SECTION("HANDLER AND FORMATTER") {
        std::set<ProgramElement> variables;
        std::set<ProgramElement> assignments;
        ProgramElement x = ProgramElement::createVariable("x");
        ProgramElement y = ProgramElement::createVariable("y");
        ProgramElement z = ProgramElement::createVariable("z");
        variables.insert(x);
        variables.insert(y);
        variables.insert(z);


        std::string r1 = "variable v; Select v";
        std::string r2 = "assign a; Select a such that Modifies(a, _)";
        std::string r3 = "assign a; while w; Select a such that Parent*(w, a)";
        std::string r4 = "if ifs; print p;\nSelect p such that Parent (4, ifs)";
        std::string r5 = "assign a; assign a1;\n Select a such that Follows (a1, a)";
        std::string r6 = "if ifs; stmt s; assign a; assign a1; Select a such that Parent (ifs, a1) pattern a1 (_, _\"100\"_)";
        std::string r7 = "constant c; Select c";
        std::string r8 = "assign a; assign a1; Select a1 such that Follows (a, a1) pattern a (_, _\"100\"_)";
        std::string r9 = "assign a; assign a1; Select a1 such that Follows (a1, a) pattern a (_, _\"5\"_)";
        std::string r10 = "if ifs; stmt s; assign a; assign a1; Select a such that Follows (a1, s) pattern a1 (_, _\"100\"_)";
        std::string r11 = "if ifs; stmt s; assign a; assign a1; Select a such that Follows (s, a1) pattern a1 (_, _\"5\"_)";
        std::string r12 = "stmt s; Select s such that Parent(_, _)";

        std::set<ProgramElement> s1 = rp.processResults(qh.processClause(qp.parsePQL(r1)));
        REQUIRE(s1 == variables);

        std::list<std::string> s2 = qr->parse(r2);
        for (std::string s : s2) {
            cout << "two: " << s << "\n";
        }
        std::list<std::string> s3 = qr->parse(r3);
        for (std::string s : s3) {
            cout << "three: " << s << "\n";
        }
        std::list<std::string> s4 = qr->parse(r4);
        for (std::string s : s4) {
            cout << "four: " << s << "\n";
        }
        std::list<std::string> s5 = qr->parse(r5);
        for (std::string s : s5) {
            cout << "five: " << s << "\n";
        }
        std::list<std::string> s6 = qr->parse(r6);
        for (std::string s : s6) {
            cout << "six: " << s << "\n";
        }
        std::list<std::string> s7 = qr->parse(r7);
        for (std::string s : s7) {
            cout << "seven: " << s << "\n";
        }
        std::list<std::string> s8 = qr->parse(r8);
        for (std::string s : s8) {
            cout << "eight: " << s << "\n";
        }
        std::list<std::string> s9 = qr->parse(r9);
        for (std::string s : s9) {
            cout << "nine: " << s << "\n";
        }
        std::list<std::string> s10 = qr->parse(r10);
        for (std::string s : s10) {
            cout << "ten: " << s << "\n";
        }
        std::list<std::string> s11 = qr->parse(r11);
        for (std::string s : s11) {
            cout << "eleven: " << s << "\n";
        }
        std::list<std::string> s12 = qr->parse(r12);
        for (std::string s : s12) {
            cout << "twelve: " << s << "\n";
        }

    }
}
