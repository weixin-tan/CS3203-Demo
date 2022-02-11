#include "ParsedStatement.h"
#include "StatementType.h"
#include "PKB.h"

#include "catch.hpp"
using namespace std;

#define NULL_STMT_NO (-1)


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