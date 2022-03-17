#include <vector>

#include "Clause.h"
#include "catch.hpp"
#include "SuchThatHandler.h"
#include "PKB.h"


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


Entity procSyn1 = Entity(EntityType::PROCEDURE, "p1");
Entity procSyn2 = Entity(EntityType::PROCEDURE, "p2");

Entity varSyn = Entity(EntityType::VARIABLE, "v");
Entity constSyn = Entity(EntityType::CONSTANT, "c");

Entity stmtSyn = Entity(EntityType::STATEMENT, "s");
Entity printSyn = Entity(EntityType::PRINT, "p");
Entity readSyn = Entity(EntityType::READ, "r");
Entity assignSyn = Entity(EntityType::ASSIGNMENT, "a");
Entity callSyn = Entity(EntityType::CALL, "call");
Entity whileSyn = Entity(EntityType::WHILE, "w");
Entity ifSyn = Entity(EntityType::IF, "ifs");

Entity fixedStrVarX = Entity(EntityType::FIXED_STRING, "x");
Entity fixedStrVarY = Entity(EntityType::FIXED_STRING, "y");
Entity fixedStrVarZ = Entity(EntityType::FIXED_STRING, "z");
Entity fixedStrProcF = Entity(EntityType::FIXED_STRING, "f");
Entity fixedStrProcG = Entity(EntityType::FIXED_STRING, "g");

Entity fixedIntStmt = Entity(EntityType::FIXED_INTEGER, "1");
Entity fixedIntPrint = Entity(EntityType::FIXED_INTEGER, "11");
Entity fixedIntRead = Entity(EntityType::FIXED_INTEGER, "5");
Entity fixedIntAssign = Entity(EntityType::FIXED_INTEGER, "2");
Entity fixedIntWhile = Entity(EntityType::FIXED_INTEGER, "4");
Entity fixedIntCall = Entity(EntityType::FIXED_INTEGER, "12");
Entity fixedIntIf = Entity(EntityType::FIXED_INTEGER, "6");

Entity wc = Entity(EntityType::WILDCARD, "_");



RelationshipRef followsRef_stmtSyn_stmtSyn = RelationshipRef(RelationshipType::FOLLOWS, printSyn, callSyn); // Follows(p, call)
RelationshipRef followsTRef_stmtSyn_fixedInt = RelationshipRef(RelationshipType::FOLLOWS_T, assignSyn, fixedIntCall); // Follows*(a, "12")
RelationshipRef parentRef_stmtSyn_wc = RelationshipRef(RelationshipType::PARENT, whileSyn, wc); //Parent(w, _)
RelationshipRef parentTRef_fixedInt_stmtSyn = RelationshipRef(RelationshipType::PARENT_T, fixedIntWhile, assignSyn);//Parent*("4", a)
RelationshipRef nextRef_fixedInt_fixedInt = RelationshipRef(RelationshipType::NEXT, fixedIntPrint, fixedIntCall); //Next("11", "12")
RelationshipRef nextTRef_fixedInt_wc = RelationshipRef(RelationshipType::NEXT_T, fixedIntStmt, wc); //Next*("1", _);
RelationshipRef affectsRef_wc_stmtSyn = RelationshipRef(RelationshipType::AFFECTS, wc, assignSyn); // Affects(_, a)
RelationshipRef affectsTRef_wc_fixedInt = RelationshipRef(RelationshipType::AFFECTS_T, wc, fixedIntAssign); // Affects*(_, "2")
RelationshipRef followsRef_wc_wc = RelationshipRef(RelationshipType::FOLLOWS, wc, wc); // Follows(_, _)

