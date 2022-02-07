#include "ParsedStatement.h"
#include "StatementType.h"
#include "PKB.h"

#include "catch.hpp"
using namespace std;

#define NULL_STMT_NO (-1)


std::pair<EntityType, std::string> entityToPair(const Entity& e) {
  return {e.eType, e.name};
}
std::set<std::pair<EntityType, std::string>> entityVecToSet(const std::vector<Entity>& entityVec) {
  std::set<std::pair<EntityType, std::string>> entityPairSet;
  for (const Entity& e : entityVec) entityPairSet.insert(entityToPair(e));
  return entityPairSet;
}

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
      REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies, Entity(Assignment, "1"), Entity(Variable, "x"))
                  == true);
      REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies, Entity(Assignment, "2"), Entity(Variable, "y"))
                  == true);
      // TODO: Current PKB does not search recursively
      REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies, Entity(While, "4"), Entity(Variable, "y"))
                  == true);
      REQUIRE(pkb_getter->isRelationship(RelationshipType::Modifies, Entity(If, "6"), Entity(Variable, "y"))
                  == true);
    }
    SECTION("getEntity") {  // TODO: this does not particularly tie to Modifies, maybe start a General SECTION
      SECTION("Variables") {
        std::set<std::pair<EntityType, std::string>> result = entityVecToSet(pkb_getter->getEntity(EntityType::Variable));
        std::set<std::pair<EntityType, std::string>> expected =
            entityVecToSet({Entity(EntityType::Variable, "x"),
                            Entity(EntityType::Variable, "y"),
                            Entity(EntityType::Variable, "z"),
                            });

        REQUIRE(result == expected);
      }
      SECTION("Assignments") {
        std::set<std::pair<EntityType, std::string>> result = entityVecToSet(pkb_getter->getEntity(EntityType::Assignment));
        std::set<std::pair<EntityType, std::string>> expected =
            entityVecToSet({Entity(EntityType::Assignment, "1"),
                            Entity(EntityType::Assignment, "2"),
                            Entity(EntityType::Assignment, "3"),
                            Entity(EntityType::Assignment, "7"),
                            Entity(EntityType::Assignment, "8"),
                            Entity(EntityType::Assignment, "9"),
                            Entity(EntityType::Assignment, "10"),
                            });

        REQUIRE(result == expected);
      }
      SECTION("Ifs") {
        std::set<std::pair<EntityType, std::string>> result = entityVecToSet(pkb_getter->getEntity(EntityType::If));
        std::set<std::pair<EntityType, std::string>> expected =
            entityVecToSet({Entity(EntityType::If, "6"),});

        REQUIRE(result == expected);
      }
      SECTION("Whiles") {
        std::set<std::pair<EntityType, std::string>> result = entityVecToSet(pkb_getter->getEntity(EntityType::While));
        std::set<std::pair<EntityType, std::string>> expected =
            entityVecToSet({Entity(EntityType::While, "4"),});

        REQUIRE(result == expected);
      }
    }
    SECTION("getLeftSide") {
      SECTION("Assignment given Variable") {
        std::set<std::pair<EntityType, std::string>> result =
            entityVecToSet(pkb_getter->getLeftSide(RelationshipType::Modifies, Entity(Variable, "x"), EntityType::Assignment));

        std::set<std::pair<EntityType, std::string>> expected = entityVecToSet({
                                                                                   Entity(EntityType::Assignment, "1"),
                                                                                   Entity(EntityType::Assignment, "3"),
                                                                                   Entity(EntityType::Assignment, "7"),
                                                                               });

        REQUIRE(result == expected);
      }
      SECTION("If given Variable") {
        std::set<std::pair<EntityType, std::string>> result =
            entityVecToSet(pkb_getter->getLeftSide(RelationshipType::Modifies, Entity(Variable, "x"), EntityType::If));

        std::set<std::pair<EntityType, std::string>> expected = entityVecToSet({
                                                                                   Entity(EntityType::If, "6"),
                                                                               });
        // TODO
        REQUIRE(result == expected);
      }
      SECTION("While given Variable") {
        std::set<std::pair<EntityType, std::string>> result =
            entityVecToSet(pkb_getter->getLeftSide(RelationshipType::Modifies, Entity(Variable, "x"), EntityType::While));

        std::set<std::pair<EntityType, std::string>> expected = entityVecToSet({
                                                                                   Entity(EntityType::While, "4"),
                                                                               });
        // TODO
        REQUIRE(result == expected);
      }
    }
    SECTION("getRightSide") {
      SECTION("Variables given Assignment") {
        std::set<std::pair<EntityType, std::string>> result;
        std::set<std::pair<EntityType, std::string>> expected;

        result = entityVecToSet(pkb_getter->getRightSide(RelationshipType::Modifies, Entity(Assignment, "1"), EntityType::Variable));
        expected = entityVecToSet({Entity(EntityType::Variable, "x")});
        REQUIRE(result == expected);

        result = entityVecToSet(pkb_getter->getRightSide(RelationshipType::Modifies, Entity(Assignment, "2"), EntityType::Variable));
        expected = entityVecToSet({Entity(EntityType::Variable, "y")});
        REQUIRE(result == expected);
      }
    }
  }
}
