#include <vector>

#include "QPS/Clause.h"
#include "catch.hpp"
#include "QPS/PatternHandler.h"
#include "PKB.h"
#include "PkbGetterStub.h"
#include "QPSUtil.h"

// Source for basic testing
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
//procedure g{
// }

// Source for partial and full matching
//procedure f {
//  x = a + b + c; // 1
//  y = x + z; // 2
//  if (y == 10) { // 3
//    z = x; // 4
//  } else {
//    z = x; // 5
//  }
//  while (z > x) { // 6
//    z = x - y; // 7
//  }
//  a = b + c; // 8
//}

PATTERN_SOURCE_INFO patternInfo;
QPS_SOURCE_INFO sourceInfoP;
QPS_GENERAL_DATA_INFO entityInfoP;

TEST_CASE("Testing Right Side WILDCARD PATTERN Relationships") {
    DB db;
    PkbGetter* pg = new PkbGetterStub(&db);
    PatternHandler* p = new PatternHandler(pg);

    SECTION("Assignments") {
        RelationshipRef assignPatternRef_varSyn_wc = RelationshipRef(RelationshipType::PATTERN, entityInfoP.varSyn, entityInfoP.wildcard, entityInfoP.assignSyn); // a (v, _)
        RelationshipRef assignPatternRef_wc_wc = RelationshipRef(RelationshipType::PATTERN, entityInfoP.wildcard, entityInfoP.wildcard, entityInfoP.assignSyn); // a (_, _)
        RelationshipRef assignPatternRef_fixedStr_wc = RelationshipRef(RelationshipType::PATTERN, entityInfoP.fixedStringZ, entityInfoP.wildcard, entityInfoP.assignSyn); // a ("z", _)

        Result result1 = p->handlePattern(assignPatternRef_varSyn_wc);
        Result result2 = p->handlePattern(assignPatternRef_wc_wc);
        Result result3 = p->handlePattern(assignPatternRef_fixedStr_wc);

        Result expectedResult1;
        expectedResult1.setResultType(ResultType::PATTERN_CLAUSE);
        expectedResult1.setValid(true);
        expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(entityInfoP.assignSyn, entityInfoP.varSyn));
        std::set<std::pair<ProgramElement*, ProgramElement*>> result1Elements = {{&sourceInfoP.assign1, &sourceInfoP.varX}, {&sourceInfoP.assign2, &sourceInfoP.varY}, {&sourceInfoP.assign3, &sourceInfoP.varX},
                                                                                 {&sourceInfoP.assign7, &sourceInfoP.varX}, {&sourceInfoP.assign8, &sourceInfoP.varZ}, {&sourceInfoP.assign9, &sourceInfoP.varY},
                                                                                 {&sourceInfoP.assign10, &sourceInfoP.varZ}};
        expectedResult1.setTwoSynSet(result1Elements);

        Result expectedResult2;
        expectedResult2.setResultType(ResultType::PATTERN_CLAUSE);
        expectedResult2.setValid(true);
        expectedResult2.setOneSynEntity(entityInfoP.assignSyn);
        expectedResult2.setOneSynSet(sourceInfoP.assigns);

        Result expectedResult3;
        expectedResult3.setResultType(ResultType::PATTERN_CLAUSE);
        expectedResult3.setValid(true);
        expectedResult3.setOneSynEntity(entityInfoP.assignSyn);
        std::set<ProgramElement*> result3Elements = {&sourceInfoP.assign8, &sourceInfoP.assign10};
        expectedResult3.setOneSynSet(result3Elements);

        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResult2);
        REQUIRE(result3 == expectedResult3);
    }

    SECTION("Whiles") {
        RelationshipRef whilePatternRef_varSyn_wc = RelationshipRef(RelationshipType::PATTERN, entityInfoP.varSyn, entityInfoP.wildcard, entityInfoP.whileSyn); // w (v, _)
        RelationshipRef whilePatternRef_wc_wc = RelationshipRef(RelationshipType::PATTERN, entityInfoP.wildcard, entityInfoP.wildcard, entityInfoP.whileSyn); // w (_, _)
        RelationshipRef whilePatternRef_fixedStr_wc = RelationshipRef(RelationshipType::PATTERN, entityInfoP.fixedStringX, entityInfoP.wildcard, entityInfoP.whileSyn); // w ("x", _)

        Result result1 = p->handlePattern(whilePatternRef_varSyn_wc);
        Result result2 = p->handlePattern(whilePatternRef_wc_wc);
        Result result3 = p->handlePattern(whilePatternRef_fixedStr_wc);

        Result expectedResult1;
        expectedResult1.setResultType(ResultType::PATTERN_CLAUSE);
        expectedResult1.setValid(true);
        expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(entityInfoP.whileSyn, entityInfoP.varSyn));
        std::set<std::pair<ProgramElement*, ProgramElement*>> result1Elements = {{&sourceInfoP.while4, &sourceInfoP.varX}};
        expectedResult1.setTwoSynSet(result1Elements);

        Result expectedResultWhile;
        expectedResultWhile.setResultType(ResultType::PATTERN_CLAUSE);
        expectedResultWhile.setValid(true);
        expectedResultWhile.setOneSynEntity(entityInfoP.whileSyn);
        expectedResultWhile.setOneSynSet(sourceInfoP.whiles);

        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResultWhile);
        REQUIRE(result3 == expectedResultWhile);
    }

    SECTION("Ifs") {
        RelationshipRef ifPatternRef_varSyn_wc = RelationshipRef(RelationshipType::PATTERN, entityInfoP.varSyn, entityInfoP.wildcard, entityInfoP.ifSyn); // ifs (v, _, _)
        RelationshipRef ifPatternRef_wc_wc = RelationshipRef(RelationshipType::PATTERN, entityInfoP.wildcard, entityInfoP.wildcard, entityInfoP.ifSyn); // ifs (_, _, _)
        RelationshipRef ifPatternRef_fixedStr_wc = RelationshipRef(RelationshipType::PATTERN, entityInfoP.fixedStringZ, entityInfoP.wildcard, entityInfoP.ifSyn); // ifs ("z", _, _)

        Result result1 = p->handlePattern(ifPatternRef_varSyn_wc);
        Result result2 = p->handlePattern(ifPatternRef_wc_wc);
        Result result3 = p->handlePattern(ifPatternRef_fixedStr_wc);

        Result expectedResult1;
        expectedResult1.setResultType(ResultType::PATTERN_CLAUSE);
        expectedResult1.setValid(true);
        expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(entityInfoP.ifSyn, entityInfoP.varSyn));
        std::set<std::pair<ProgramElement*, ProgramElement*>> result1Elements = {{&sourceInfoP.if6, &sourceInfoP.varZ}};
        expectedResult1.setTwoSynSet(result1Elements);

        Result expectedResultIf;
        expectedResultIf.setResultType(ResultType::PATTERN_CLAUSE);
        expectedResultIf.setValid(true);
        expectedResultIf.setOneSynEntity(entityInfoP.ifSyn);
        expectedResultIf.setOneSynSet(sourceInfoP.ifs);

        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResultIf);
        REQUIRE(result3 == expectedResultIf);
    }
}

