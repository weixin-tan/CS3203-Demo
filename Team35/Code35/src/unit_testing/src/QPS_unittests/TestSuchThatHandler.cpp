#include <vector>

#include "QPS/Clause.h"
#include "catch.hpp"
#include "QPS/SuchThatHandler.h"
#include "PkbGetterStub.h"
#include "QPSUtil.h"

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
//procedure g{
// }

// Modifies, Uses are similar in implementation
// Follows, Follows*, Parent, Parent*, Next, Next*, Affects, Affects* are similar in implementation
// Calls, Calls* are similar in implementation

QPS_GENERAL_DATA_INFO entInfoST;
QPS_SOURCE_INFO sourceInfoST;

TEST_CASE("Testing Modifies/Uses Relationships"){

    DB db;
    PkbGetter* pg = new PkbGetterStub(&db);
    SuchThatHandler* st = new SuchThatHandler(pg);

    RelationshipRef modifiesRef_procSyn_varSyn = RelationshipRef(RelationshipType::MODIFIES, entInfoST.procSyn1, entInfoST.varSyn); // Modifies(p1, v)
    RelationshipRef usesRef_fixedStr_varSyn = RelationshipRef(RelationshipType::USES, entInfoST.fixedStringF, entInfoST.varSyn); // Uses("f", v)
    RelationshipRef modifiesRef_stmtSyn_varSyn = RelationshipRef(RelationshipType::MODIFIES, entInfoST.readSyn, entInfoST.varSyn); // Modifies(r, v)
    RelationshipRef usesRef_fixedInt_varSyn = RelationshipRef(RelationshipType::USES, entInfoST.fixedInt2, entInfoST.varSyn); // Uses("2", v)
    RelationshipRef usesRef_procSyn_fixedStr = RelationshipRef(RelationshipType::USES, entInfoST.procSyn1, entInfoST.fixedStringX); // Uses(p1, "x")
    RelationshipRef modifiesRef_fixedStr_fixedStr = RelationshipRef(RelationshipType::MODIFIES, entInfoST.fixedStringF, entInfoST.fixedStringY); // Modifies("f", "y")
    RelationshipRef usesRef_stmtSyn_fixedStr = RelationshipRef(RelationshipType::USES, entInfoST.ifSyn, entInfoST.fixedStringZ); // Uses(ifs, "z")
    RelationshipRef modifiesRef_fixedInt_fixedStr = RelationshipRef(RelationshipType::MODIFIES, entInfoST.fixedInt5, entInfoST.fixedStringZ); // Modifies("5", "z")
    RelationshipRef modifiesRef_procSyn_wc = RelationshipRef(RelationshipType::MODIFIES, entInfoST.procSyn1, entInfoST.wildcard); // Modifies(p1, _)
    RelationshipRef usesRef_fixedStr_wc = RelationshipRef(RelationshipType::USES, entInfoST.fixedStringF, entInfoST.wildcard); // Uses("f", _)
    RelationshipRef modifiesRef_stmtSyn_wc = RelationshipRef(RelationshipType::MODIFIES, entInfoST.stmtSyn, entInfoST.wildcard); // Modifies(s, _)
    RelationshipRef usesRef_fixedInt_wc = RelationshipRef(RelationshipType::USES, entInfoST.fixedInt6, entInfoST.wildcard); // Uses("6", _)


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
    expectedResultTrue.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResultTrue.setValid(true);

    Result expectedResult1;
    expectedResult1.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult1.setValid(true);
    expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(entInfoST.procSyn1, entInfoST.varSyn));
    std::set<std::pair<ProgramElement*, ProgramElement*>> result1Elements = {{&sourceInfoST.procF, &sourceInfoST.varX}, {&sourceInfoST.procF, &sourceInfoST.varY}, {&sourceInfoST.procF, &sourceInfoST.varZ}};
    expectedResult1.setTwoSynSet(result1Elements);

    Result expectedResult2;
    expectedResult2.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult2.setValid(true);
    expectedResult2.setOneSynEntity(entInfoST.varSyn);
    std::set<ProgramElement*> result2Elements = {&sourceInfoST.varX, &sourceInfoST.varZ};
    expectedResult2.setOneSynSet(result2Elements);

    Result expectedResult3;
    expectedResult3.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult3.setValid(true);
    expectedResult3.setTwoSynEntities(std::pair<Entity, Entity>(entInfoST.readSyn, entInfoST.varSyn));
    std::set<std::pair<ProgramElement*, ProgramElement*>> result3Elements = {{&sourceInfoST.read5, &sourceInfoST.varZ}};
    expectedResult3.setTwoSynSet(result3Elements);

    Result expectedResult4;
    expectedResult4.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult4.setValid(true);
    expectedResult4.setOneSynEntity(entInfoST.varSyn);
    std::set<ProgramElement*> result4Elements = {&sourceInfoST.varX};
    expectedResult4.setOneSynSet(result4Elements);

    Result expectedResult5;
    expectedResult5.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult5.setValid(true);
    expectedResult5.setOneSynEntity(entInfoST.procSyn1);
    std::set<ProgramElement*> result5Elements = {&sourceInfoST.procF};
    expectedResult5.setOneSynSet(result5Elements);

    Result expectedResult7;
    expectedResult7.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult7.setValid(true);
    expectedResult7.setOneSynEntity(entInfoST.ifSyn);
    std::set<ProgramElement*> result7Elements = {&sourceInfoST.if6};
    expectedResult7.setOneSynSet(result7Elements);

    Result expectedResult9;
    expectedResult9.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult9.setValid(true);
    expectedResult9.setOneSynEntity(entInfoST.procSyn1);
    std::set<ProgramElement*> result9Elements = {&sourceInfoST.procF};
    expectedResult9.setOneSynSet(result9Elements);

    Result expectedResult11;
    expectedResult11.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult11.setValid(true);
    expectedResult11.setOneSynEntity(entInfoST.stmtSyn);
    std::set<ProgramElement*> result11Elements = {&sourceInfoST.stmt1, &sourceInfoST.stmt2, &sourceInfoST.stmt3, &sourceInfoST.stmt5,
                                                  &sourceInfoST.stmt7, &sourceInfoST.stmt8, &sourceInfoST.stmt9, &sourceInfoST.stmt10};
    expectedResult11.setOneSynSet(result11Elements);

    REQUIRE(result1 == expectedResult1);
    REQUIRE(result2 == expectedResult2);
    REQUIRE(result3 == expectedResult3);
    REQUIRE(result4 == expectedResult4);
    REQUIRE(result5 == expectedResult5);
    REQUIRE(result6 == expectedResultTrue);
    REQUIRE(result7 == expectedResult7);
    REQUIRE(result8 == expectedResultTrue);
    REQUIRE(result9 == expectedResult9);
    REQUIRE(result10 == expectedResultTrue);
    REQUIRE(result11 == expectedResult11);
    REQUIRE(result12 == expectedResultTrue);
}

