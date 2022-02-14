#include "ParsedStatement.h"
#include "StatementType.h"
#include "PKB.h"

#include "catch.hpp"
using namespace std;


TEST_CASE("PKB Basic") {
  PKB pkb = PKB();
  PkbGetter* pkb_getter = pkb.getGetter();
  PkbSetter* pkb_setter = pkb.getSetter();

  REQUIRE(pkb_getter != NULL);
  REQUIRE(pkb_setter != NULL);

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
    ParsedStatement(1, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"}, {},  ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
    ParsedStatement(2, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {"x"}, {"y"},{}, ParsedStatement::default_procedure_name, 1),
    ParsedStatement(3, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"},{}, ParsedStatement::default_procedure_name, 2),
    ParsedStatement(4, ParsedStatement::default_null_stmt_no, ParsedStatement::default_null_stmt_no, StatementType::kwhile_stmt, ParsedStatement::default_pattern, "f", {"x"}, {},{}, ParsedStatement::default_procedure_name, 3),
    ParsedStatement(5, ParsedStatement::default_null_stmt_no, 4, StatementType::kread_stmt, ParsedStatement::default_pattern, "f", {}, {"z"}, {},ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
    ParsedStatement(6, ParsedStatement::default_null_stmt_no, 4, StatementType::kif_stmt, ParsedStatement::default_pattern, "f", {"z"}, {},{}, ParsedStatement::default_procedure_name, 5),
    ParsedStatement(7, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"x"},{}, ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
    ParsedStatement(8, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"z"},{}, ParsedStatement::default_procedure_name, 7),
    ParsedStatement(9, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"y"},{}, ParsedStatement::default_procedure_name, ParsedStatement::default_null_stmt_no),
    ParsedStatement(10, 6, ParsedStatement::default_null_stmt_no, StatementType::kassign_stmt, ParsedStatement::default_pattern, "f", {}, {"z"},{}, ParsedStatement::default_procedure_name, 9),
  };

  for (const ParsedStatement& parsed_statement : statements)
    pkb_setter->insertStmt(parsed_statement);


  SECTION("Modifies") {
    SECTION("isRelationship") {
      REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::Assignment, 1), ProgramElement::createVariable("x"))
                  == true);
      REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::Assignment, 2), ProgramElement::createVariable("y"))
                  == true);
      REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::While, 4), ProgramElement::createVariable("y"))
                  == true);
      REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::If, 6), ProgramElement::createVariable("y"))
                  == true);
    }
    SECTION("getEntity") {
      SECTION("Statements") {
          std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::Statement);
          std::set<ProgramElement> expected = {
                  ProgramElement::createStatement(ElementType::Statement, 1),
                  ProgramElement::createStatement(ElementType::Statement, 2),
                  ProgramElement::createStatement(ElementType::Statement, 3),
                  ProgramElement::createStatement(ElementType::Statement, 4),
                  ProgramElement::createStatement(ElementType::Statement, 5),
                  ProgramElement::createStatement(ElementType::Statement, 6),
                  ProgramElement::createStatement(ElementType::Statement, 7),
                  ProgramElement::createStatement(ElementType::Statement, 8),
                  ProgramElement::createStatement(ElementType::Statement, 9),
                  ProgramElement::createStatement(ElementType::Statement, 10),
          };
          REQUIRE(result == expected);
      }
      SECTION("Variables") {
        std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::Variable);
        std::set<ProgramElement> expected = {
                ProgramElement::createVariable("x"),
                ProgramElement::createVariable("y"),
                ProgramElement::createVariable("z"),
            };

        REQUIRE(result == expected);
      }
      SECTION("Assignments") {
        std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::Assignment);
        std::set<ProgramElement> expected = {
                ProgramElement::createStatement(ElementType::Assignment, 1),
                ProgramElement::createStatement(ElementType::Assignment, 2),
                ProgramElement::createStatement(ElementType::Assignment, 3),
                ProgramElement::createStatement(ElementType::Assignment, 7),
                ProgramElement::createStatement(ElementType::Assignment, 8),
                ProgramElement::createStatement(ElementType::Assignment, 9),
                ProgramElement::createStatement(ElementType::Assignment, 10),
            };

        REQUIRE(result == expected);
      }
      SECTION("Ifs") {
        std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::If);
        std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::If, 6)};

        REQUIRE(result == expected);
      }
      SECTION("Whiles") {
        std::set<ProgramElement> result = pkb_getter->getEntity(ElementType::While);
        std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::While, 4)};

        REQUIRE(result == expected);
      }
    }
    SECTION("getLeftSide") {
      SECTION("Assignment given Variable") {
        std::set<ProgramElement> result = pkb_getter->getLeftSide(RelationshipType::Modifies, ProgramElement::createVariable("x"), ElementType::Assignment);

        std::set<ProgramElement> expected = {
                ProgramElement::createStatement(ElementType::Assignment, 1),
                ProgramElement::createStatement(ElementType::Assignment, 3),
                ProgramElement::createStatement(ElementType::Assignment, 7),
                };

        REQUIRE(result == expected);
      }
      SECTION("If given Variable") {
        std::set<ProgramElement> result =
            pkb_getter->getLeftSide(RelationshipType::Modifies, ProgramElement::createVariable("x"), ElementType::If);

        std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::If, 6)};
        REQUIRE(result == expected);
      }
      SECTION("While given Variable") {
        std::set<ProgramElement> result =
            pkb_getter->getLeftSide(RelationshipType::Modifies, ProgramElement::createVariable("x"), ElementType::While);

        std::set<ProgramElement> expected = {ProgramElement::createStatement(ElementType::While, 4)};
        REQUIRE(result == expected);
      }
    }
    SECTION("getRightSide") {
      SECTION("Variables given Assignment") {
        std::set<ProgramElement> result = pkb_getter->getRightSide(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::Assignment, 1), ElementType::Variable);
        std::set<ProgramElement> expected = {ProgramElement::createVariable("x")};

        REQUIRE(result == expected);

        result = pkb_getter->getRightSide(RelationshipType::Modifies, ProgramElement::createStatement(ElementType::Assignment, 2), ElementType::Variable);
        expected = {ProgramElement::createVariable("y")};
        REQUIRE(result == expected);
      }
    }
  }
}