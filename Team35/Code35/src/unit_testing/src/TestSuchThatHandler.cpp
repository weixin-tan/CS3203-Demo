#include <vector>

#include "Clause.h"
#include "catch.hpp"
#include "SuchThatHandler.h"
#include "PKB.h"
#include "PkbStub.h"








// Source for testing
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

// Modifies, Uses are similar in implementation
// Follows, Follows*, Parent, Parent*, Next, Next*, Affects, Affects* are similar in implementation
// Calls, Calls* are similar in implementation


Entity procSyn1 = Entity(EntityType::Procedure, "p1");
Entity procSyn2 = Entity(EntityType::Procedure, "p2");

Entity varSyn = Entity(EntityType::Variable, "v");
Entity constSyn = Entity(EntityType::Constant, "c");

Entity stmtSyn = Entity(EntityType::Statement, "s");
Entity printSyn = Entity(EntityType::Print, "p");
Entity readSyn = Entity(EntityType::Read, "r");
Entity assignSyn = Entity(EntityType::Assignment, "a");
Entity callSyn = Entity(EntityType::Call, "call");
Entity whileSyn = Entity(EntityType::While, "w");
Entity ifSyn = Entity(EntityType::If, "ifs");

Entity fixedStrVarX = Entity(EntityType::FixedString, "x");
Entity fixedStrVarY = Entity(EntityType::FixedString, "y");
Entity fixedStrVarZ = Entity(EntityType::FixedString, "z");
Entity fixedStrProcF = Entity(EntityType::FixedString, "f");
Entity fixedStrProcG = Entity(EntityType::FixedString, "g");

Entity fixedIntStmt = Entity(EntityType::FixedInteger, "1");
Entity fixedIntPrint = Entity(EntityType::FixedInteger, "11");
Entity fixedIntRead = Entity(EntityType::FixedInteger, "5");
Entity fixedIntAssign = Entity(EntityType::FixedInteger, "2");
Entity fixedIntWhile = Entity(EntityType::FixedInteger, "4");
Entity fixedIntCall = Entity(EntityType::FixedInteger, "12");
Entity fixedIntIf = Entity(EntityType::FixedInteger, "6");

Entity wc = Entity(EntityType::Wildcard, "_");

RelationshipRef callsRef_procSyn_procSyn = RelationshipRef(RelationshipType::Calls, procSyn1, procSyn2); // Calls(p1, p2)
RelationshipRef callsRef_procSyn_fixedStr = RelationshipRef(RelationshipType::Calls, procSyn1, fixedStrProcG); // Calls(f, "g")
RelationshipRef callsRef_procSyn_wc = RelationshipRef(RelationshipType::Calls, procSyn1, wc); // Calls(p1, _)
RelationshipRef callsRef_fixedStr_procSyn = RelationshipRef(RelationshipType::Calls, fixedStrProcF, procSyn1); // Calls("f", p1)
RelationshipRef callsRef_fixedStr_fixedStr = RelationshipRef(RelationshipType::Calls, fixedStrProcF, fixedStrProcG); // Calls("f", "g")
RelationshipRef callsRef_fixedStr_wc = RelationshipRef(RelationshipType::Calls, fixedStrProcF, wc); // Calls("f", _)
RelationshipRef callsRef_wc_procSyn = RelationshipRef(RelationshipType::Calls, wc, procSyn1); // Calls(_, p1)
RelationshipRef callsRef_wc_fixedStr = RelationshipRef(RelationshipType::Calls, wc, fixedStrProcG); // Calls(_, "g")
RelationshipRef callsRef_wc_wc = RelationshipRef(RelationshipType::Calls, wc, wc); // Calls (_, _)

