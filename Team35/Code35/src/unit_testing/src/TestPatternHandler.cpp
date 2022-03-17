#include <vector>

#include "Clause.h"
#include "catch.hpp"
#include "PatternHandler.h"
#include "PKB.h"


// Source for testing
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
/*
Entity varSyn = Entity(EntityType::Variable, "v");
Entity assignSyn = Entity(EntityType::Assignment, "a");
Entity whileSyn = Entity(EntityType::While, "w");
Entity ifSyn = Entity(EntityType::If, "ifs");

Entity fixedStrVarX = Entity(EntityType::FixedString, "x");
Entity fixedStrVarY = Entity(EntityType::FixedString, "y");
Entity fixedStrVarZ = Entity(EntityType::FixedString, "z");
Entity fixedStrABC = Entity(EntityType::FixedString, "a + b + c");
Entity fixedStrBC = Entity(EntityType::FixedString, "b + c");
Entity fixedStrWWX = Entity(EntityType::FixedStringWithinWildcard, "x");
Entity fixedStrWWBC = Entity(EntityType::FixedStringWithinWildcard, "b + c");

Entity wc = Entity(EntityType::Wildcard, "_");


TEST_CASE("Testing Right Side Wildcard Pattern Relationships") {

    PKB pkb;
    PatternHandler* p(pkb.getGetter());

    SECTION("Assignments") {
        RelationshipRef assignPatternRef_varSyn_wc = RelationshipRef(RelationshipType::Pattern, varSyn, wc, assignSyn); // a (v, _)
        RelationshipRef assignPatternRef_wc_wc = RelationshipRef(RelationshipType::Pattern, wc, wc, assignSyn); // a (_, _)
        RelationshipRef assignPatternRef_fixedStr_wc = RelationshipRef(RelationshipType::Pattern, varSyn, fixedStrVarZ, assignSyn); // a ("z", _)

        Result result1 = p->handlePattern(assignPatternRef_varSyn_wc);
        Result result2 = p->handlePattern(assignPatternRef_wc_wc);
        Result result3 = p->handlePattern(assignPatternRef_fixedStr_wc);

        Result expectedResult1;
        expectedResult1.setValid(true);
        expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(assignSyn, varSyn));
        std::set<std::pair<ProgramElement, ProgramElement>> result1Elements;
        result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kAssignment, 1), ProgramElement::createVariable("x")));
        result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kAssignment, 2), ProgramElement::createVariable("y")));
        result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kAssignment, 4), ProgramElement::createVariable("z")));
        result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kAssignment, 5), ProgramElement::createVariable("z")));
        result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kAssignment, 7), ProgramElement::createVariable("z")));
        result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kAssignment, 8), ProgramElement::createVariable("a")));
        expectedResult1.setTwoSynSet(result1Elements);

        Result expectedResult2;
        expectedResult2.setValid(true);
        expectedResult2.setOneSynEntity(assignSyn);
        std::set<ProgramElement> result2Elements;
        result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 1));
        result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 2));
        result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 4));
        result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 5));
        result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 7));
        result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 8));
        expectedResult2.setOneSynSet(result2Elements);

        Result expectedResult3;
        expectedResult3.setValid(true);
        expectedResult3.setOneSynEntity(assignSyn);
        std::set<ProgramElement> result3Elements;
        result3Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 4));
        result3Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 5));
        result3Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 7));
        expectedResult3.setOneSynSet(result3Elements);

        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResult2);
        REQUIRE(result3 == expectedResult3);
    }

    SECTION("Whiles") {
        RelationshipRef whilePatternRef_varSyn_wc = RelationshipRef(RelationshipType::Pattern, varSyn, wc, whileSyn); // w (v, _)
        RelationshipRef whilePatternRef_wc_wc = RelationshipRef(RelationshipType::Pattern, wc, wc, whileSyn); // w (_, _)
        RelationshipRef whilePatternRef_fixedStr_wc = RelationshipRef(RelationshipType::Pattern, varSyn, fixedStrVarX, whileSyn); // w ("x", _)

        Result result1 = p->handlePattern(whilePatternRef_varSyn_wc);
        Result result2 = p->handlePattern(whilePatternRef_wc_wc);
        Result result3 = p->handlePattern(whilePatternRef_fixedStr_wc);

        Result expectedResult1;
        expectedResult1.setValid(true);
        expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(whileSyn, varSyn));
        std::set<std::pair<ProgramElement, ProgramElement>> result1Elements;
        result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kWhile, 6), ProgramElement::createVariable("z")));
        result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kWhile, 6), ProgramElement::createVariable("x")));
        expectedResult1.setTwoSynSet(result1Elements);

        Result expectedResultWhile;
        expectedResultWhile.setValid(true);
        expectedResultWhile.setOneSynEntity(whileSyn);
        std::set<ProgramElement> resultWhileElements;
        resultWhileElements.insert(ProgramElement::createStatement(ElementType::kWhile, 6));
        expectedResultWhile.setOneSynSet(resultWhileElements);

        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResultWhile);
        REQUIRE(result3 == expectedResultWhile);
    }

    SECTION("Ifs") {
        RelationshipRef ifPatternRef_varSyn_wc = RelationshipRef(RelationshipType::Pattern, varSyn, wc, ifSyn); // ifs (v, _, _)
        RelationshipRef ifPatternRef_wc_wc = RelationshipRef(RelationshipType::Pattern, wc, wc, ifSyn); // ifs (_, _, _)
        RelationshipRef ifPatternRef_fixedStr_wc = RelationshipRef(RelationshipType::Pattern, varSyn, fixedStrVarY, ifSyn); // ifs ("y", _, _)

        Result result1 = p->handlePattern(ifPatternRef_varSyn_wc);
        Result result2 = p->handlePattern(ifPatternRef_wc_wc);
        Result result3 = p->handlePattern(ifPatternRef_fixedStr_wc);

        Result expectedResult1;
        expectedResult1.setValid(true);
        expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(ifSyn, varSyn));
        std::set<std::pair<ProgramElement, ProgramElement>> result1Elements;
        result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kIf, 3), ProgramElement::createVariable("y")));
        expectedResult1.setTwoSynSet(result1Elements);

        Result expectedResultIf;
        expectedResultIf.setValid(true);
        expectedResultIf.setOneSynEntity(ifSyn);
        std::set<ProgramElement> resultIfElements;
        resultIfElements.insert(ProgramElement::createStatement(ElementType::kIf, 3));
        expectedResultIf.setOneSynSet(resultIfElements);

        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResultIf);
        REQUIRE(result3 == expectedResultIf);
    }
}

TEST_CASE("Testing Partial Match Pattern Relationships") {

    PKB pkb;
    PatternHandler* p(pkb.getGetter());

    RelationshipRef assignPatternRef_varSyn_fixedStrWW = RelationshipRef(RelationshipType::Pattern, varSyn, fixedStrWWBC, assignSyn); // a (v, _"b + c"_)
    RelationshipRef assignPatternRef_wc_fixedStrWW = RelationshipRef(RelationshipType::Pattern, wc, fixedStrWWX, assignSyn); // a (_, _"x"_)
    RelationshipRef assignPatternRef_fixedStr_fixedStrWW = RelationshipRef(RelationshipType::Pattern, fixedStrVarX, fixedStrWWBC, assignSyn); // a ("x", _"b + c"_)

    Result result1 = p->handlePattern(assignPatternRef_varSyn_fixedStrWW);
    Result result2 = p->handlePattern(assignPatternRef_wc_fixedStrWW);
    Result result3 = p->handlePattern(assignPatternRef_fixedStr_fixedStrWW);

    Result expectedResult1;
    expectedResult1.setValid(true);
    expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(assignSyn, varSyn));
    std::set<std::pair<ProgramElement, ProgramElement>> result1Elements;
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kAssignment, 1), ProgramElement::createVariable("x")));
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kAssignment, 8), ProgramElement::createVariable("a")));
    expectedResult1.setTwoSynSet(result1Elements);

    Result expectedResult2;
    expectedResult2.setValid(true);
    expectedResult2.setOneSynEntity(assignSyn);
    std::set<ProgramElement> result2Elements;
    result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 2));
    result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 4));
    result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 5));
    result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 7));
    expectedResult2.setOneSynSet(result2Elements);

    Result expectedResult3;
    expectedResult3.setValid(true);
    expectedResult3.setOneSynEntity(assignSyn);
    std::set<ProgramElement> result3Elements;
    result3Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 1));
    expectedResult3.setOneSynSet(result3Elements);

    REQUIRE(result1 == expectedResult1);
    REQUIRE(result2 == expectedResult2);
    REQUIRE(result3 == expectedResult3);
}

TEST_CASE("Testing Full Match Pattern Relationships") {

    PKB pkb;
    PatternHandler* p(pkb.getGetter());

    RelationshipRef assignPatternRef_varSyn_fixedStr = RelationshipRef(RelationshipType::Pattern, varSyn, fixedStrBC, assignSyn); // a (v, "b + c")
    RelationshipRef assignPatternRef_wc_fixedStr = RelationshipRef(RelationshipType::Pattern, wc, fixedStrABC, assignSyn); // a (_, "a + b + c")
    RelationshipRef assignPatternRef_fixedStr_fixedStr = RelationshipRef(RelationshipType::Pattern, fixedStrVarZ, fixedStrVarX, assignSyn); // a ("z", "x")

    Result result1 = p->handlePattern(assignPatternRef_varSyn_fixedStr);
    Result result2 = p->handlePattern(assignPatternRef_wc_fixedStr);
    Result result3 = p->handlePattern(assignPatternRef_fixedStr_fixedStr);


    Result expectedResult1;
    expectedResult1.setValid(true);
    expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(assignSyn, varSyn));
    std::set<std::pair<ProgramElement, ProgramElement>> result1Elements;
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kAssignment, 8), ProgramElement::createVariable("a")));
    expectedResult1.setTwoSynSet(result1Elements);

    Result expectedResult2;
    expectedResult2.setValid(true);
    expectedResult2.setOneSynEntity(assignSyn);
    std::set<ProgramElement> result2Elements;
    result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 1));
    expectedResult2.setOneSynSet(result2Elements);

    Result expectedResult3;
    expectedResult3.setValid(true);
    expectedResult3.setOneSynEntity(assignSyn);
    std::set<ProgramElement> result3Elements;
    result3Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 4));
    result3Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 5));
    expectedResult3.setOneSynSet(result3Elements);

    REQUIRE(result1 == expectedResult1);
    REQUIRE(result2 == expectedResult2);
    REQUIRE(result3 == expectedResult3);
}
*/