TEST_CASE("Testing Partial Match PATTERN Relationships") {
    DB db;
    PkbGetter* pg = new PkbGetterStub(&db);
    PatternHandler* p = new PatternHandler(pg);

    RelationshipRef assignPatternRef_varSyn_fixedStrWW = RelationshipRef(RelationshipType::PATTERN, entityInfoP.varSyn, entityInfoP.fixedStringWWBC, entityInfoP.assignSyn); // a (v, _"b + c"_)
    RelationshipRef assignPatternRef_wc_fixedStrWW = RelationshipRef(RelationshipType::PATTERN, entityInfoP.wildcard, entityInfoP.fixedStringWWX, entityInfoP.assignSyn); // a (_, _"x"_)
    RelationshipRef assignPatternRef_fixedStr_fixedStrWW = RelationshipRef(RelationshipType::PATTERN, entityInfoP.fixedStringX, entityInfoP.fixedStringWWBC, entityInfoP.assignSyn); // a ("x", _"b + c"_)

    Result result1 = p->handlePattern(assignPatternRef_varSyn_fixedStrWW);
    Result result2 = p->handlePattern(assignPatternRef_wc_fixedStrWW);
    Result result3 = p->handlePattern(assignPatternRef_fixedStr_fixedStrWW);

    Result expectedResult1;
    expectedResult1.setResultType(ResultType::PATTERN_CLAUSE);
    expectedResult1.setValid(true);
    expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(entityInfoP.assignSyn, entityInfoP.varSyn));
    expectedResult1.setTwoSynSet(patternInfo.assignVarPartial);

    Result expectedResult2;
    expectedResult2.setResultType(ResultType::PATTERN_CLAUSE);
    expectedResult2.setValid(true);
    expectedResult2.setOneSynEntity(entityInfoP.assignSyn);
    std::set<ProgramElement*> result2Elements = {&patternInfo.assign2, &patternInfo.assign4, &patternInfo.assign5, &patternInfo.assign7};
    expectedResult2.setOneSynSet(patternInfo.assignGivenExprPartial);

    Result expectedResult3;
    expectedResult3.setResultType(ResultType::PATTERN_CLAUSE);
    expectedResult3.setValid(true);
    expectedResult3.setOneSynEntity(entityInfoP.assignSyn);
    expectedResult3.setOneSynSet(patternInfo.assignGivenExprVarPartial);

    REQUIRE(result1 == expectedResult1);
    REQUIRE(result2 == expectedResult2);
    REQUIRE(result3 == expectedResult3);
}