RelationshipRef followsRef_stmtSyn_stmtSyn = RelationshipRef(RelationshipType::Follows, printSyn, callSyn); // Follows(p, call)
RelationshipRef followsTRef_stmtSyn_fixedInt = RelationshipRef(RelationshipType::FollowsT, assignSyn, fixedIntCall); // Follows*(a, "12")
RelationshipRef parentRef_stmtSyn_wc = RelationshipRef(RelationshipType::Parent, whileSyn, wc); //Parent(w, _)
RelationshipRef parentTRef_fixedInt_stmtSyn = RelationshipRef(RelationshipType::ParentT, fixedIntWhile,  assignSyn);//Parent*("4", a)
RelationshipRef nextRef_fixedInt_fixedInt = RelationshipRef(RelationshipType::Next, fixedIntPrint, fixedIntCall); //Next("11", "12")
RelationshipRef nextTRef_fixedInt_wc = RelationshipRef(RelationshipType::NextT, fixedIntStmt, wc); //Next*("1", _);
RelationshipRef affectsRef_wc_stmtSyn = RelationshipRef(RelationshipType::Affects, wc, assignSyn); // Affects(_, a)
RelationshipRef affectsTRef_wc_fixedInt = RelationshipRef(RelationshipType::AffectsT, wc, fixedIntAssign); // Affects*(_, "2")
RelationshipRef followsRef_wc_wc = RelationshipRef(RelationshipType::Follows, wc, wc); // Follows(_, _)

