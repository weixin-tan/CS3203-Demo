#include <vector>

#include "catch.hpp"
#include "QPS/Clause.h"
#include "QPS/WithHandler.h"
#include "PKBGetterStubForWithHandler.h"

using namespace std;

// Source for testing
//procedure f {
//  x = 5;  // 1
//  y = x;  // 2
//  x = 5;  // 3
//  while (x > 4) {  // 4
//    read x;  // 5
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
//procedure g{
// }

// x, y, z

TEST_CASE("Testing Both Side Fixed") {
    Entity fixedInt1 = Entity(EntityType::FIXED_INTEGER, "1");
    Entity fixedInt2 = Entity(EntityType::FIXED_INTEGER, "2");

    Entity fixedStringX = Entity(EntityType::FIXED_STRING, "x");
    Entity fixedStringX_2 = Entity(EntityType::FIXED_STRING, "x");
    Entity fixedStringY = Entity(EntityType::FIXED_STRING, "y");

    DB db;
    PkbGetter* pg = new PKBGetterStubForWithHandler(&db);
    WithHandler* wh = new WithHandler(pg);

    RelationshipRef r1 = RelationshipRef(RelationshipType::WITH, fixedInt1, fixedInt1);
    RelationshipRef r2 = RelationshipRef(RelationshipType::WITH, fixedStringX, fixedStringX_2);
    RelationshipRef r3 = RelationshipRef(RelationshipType::WITH, fixedInt1, fixedInt2);
    RelationshipRef r4 = RelationshipRef(RelationshipType::WITH, fixedStringX, fixedStringY);

    Result result1 = wh->handleWith(r1);
    Result result2 = wh->handleWith(r2);
    Result result3 = wh->handleWith(r3);
    Result result4 = wh->handleWith(r4);

    SECTION("result is valid") {
        REQUIRE(result1.getValid());
        REQUIRE(result2.getValid());
    }

    SECTION("result is invalid") {
        REQUIRE(!result3.getValid());
        REQUIRE(!result4.getValid());
    }
}

TEST_CASE("1 side fixed") {
    Entity varSyn = Entity(EntityType::VARIABLE, "v", EntityAttributeType::VARNAME);
    Entity procSyn1 = Entity(EntityType::PROCEDURE, "p1");

    DB db;
    PkbGetter* pg = new PKBGetterStubForWithHandler(&db);
    WithHandler* wh = new WithHandler(pg);

    Entity fixedStringX = Entity(EntityType::FIXED_STRING, "x");
    Entity fixedStringW = Entity(EntityType::FIXED_STRING, "w");
    Entity fixedStringF = Entity(EntityType::FIXED_STRING, "f");

    RelationshipRef r1 = RelationshipRef(RelationshipType::WITH, varSyn, fixedStringX);
    RelationshipRef r2 = RelationshipRef(RelationshipType::WITH, varSyn, fixedStringW);
    RelationshipRef r3 = RelationshipRef(RelationshipType::WITH, procSyn1, fixedStringF);
    RelationshipRef r4 = RelationshipRef(RelationshipType::WITH, procSyn1, fixedStringW);

    Result result1 = wh->handleWith(r1);
    Result result2 = wh->handleWith(r2);
    Result result3 = wh->handleWith(r3);
    Result result4 = wh->handleWith(r4);

    Result expectedResult1;
    expectedResult1.setResultType(ResultType::WITH_CLAUSE);
    expectedResult1.setValid(true);
    expectedResult1.setOneSynEntity(varSyn);
    std::set<ProgramElement> result1Elements;
    result1Elements.insert(ProgramElement::createVariable("x"));
    expectedResult1.setOneSynSet(result1Elements);

    Result expectedResult3;
    expectedResult3.setResultType(ResultType::WITH_CLAUSE);
    expectedResult3.setValid(true);
    expectedResult3.setOneSynEntity(procSyn1);
    std::set<ProgramElement> result3Elements;
    result3Elements.insert(ProgramElement::createProcedure("f"));
    expectedResult3.setOneSynSet(result3Elements);

    SECTION("result is valid") {
        REQUIRE(result1 == expectedResult1);
        REQUIRE(result3 == expectedResult3);
    }

    SECTION("result is invalid") {
        REQUIRE((!result2.getValid() && result2.getOneSynSet().empty()));
        REQUIRE((!result4.getValid() && result4.getOneSynSet().empty()));
    }
}