TEST_CASE("Testing Calls(*) Relationships"){
    DB db;
    PkbGetter* pg = new PkbGetterStub(&db);
    SuchThatHandler* st = new SuchThatHandler(pg);

    RelationshipRef callsRef_procSyn_procSyn = RelationshipRef(RelationshipType::CALLS, entInfoST.procSyn1, entInfoST.procSyn2); // Calls(p1, p2)
    RelationshipRef callsRef_procSyn_fixedStr = RelationshipRef(RelationshipType::CALLS, entInfoST.procSyn1, entInfoST.fixedStringG); // Calls(p1, "g")
    RelationshipRef callsRef_procSyn_wc = RelationshipRef(RelationshipType::CALLS, entInfoST.procSyn1, entInfoST.wildcard); // Calls(p1, _)
    RelationshipRef callsRef_fixedStr_procSyn = RelationshipRef(RelationshipType::CALLS, entInfoST.fixedStringF, entInfoST.procSyn1); // Calls("f", p1)
    RelationshipRef callsRef_fixedStr_fixedStr = RelationshipRef(RelationshipType::CALLS, entInfoST.fixedStringF, entInfoST.fixedStringG); // Calls("f", "g")
    RelationshipRef callsRef_fixedStr_wc = RelationshipRef(RelationshipType::CALLS, entInfoST.fixedStringF, entInfoST.wildcard); // Calls("f", _)
    RelationshipRef callsRef_wc_procSyn = RelationshipRef(RelationshipType::CALLS, entInfoST.wildcard, entInfoST.procSyn1); // Calls(_, p1)
    RelationshipRef callsRef_wc_fixedStr = RelationshipRef(RelationshipType::CALLS, entInfoST.wildcard, entInfoST.fixedStringG); // Calls(_, "g")
    RelationshipRef callsRef_wc_wc = RelationshipRef(RelationshipType::CALLS, entInfoST.wildcard, entInfoST.wildcard); // Calls (_, _)

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
    expectedResultTrue.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResultTrue.setValid(true);

    Result expectedResultG;
    expectedResultG.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResultG.setValid(true);
    expectedResultG.setOneSynEntity(entInfoST.procSyn1);
    std::set<ProgramElement*> g = {&sourceInfoST.procG};
    expectedResultG.setOneSynSet(g);

    Result expectedResultF;
    expectedResultF.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResultF.setValid(true);
    expectedResultF.setOneSynEntity(entInfoST.procSyn1);
    std::set<ProgramElement*> f = {&sourceInfoST.procF};
    expectedResultF.setOneSynSet(f);

    Result expectedResultFG;
    expectedResultFG.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResultFG.setValid(true);
    expectedResultFG.setTwoSynEntities(std::pair<Entity, Entity>(entInfoST.procSyn1, entInfoST.procSyn2));
    std::set<std::pair<ProgramElement*, ProgramElement*>> fg = {{&sourceInfoST.procF, &sourceInfoST.procG}};
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
    DB db;
    PkbGetter* pg = new PkbGetterStub(&db);
    SuchThatHandler* st = new SuchThatHandler(pg);

    RelationshipRef followsRef_stmtSyn_stmtSyn = RelationshipRef(RelationshipType::FOLLOWS, entInfoST.printSyn, entInfoST.callSyn); // Follows(p, call)
    RelationshipRef followsTRef_stmtSyn_fixedInt = RelationshipRef(RelationshipType::FOLLOWS_T, entInfoST.assignSyn, entInfoST.fixedInt12); // Follows*(a, "12")
    RelationshipRef parentRef_stmtSyn_wc = RelationshipRef(RelationshipType::PARENT, entInfoST.whileSyn, entInfoST.wildcard); //Parent(w, _)
    RelationshipRef parentTRef_fixedInt_stmtSyn = RelationshipRef(RelationshipType::PARENT_T, entInfoST.fixedInt4,  entInfoST.assignSyn);//Parent*("4", a)
    RelationshipRef nextRef_fixedInt_fixedInt = RelationshipRef(RelationshipType::NEXT, entInfoST.fixedInt11, entInfoST.fixedInt12); //Next("11", "12")
    RelationshipRef nextTRef_fixedInt_wc = RelationshipRef(RelationshipType::NEXT_T, entInfoST.fixedInt1, entInfoST.wildcard); //Next*("1", _);
    RelationshipRef affectsRef_wc_stmtSyn = RelationshipRef(RelationshipType::AFFECTS, entInfoST.wildcard, entInfoST.stmtSyn); // Affects(_, s)
    RelationshipRef affectsTRef_wc_wc = RelationshipRef(RelationshipType::AFFECTS_T, entInfoST.wildcard, entInfoST.wildcard); // Affects*(_, _)
    RelationshipRef nextRef_wc_fixedInt = RelationshipRef(RelationshipType::NEXT, entInfoST.wildcard, entInfoST.fixedInt2); // Next(_, "2")

    Result result1 = st->handleSuchThat(followsRef_stmtSyn_stmtSyn);
    Result result2 = st->handleSuchThat(followsTRef_stmtSyn_fixedInt);
    Result result3 = st->handleSuchThat(parentRef_stmtSyn_wc);
    Result result4 = st->handleSuchThat(parentTRef_fixedInt_stmtSyn);
    Result result5 = st->handleSuchThat(nextRef_fixedInt_fixedInt);
    Result result6 = st->handleSuchThat(nextTRef_fixedInt_wc);
    Result result7 = st->handleSuchThat(affectsRef_wc_stmtSyn);
    Result result8 = st->handleSuchThat(affectsTRef_wc_wc);
    Result result9 = st->handleSuchThat(nextRef_wc_fixedInt);

    Result expectedResultTrue;
    expectedResultTrue.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResultTrue.setValid(true);

    Result expectedResult1;
    expectedResult1.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult1.setValid(true);
    expectedResult1.setTwoSynEntities(std::pair<Entity, Entity>(entInfoST.printSyn, entInfoST.callSyn));
    std::set<std::pair<ProgramElement*, ProgramElement*>> result1Elements = {{&sourceInfoST.print11, &sourceInfoST.call12}};
    expectedResult1.setTwoSynSet(result1Elements);

    Result expectedResult2;
    expectedResult2.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult2.setValid(true);
    expectedResult2.setOneSynEntity(entInfoST.assignSyn);
    std::set<ProgramElement*> result2Elements = {&sourceInfoST.assign1, &sourceInfoST.assign2, &sourceInfoST.assign3};
    expectedResult2.setOneSynSet(result2Elements);

    Result expectedResult3;
    expectedResult3.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult3.setValid(true);
    expectedResult3.setOneSynEntity(entInfoST.whileSyn);
    std::set<ProgramElement*> result3Elements = {&sourceInfoST.while4};
    expectedResult3.setOneSynSet(result3Elements);

    Result expectedResult4;
    expectedResult4.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult4.setValid(true);
    expectedResult4.setOneSynEntity(entInfoST.assignSyn);
    std::set<ProgramElement*> result4Elements = {&sourceInfoST.assign7, &sourceInfoST.assign8, &sourceInfoST.assign9, &sourceInfoST.assign10};
    expectedResult4.setOneSynSet(result4Elements);
    
    Result expectedResult7;
    expectedResult7.setResultType(ResultType::SUCH_THAT_CLAUSE);
    expectedResult7.setValid(true);
    expectedResult7.setOneSynEntity(entInfoST.stmtSyn);
    std::set<ProgramElement*> result7Elements = {&sourceInfoST.stmt2, &sourceInfoST.stmt9};
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