TEST_CASE("Testing Full Match PATTERN Relationships") {
    DB db;
    PkbGetter* pg = new PkbGetterStub(&db);
    PatternHandler* p = new PatternHandler(pg);

    RelationshipRef assignPatternRef_varSyn_fixedStr = RelationshipRef(RelationshipType::PATTERN, entityInfoP.varSyn, entityInfoP.fixedStringBC, entityInfoP.assignSyn); // a (v, "b + c")
    RelationshipRef assignPatternRef_wc_fixedStr = RelationshipRef(RelationshipType::PATTERN, entityInfoP.wildcard, entityInfoP.fixedStringABC, entityInfoP.assignSyn); // a (_, "a + b + c")
    RelationshipRef assignPatternRef_fixedStr_fixedStr = RelationshipRef(RelationshipType::PATTERN, entityInfoP.fixedStringZ, entityInfoP.fixedStringX, entityInfoP.assignSyn); // a ("z", "x")

    Result result1 = p->handlePattern(assignPatternRef_varSyn_fixedStr);
    Result result2 = p->handlePattern(assignPatternRef_wc_fixedStr);
    Result result3 = p->handlePattern(assignPatternRef_fixedStr_fixedStr);

    Result expectedResult1;
    expectedResult1.setResultType(ResultType::PATTERN_CLAUSE);
    expectedResult1.setValid(true);
    expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(entityInfoP.assignSyn, entityInfoP.varSyn));
    expectedResult1.setTwoSynSet(patternInfo.assignVarFull);

    Result expectedResult2;
    expectedResult2.setResultType(ResultType::PATTERN_CLAUSE);
    expectedResult2.setValid(true);
    expectedResult2.setOneSynEntity(entityInfoP.assignSyn);
    expectedResult2.setOneSynSet(patternInfo.assignGivenExprFull);

    Result expectedResult3;
    expectedResult3.setResultType(ResultType::PATTERN_CLAUSE);
    expectedResult3.setValid(true);
    expectedResult3.setOneSynEntity(entityInfoP.assignSyn);
    expectedResult3.setOneSynSet(patternInfo.assignGivenExprVarFull);

    REQUIRE(result1 == expectedResult1);
    REQUIRE(result2 == expectedResult2);
    REQUIRE(result3 == expectedResult3);
}