/*
    Entity procSyn1 = Entity(EntityType::PROCEDURE, "p1");
    Entity procSyn2 = Entity(EntityType::PROCEDURE, "p2");

    Entity varSyn = Entity(EntityType::VARIABLE, "v", EntityAttributeType::VARNAME);
    Entity constSyn = Entity(EntityType::CONSTANT, "c");

    Entity stmtSyn = Entity(EntityType::STATEMENT, "s");
    Entity printSyn = Entity(EntityType::PRINT, "p");
    Entity readSyn = Entity(EntityType::READ, "r");
    Entity assignSyn = Entity(EntityType::ASSIGNMENT, "a");
    Entity callSyn = Entity(EntityType::CALL, "call");
    Entity whileSyn = Entity(EntityType::WHILE, "w");
    Entity ifSyn = Entity(EntityType::IF, "ifs");
 */

TEST_CASE("test no sides fixed") {
    DB db;
    PkbGetter* pg = new PKBGetterStubForWithHandler(&db);
    WithHandler* wh = new WithHandler(pg);

    Entity stmtSyn = Entity(EntityType::STATEMENT, "s", EntityAttributeType::STMT);
    Entity printSyn = Entity(EntityType::PRINT, "p", EntityAttributeType::STMT);
    Entity printSynVar = Entity(EntityType::PRINT, "p", EntityAttributeType::VARNAME);
    Entity readSyn = Entity(EntityType::READ, "r", EntityAttributeType::STMT);
    Entity readSynVar = Entity(EntityType::READ, "r", EntityAttributeType::VARNAME);

    RelationshipRef r1 = RelationshipRef(RelationshipType::WITH, stmtSyn, printSyn);
    Result result1 = wh->handleWith(r1);
    Result expectedResult1;
    expectedResult1.setResultType(ResultType::WITH_CLAUSE);
    expectedResult1.setValid(true);
    expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(stmtSyn, printSyn));
    std::set<std::pair<ProgramElement, ProgramElement>> result1Elements;
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::PRINT,
                                                                                                     11,
                                                                                                     "x"),
                                                                     ProgramElement::createStatement(ElementType::PRINT,
                                                                                                     11,
                                                                                                     "x")));
    expectedResult1.setTwoSynSet(result1Elements);

    RelationshipRef r2 = RelationshipRef(RelationshipType::WITH, readSynVar, printSynVar);
    Result result2 = wh->handleWith(r2);
    Result expectedResult2;
    expectedResult2.setResultType(ResultType::WITH_CLAUSE);
    expectedResult2.setValid(true);
    expectedResult2.setTwoSynEntities(std::pair<Entity, Entity>(readSynVar, printSynVar));
    std::set<std::pair<ProgramElement, ProgramElement>> result2Elements;
    result2Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::READ,
                                                                                                     5,
                                                                                                     "x"),
                                                                     ProgramElement::createStatement(ElementType::PRINT,
                                                                                                     11,
                                                                                                     "x")));
    expectedResult2.setTwoSynSet(result2Elements);

    RelationshipRef r3 = RelationshipRef(RelationshipType::WITH, readSyn, printSyn);
    Result result3 = wh->handleWith(r3);
    Result expectedResult3;
    expectedResult3.setResultType(ResultType::WITH_CLAUSE);
    expectedResult3.setValid(false);
    expectedResult3.setTwoSynEntities(std::pair<Entity, Entity>(readSyn, printSyn));

    SECTION("result is valid") {
        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResult2);
    }

    SECTION("result is invalid") {
        REQUIRE(result3 == expectedResult3);
    }
}