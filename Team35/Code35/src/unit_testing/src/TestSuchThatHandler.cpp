#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "Clause.h"
#include "catch.hpp"
#include "SuchThatHandler.h"
#include "PKB.h"

class PkbGetterStub : public PkbGetter {

};

class PkbStub : private PKB {
public:
    PkbStub();

    PkbGetterStub* pg;
};

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

PkbStub pkb;
SuchThatHandler* st = new SuchThatHandler(pkb.pg);

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

TEST_CASE("testing valid queries on parseQuery"){
    //st->handleSuchThat()
}
