#include <vector>

#include "catch.hpp"
#include "QPS/Clause.h"
#include "QPS/WithHandler.h"
#include "PkbGetterStub.h"
#include "QPSUtil.h"

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

QPS_GENERAL_DATA_INFO entInfoW;
QPS_SOURCE_INFO sourceInfoW;

Result scrubResult(Result r) {
    if (r.getTwoSynSet().empty()) {
        Entity e = r.getOneSynEntity();
        e.aType = EntityAttributeType::NULL_ATTRIBUTE;
        r.setOneSynEntity(e);
    } else {
        std::pair<Entity, Entity> e = r.getTwoSynEntities();
        e.first.aType = EntityAttributeType::NULL_ATTRIBUTE;
        e.second.aType = EntityAttributeType::NULL_ATTRIBUTE;
        r.setTwoSynEntities(e);
    }
    return r;
}

TEST_CASE("Testing Both Side Fixed") {
    DB db;
    PkbGetter* pg = new PkbGetterStub(&db);
    WithHandler* wh = new WithHandler(pg);

    RelationshipRef r1 = RelationshipRef(RelationshipType::WITH, entInfoW.fixedInt1, entInfoW.fixedInt1);
    RelationshipRef r2 = RelationshipRef(RelationshipType::WITH, entInfoW.fixedStringX, entInfoW.fixedStringX);
    RelationshipRef r3 = RelationshipRef(RelationshipType::WITH, entInfoW.fixedInt1, entInfoW.fixedInt2);
    RelationshipRef r4 = RelationshipRef(RelationshipType::WITH, entInfoW.fixedStringX, entInfoW.fixedStringY);

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
    DB db;
    PkbGetter* pg = new PkbGetterStub(&db);
    WithHandler* wh = new WithHandler(pg);
    RelationshipRef r1 = RelationshipRef(RelationshipType::WITH, entInfoW.varSynVarAttr, entInfoW.fixedStringX);
    RelationshipRef r2 = RelationshipRef(RelationshipType::WITH, entInfoW.varSynVarAttr, entInfoW.fixedStringW);
    RelationshipRef r3 = RelationshipRef(RelationshipType::WITH, entInfoW.procSyn1, entInfoW.fixedStringF);
    RelationshipRef r4 = RelationshipRef(RelationshipType::WITH, entInfoW.procSyn1, entInfoW.fixedStringW);

    Result result1 = wh->handleWith(r1);
    result1 = scrubResult(result1);
    Result result2 = wh->handleWith(r2);
    result2 = scrubResult(result2);
    Result result3 = wh->handleWith(r3);
    result3 = scrubResult(result3);
    Result result4 = wh->handleWith(r4);
    result4 = scrubResult(result4);

    Result expectedResult1;
    expectedResult1.setResultType(ResultType::WITH_CLAUSE);
    expectedResult1.setValid(true);
    expectedResult1.setOneSynEntity(entInfoW.varSyn);
    std::set<ProgramElement*> result1Elements = {&sourceInfoW.varX};
    expectedResult1.setOneSynSet(result1Elements);

    Result expectedResult3;
    expectedResult3.setResultType(ResultType::WITH_CLAUSE);
    expectedResult3.setValid(true);
    expectedResult3.setOneSynEntity(entInfoW.procSyn1);
    std::set<ProgramElement*> result3Elements = {&sourceInfoW.procF};
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

TEST_CASE("test no sides fixed") {
    DB db;
    PkbGetter* pg = new PkbGetterStub(&db);
    WithHandler* wh = new WithHandler(pg);

    RelationshipRef r1 = RelationshipRef(RelationshipType::WITH, entInfoW.stmtSynStmtAttr, entInfoW.printSynStmtAttr);
    Result result1 = wh->handleWith(r1);
    result1 = scrubResult(result1);
    Result expectedResult1;
    expectedResult1.setResultType(ResultType::WITH_CLAUSE);
    expectedResult1.setValid(true);
    expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(entInfoW.stmtSyn, entInfoW.printSyn));
    std::set<std::pair<ProgramElement*, ProgramElement*>> result1Elements = {{&sourceInfoW.stmt11, &sourceInfoW.print11}};
    expectedResult1.setTwoSynSet(result1Elements);

    RelationshipRef r2 = RelationshipRef(RelationshipType::WITH, entInfoW.readSynVarAttr, entInfoW.printSynVarAttr);
    Result result2 = wh->handleWith(r2);
    result2 = scrubResult(result2);
    Result expectedResult2;
    expectedResult2.setResultType(ResultType::WITH_CLAUSE);
    expectedResult2.setValid(true);
    expectedResult2.setTwoSynEntities(std::pair<Entity, Entity>(entInfoW.readSyn, entInfoW.printSyn));
    std::set<std::pair<ProgramElement*, ProgramElement*>> result2Elements = {{&sourceInfoW.read5, &sourceInfoW.print11}};
    expectedResult2.setTwoSynSet(result2Elements);

    RelationshipRef r3 = RelationshipRef(RelationshipType::WITH, entInfoW.readSynStmtAttr, entInfoW.printSynStmtAttr);
    Result result3 = wh->handleWith(r3);
    result3 = scrubResult(result3);
    Result expectedResult3;
    expectedResult3.setResultType(ResultType::WITH_CLAUSE);
    expectedResult3.setValid(false);
    expectedResult3.setTwoSynEntities(std::pair<Entity, Entity>(entInfoW.readSyn, entInfoW.printSyn));

    SECTION("result is valid") {
        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResult2);
    }

    SECTION("result is invalid") {
        REQUIRE(result3 == expectedResult3);
    }
}