TEST_CASE("Testing Modifies/Uses Relationships"){

    RelationshipRef modifiesRef_procSyn_varSyn = RelationshipRef(RelationshipType::MODIFIES, procSyn1, varSyn); // Modifies(p1, v)
    RelationshipRef usesRef_fixedStr_varSyn = RelationshipRef(RelationshipType::USES, fixedStrProcF, varSyn); // Uses("f", v)
    RelationshipRef modifiesRef_stmtSyn_varSyn = RelationshipRef(RelationshipType::MODIFIES, readSyn, varSyn); // Modifies(r, v)
    RelationshipRef usesRef_fixedInt_varSyn = RelationshipRef(RelationshipType::USES, fixedIntAssign, varSyn); // Uses("2", v)
    RelationshipRef usesRef_procSyn_fixedStr = RelationshipRef(RelationshipType::USES, procSyn1, fixedStrVarX); // Uses(p1, "x")
    RelationshipRef modifiesRef_fixedStr_fixedStr = RelationshipRef(RelationshipType::MODIFIES, fixedStrProcF, fixedStrVarY); // Modifies("f", "y")
    RelationshipRef usesRef_stmtSyn_fixedStr = RelationshipRef(RelationshipType::USES, ifSyn, fixedStrVarZ); // Uses(ifs, "z")
    RelationshipRef modifiesRef_fixedInt_fixedStr = RelationshipRef(RelationshipType::MODIFIES, fixedIntRead, fixedStrVarZ); // Modifies("5", "z")
    RelationshipRef modifiesRef_procSyn_wc = RelationshipRef(RelationshipType::MODIFIES, procSyn1, wc); // Modifies(p1, _)
    RelationshipRef usesRef_fixedStr_wc = RelationshipRef(RelationshipType::USES, fixedStrProcF, wc); // Uses("f", _)
    RelationshipRef modifiesRef_stmtSyn_wc = RelationshipRef(RelationshipType::MODIFIES, stmtSyn, wc); // Modifies(s, _)
    RelationshipRef usesRef_fixedInt_wc = RelationshipRef(RelationshipType::USES, fixedIntIf, wc); // Uses("6", _)

    DB db;
    PkbGetter pg(&db);
    SuchThatHandler* st = new SuchThatHandler(&pg);

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

    Result expectedResultTrue;
    expectedResultTrue.setValid(true);

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
    result3Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::READ, 5), ProgramElement::createVariable("z")));
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

    Result expectedResult7;
    expectedResult7.setValid(true);
    expectedResult7.setOneSynEntity(ifSyn);
    std::set<ProgramElement> result7Elements;
    result7Elements.insert(ProgramElement::createStatement(ElementType::IF, 6));
    expectedResult7.setOneSynSet(result7Elements);

    Result expectedResult9;
    expectedResult9.setValid(true);
    expectedResult9.setOneSynEntity(procSyn1);
    std::set<ProgramElement> result9Elements;
    result9Elements.insert(ProgramElement::createProcedure("f"));
    expectedResult9.setOneSynSet(result9Elements);

    Result expectedResult11;
    expectedResult11.setValid(true);
    expectedResult11.setOneSynEntity(stmtSyn);
    /*
    std::set<ProgramElement> result11Elements;
    result11Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 1));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 2));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 3));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 5));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 7));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 8));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 9));
    result11Elements.insert(ProgramElement::createStatement(ElementType::kStatement, 10));
    */

    REQUIRE(result1 == expectedResult1);
    //REQUIRE(result2 == expectedResult2);
    //REQUIRE(result3 == expectedResult3);
    //REQUIRE(result4 == expectedResult4);
    //REQUIRE(result5 == expectedResult5);
    //REQUIRE(result6 == expectedResultTrue);
    //REQUIRE(result7 == expectedResult7);
    //REQUIRE(result8 == expectedResultTrue);
    //REQUIRE(result9 == expectedResult9);
    //REQUIRE(result10 == expectedResultTrue);
    //REQUIRE(result11 == expectedResult11);
    //REQUIRE(result12 == expectedResultTrue);
}
/*
TEST_CASE("Testing Calls(*) Relationships"){
    SuchThatHandler* st = new SuchThatHandler(pkb.getPkbGetterStub());

    RelationshipRef callsRef_procSyn_procSyn = RelationshipRef(RelationshipType::Calls, procSyn1, procSyn2); // Calls(p1, p2)
    RelationshipRef callsRef_procSyn_fixedStr = RelationshipRef(RelationshipType::Calls, procSyn1, fixedStrProcG); // Calls(p1, "g")
    RelationshipRef callsRef_procSyn_wc = RelationshipRef(RelationshipType::Calls, procSyn1, wc); // Calls(p1, _)
    RelationshipRef callsRef_fixedStr_procSyn = RelationshipRef(RelationshipType::Calls, fixedStrProcF, procSyn1); // Calls("f", p1)
    RelationshipRef callsRef_fixedStr_fixedStr = RelationshipRef(RelationshipType::Calls, fixedStrProcF, fixedStrProcG); // Calls("f", "g")
    RelationshipRef callsRef_fixedStr_wc = RelationshipRef(RelationshipType::Calls, fixedStrProcF, wc); // Calls("f", _)
    RelationshipRef callsRef_wc_procSyn = RelationshipRef(RelationshipType::Calls, wc, procSyn1); // Calls(_, p1)
    RelationshipRef callsRef_wc_fixedStr = RelationshipRef(RelationshipType::Calls, wc, fixedStrProcG); // Calls(_, "g")
    RelationshipRef callsRef_wc_wc = RelationshipRef(RelationshipType::Calls, wc, wc); // Calls (_, _)

    Result result1 = st->handleSuchThat(callsRef_procSyn_procSyn);
    Result result2 = st->handleSuchThat(callsRef_procSyn_fixedStr);
    Result result3 = st->handleSuchThat(callsRef_procSyn_wc);
    Result result4 = st->handleSuchThat(callsRef_fixedStr_procSyn);
    Result result5 = st->handleSuchThat(callsRef_fixedStr_fixedStr);
    Result result6 = st->handleSuchThat(callsRef_fixedStr_wc);
    Result result7 = st->handleSuchThat(callsRef_wc_procSyn);
    Result result8 = st->handleSuchThat(callsRef_wc_fixedStr);
    Result result9 = st->handleSuchThat(callsRef_wc_wc);

    Result expectedResultTrue;
    expectedResultTrue.setValid(true);

    Result expectedResultG;
    expectedResultG.setValid(true);
    expectedResultG.setOneSynEntity(procSyn1);
    std::set<ProgramElement> g;
    g.insert(ProgramElement::createProcedure("g"));
    expectedResultG.setOneSynSet(g);

    Result expectedResultF;
    expectedResultF.setValid(true);
    expectedResultF.setOneSynEntity(procSyn1);
    std::set<ProgramElement> f;
    f.insert(ProgramElement::createProcedure("f"));
    expectedResultF.setOneSynSet(f);

    Result expectedResultFG;
    expectedResultFG.setValid(true);
    expectedResultFG.setTwoSynEntities(std::pair<Entity, Entity>(procSyn1, procSyn2));
    std::set<std::pair<ProgramElement, ProgramElement>> fg;
    fg.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createProcedure("g")));
    expectedResultFG.setTwoSynSet(fg);

    REQUIRE(result1 == expectedResultFG);
    REQUIRE(result2 == expectedResultF);
    REQUIRE(result3 == expectedResultF);
    REQUIRE(result4 == expectedResultG);
    REQUIRE(result5 == expectedResultTrue);
    REQUIRE(result6 == expectedResultTrue);
    REQUIRE(result7 == expectedResultG);
    REQUIRE(result8 == expectedResultTrue);
    REQUIRE(result9 == expectedResultTrue);
}

TEST_CASE("Testing Follows(*)/Parent(*)/Next(*)/Affects(*) Relationships"){
    SuchThatHandler* st = new SuchThatHandler(pkb.getPkbGetterStub());

    RelationshipRef followsRef_stmtSyn_stmtSyn = RelationshipRef(RelationshipType::Follows, printSyn, callSyn); // Follows(p, call)
    RelationshipRef followsTRef_stmtSyn_fixedInt = RelationshipRef(RelationshipType::FOLLOWS_T, assignSyn, fixedIntCall); // Follows*(a, "12")
    RelationshipRef parentRef_stmtSyn_wc = RelationshipRef(RelationshipType::Parent, whileSyn, wc); //Parent(w, _)
    RelationshipRef parentTRef_fixedInt_stmtSyn = RelationshipRef(RelationshipType::PARENT_T, fixedIntWhile,  assignSyn);//Parent*("4", a)
    RelationshipRef nextRef_fixedInt_fixedInt = RelationshipRef(RelationshipType::Next, fixedIntPrint, fixedIntCall); //Next("11", "12")
    RelationshipRef nextTRef_fixedInt_wc = RelationshipRef(RelationshipType::NEXT_T, fixedIntStmt, wc); //Next*("1", _);
    RelationshipRef affectsRef_wc_stmtSyn = RelationshipRef(RelationshipType::Affects, wc, assignSyn); // Affects(_, a)
    RelationshipRef affectsTRef_wc_fixedInt = RelationshipRef(RelationshipType::AFFECTS_T, wc, fixedIntAssign); // Affects*(_, "2")
    RelationshipRef followsRef_wc_wc = RelationshipRef(RelationshipType::Follows, wc, wc); // Follows(_, _)

    Result result1 = st->handleSuchThat(followsRef_stmtSyn_stmtSyn);
    Result result2 = st->handleSuchThat(followsTRef_stmtSyn_fixedInt);
    Result result3 = st->handleSuchThat(parentRef_stmtSyn_wc);
    Result result4 = st->handleSuchThat(parentTRef_fixedInt_stmtSyn);
    Result result5 = st->handleSuchThat(nextRef_fixedInt_fixedInt);
    Result result6 = st->handleSuchThat(nextTRef_fixedInt_wc);
    Result result7 = st->handleSuchThat(affectsRef_wc_stmtSyn);
    Result result8 = st->handleSuchThat(affectsTRef_wc_fixedInt);
    Result result9 = st->handleSuchThat(followsRef_wc_wc);

    Result expectedResultTrue;
    expectedResultTrue.setValid(true);

    Result expectedResult1;
    expectedResult1.setValid(true);
    expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(printSyn, callSyn));
    std::set<std::pair<ProgramElement, ProgramElement>> result1Elements;
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::kPrint, 11), ProgramElement::createStatement(ElementType::kCall, 12)));
    expectedResult1.setTwoSynSet(result1Elements);

    Result expectedResult2;
    expectedResult2.setValid(true);
    expectedResult2.setOneSynEntity(assignSyn);
    std::set<ProgramElement> result2Elements;
    result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 1));
    result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 2));
    result2Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 3));
    expectedResult2.setOneSynSet(result2Elements);

    Result expectedResult3;
    expectedResult3.setValid(true);
    expectedResult3.setOneSynEntity(whileSyn);
    std::set<ProgramElement> result3Elements;
    result3Elements.insert(ProgramElement::createStatement(ElementType::kWhile, 4));
    expectedResult3.setOneSynSet(result3Elements);

    Result expectedResult4;
    expectedResult4.setValid(true);
    expectedResult4.setOneSynEntity(assignSyn);
    std::set<ProgramElement> result4Elements;
    result4Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 7));
    result4Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 8));
    result4Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 9));
    result4Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 10));
    expectedResult4.setOneSynSet(result4Elements);
    
    Result expectedResult7;
    expectedResult7.setValid(true);
    expectedResult7.setOneSynEntity(assignSyn);
    std::set<ProgramElement> result7Elements;
    result7Elements.insert(ProgramElement::createStatement(ElementType::kAssignment, 2));
    expectedResult7.setOneSynSet(result7Elements);

    REQUIRE(result1 == expectedResult1);
    REQUIRE(result2 == expectedResult2);
    REQUIRE(result3 == expectedResult3);
    REQUIRE(result4 == expectedResult4);
    REQUIRE(result5 == expectedResultTrue);
    REQUIRE(result6 == expectedResultTrue);
    REQUIRE(result7 == expectedResult7);
    REQUIRE(result8 == expectedResultTrue);
    REQUIRE(result9 == expectedResultTrue);
}
 */