TEST_CASE("Testing Modifies/Uses Relationships"){

    RelationshipRef modifiesRef_procSyn_varSyn = RelationshipRef(RelationshipType::Modifies, procSyn1, varSyn); // Modifies(p1, v)
    RelationshipRef usesRef_fixedStr_varSyn = RelationshipRef(RelationshipType::Uses, fixedStrProcF, varSyn); // Uses("f", v)
    RelationshipRef modifiesRef_stmtSyn_varSyn = RelationshipRef(RelationshipType::Modifies, readSyn, varSyn); // Modifies(r, v)
    RelationshipRef usesRef_fixedInt_varSyn = RelationshipRef(RelationshipType::Uses, fixedIntAssign, varSyn); // Uses("2", v)
    RelationshipRef usesRef_procSyn_fixedStr = RelationshipRef(RelationshipType::Uses, procSyn1, fixedStrVarX); // Uses(p1, "x")
    RelationshipRef modifiesRef_fixedStr_fixedStr = RelationshipRef(RelationshipType::Modifies, fixedStrProcF, fixedStrVarY); // Modifies("f", "y")
    RelationshipRef usesRef_stmtSyn_fixedStr = RelationshipRef(RelationshipType::Uses, ifSyn, fixedStrVarZ); // Uses(ifs, "z")
    RelationshipRef modifiesRef_fixedInt_fixedStr = RelationshipRef(RelationshipType::Modifies, fixedIntRead, fixedStrVarZ); // Modifies("5", "z")
    RelationshipRef modifiesRef_procSyn_wc = RelationshipRef(RelationshipType::Modifies, procSyn1, wc); // Modifies(p1, _)
    RelationshipRef usesRef_fixedStr_wc = RelationshipRef(RelationshipType::Uses, fixedStrProcF, wc); // Uses("f", _)
    RelationshipRef modifiesRef_stmtSyn_wc = RelationshipRef(RelationshipType::Modifies, stmtSyn, wc); // Modifies(s, _)
    RelationshipRef usesRef_fixedInt_wc = RelationshipRef(RelationshipType::Uses, fixedIntIf, wc); // Uses("6", _)

    DB db;
    PkbGetterStub pg = PkbGetterStub(&db);
    PkbStub pkb = PkbStub(pg);
    SuchThatHandler* st = new SuchThatHandler(pkb.getPkbGetterStub());

    Result result1 = st->handleSuchThat(modifiesRef_procSyn_varSyn);// Modifies(p1, v)
    Result result2 = st->handleSuchThat(usesRef_fixedStr_varSyn);// Uses("f", v)
    Result result3 = st->handleSuchThat(modifiesRef_stmtSyn_varSyn);// Modifies(r, v)
    Result result4 = st->handleSuchThat(usesRef_fixedInt_varSyn);// Uses("2", v)
    Result result5 = st->handleSuchThat(usesRef_procSyn_fixedStr);// Uses(p1, "x")
    Result result6 = st->handleSuchThat(modifiesRef_fixedStr_fixedStr);// Modifies("f", "y")
    Result result7 = st->handleSuchThat(usesRef_stmtSyn_fixedStr);// Uses(ifs, "z")
    Result result8 = st->handleSuchThat(modifiesRef_fixedInt_fixedStr);// Modifies("5", "z")
    Result result9 = st->handleSuchThat(modifiesRef_procSyn_wc);// Modifies(p1, _)
    Result result10 = st->handleSuchThat(usesRef_fixedStr_wc);// Uses("f", _)
    Result result11 = st->handleSuchThat(modifiesRef_stmtSyn_wc);// Modifies(s, _)
    Result result12 = st->handleSuchThat(usesRef_fixedInt_wc);// Uses("6", _)

    Result expectedResult1;
    expectedResult1.setValid(true);
    expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(procSyn1, varSyn));
    std::set<std::pair<ProgramElement, ProgramElement>> result1Elements;
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("x")));
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("y")));
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("z")));
    expectedResult1.setTwoSynSet(result1Elements);

    Result expectedResult2;
    expectedResult2.setValid(true);
    expectedResult2.setOneSynEntity(varSyn);
    std::set<ProgramElement> result2Elements;
    result2Elements.insert(ProgramElement::createVariable("x"));
    result2Elements.insert(ProgramElement::createVariable("y"));
    result2Elements.insert(ProgramElement::createVariable("z"));
    expectedResult2.setOneSynSet(result2Elements);

    Result expectedResult3;
    expectedResult3.setValid(true);
    expectedResult3.setTwoSynEntities(std::pair<Entity, Entity>(readSyn, varSyn));
    std::set<std::pair<ProgramElement, ProgramElement>> result3Elements;
    result3Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kRead, 5), ProgramElement::createVariable("z")));
    expectedResult3.setTwoSynSet(result3Elements);

    Result expectedResult4;
    expectedResult4.setValid(true);
    expectedResult4.setOneSynEntity(varSyn);
    std::set<ProgramElement> result4Elements;
    result4Elements.insert(ProgramElement::createVariable("x"));
    expectedResult4.setOneSynSet(result4Elements);

    Result expectedResult5;
    expectedResult5.setValid(true);
    expectedResult5.setOneSynEntity(procSyn1);
    std::set<ProgramElement> result5Elements;
    result5Elements.insert(ProgramElement::createProcedure("f"));
    expectedResult5.setOneSynSet(result5Elements);

    Result expectedResult6;
    expectedResult6.setValid(true);

    Result expectedResult7;
    expectedResult7.setValid(true);
    expectedResult7.setOneSynEntity(ifSyn);
    std::set<ProgramElement> result7Elements;
    result7Elements.insert(ProgramElement::createStatement(ElementType::kIf, 6));
    expectedResult7.setOneSynSet(result7Elements);

    Result expectedResult8;
    expectedResult8.setValid(true);

    Result expectedResult9;
    expectedResult9.setValid(true);
    expectedResult9.setOneSynEntity(procSyn1);
    std::set<ProgramElement> result9Elements;
    result9Elements.insert(ProgramElement::createProcedure("f"));
    expectedResult9.setOneSynSet(result9Elements);

    Result expectedResult10;
    expectedResult10.setValid(true);

    Result expectedResult11;
    expectedResult11.setValid(true);
    expectedResult11.setOneSynEntity(stmtSyn);
    std::set<ProgramElement> result11Elements;
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 1));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 2));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 3));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 5));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 7));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 8));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 9));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 10));

    Result expectedResult12;
    expectedResult12.setValid(true);

    REQUIRE(result1 == expectedResult1);
    REQUIRE(result2 == expectedResult2);
    REQUIRE(result3 == expectedResult3);
    REQUIRE(result4 == expectedResult4);
    REQUIRE(result5 == expectedResult5);
    REQUIRE(result6 == expectedResult6);
    REQUIRE(result7 == expectedResult7);
    REQUIRE(result8 == expectedResult8);
    REQUIRE(result9 == expectedResult9);
    REQUIRE(result10 == expectedResult10);
    REQUIRE(result11 == expectedResult11);
    REQUIRE(result12 == expectedResult12);
}

TEST_CASE("Testing Calls(*) Relationships"){
    //Result result1 = st->handleSuchThat(modifiesRef_fixedInt_fixedStr);
}

TEST_CASE("Testing Follows(*)/Parent(*)/Next(*)/Affects(*) Relationships"){
    //Result result1 = st->handleSuchThat(modifiesRef_fixedInt_fixedStr);
}