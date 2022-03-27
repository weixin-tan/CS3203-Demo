//
// Created by Prerthan Munireternam on 27/03/2022.
//
#include <vector>
#include <set>

#include "catch.hpp"
#include "QPS/Optimiser.h"
#include "QPS/QueryProcessor.h"
#include "QPS/QPSMainLogic.h"
#include "Table.h"
#include "PkbGetterStubForTestSuchThatHandler.h"



//inputs
Entity TprocSyn1 = Entity(EntityType::PROCEDURE, "p1");
Entity TprocSyn2 = Entity(EntityType::PROCEDURE, "p2");

Entity TvarSyn = Entity(EntityType::VARIABLE, "v");
Entity TconstSyn = Entity(EntityType::CONSTANT, "c");

Entity TstmtSyn = Entity(EntityType::STATEMENT, "s");
Entity TprintSyn = Entity(EntityType::PRINT, "p");
Entity TreadSyn = Entity(EntityType::READ, "r");
Entity TassignSyn = Entity(EntityType::ASSIGNMENT, "a");
Entity TcallSyn = Entity(EntityType::CALL, "call");
Entity TwhileSyn = Entity(EntityType::WHILE, "w");
Entity TifSyn = Entity(EntityType::IF, "ifs");

Entity TfixedStrVarX = Entity(EntityType::FIXED_STRING, "x");
Entity TfixedStrVarY = Entity(EntityType::FIXED_STRING, "y");
Entity TfixedStrVarZ = Entity(EntityType::FIXED_STRING, "z");
Entity TfixedStrProcF = Entity(EntityType::FIXED_STRING, "f");
Entity TfixedStrProcG = Entity(EntityType::FIXED_STRING, "g");

Entity TfixedIntStmt = Entity(EntityType::FIXED_INTEGER, "1");
Entity TfixedIntPrint = Entity(EntityType::FIXED_INTEGER, "11");
Entity TfixedIntRead = Entity(EntityType::FIXED_INTEGER, "5");
Entity TfixedIntAssign = Entity(EntityType::FIXED_INTEGER, "2");
Entity TfixedIntWhile = Entity(EntityType::FIXED_INTEGER, "4");
Entity TfixedIntCall = Entity(EntityType::FIXED_INTEGER, "12");
Entity TfixedIntIf = Entity(EntityType::FIXED_INTEGER, "6");

Entity Twc = Entity(EntityType::WILDCARD, "_");


TEST_CASE("Test case 1"){


    DB db;
    PkbGetter* pg = new PkbGetterStubForTestSuchThatHandler(&db);
    SuchThatHandler* st = new SuchThatHandler(pg);

    RelationshipRef modifiesRef_procSyn_varSyn = RelationshipRef(RelationshipType::MODIFIES, TprocSyn1, TvarSyn); // Modifies(p1, v)
    RelationshipRef usesRef_fixedStr_varSyn = RelationshipRef(RelationshipType::USES, TfixedStrProcF, TvarSyn); // Uses("f", v)
    RelationshipRef modifiesRef_stmtSyn_varSyn = RelationshipRef(RelationshipType::MODIFIES, TreadSyn, TvarSyn); // Modifies(r, v)
    RelationshipRef usesRef_fixedInt_varSyn = RelationshipRef(RelationshipType::USES, TfixedIntAssign, TvarSyn); // Uses("2", v)
    RelationshipRef usesRef_procSyn_fixedStr = RelationshipRef(RelationshipType::USES, TprocSyn1, TfixedStrVarX); // Uses(p1, "x")
    RelationshipRef modifiesRef_fixedStr_fixedStr = RelationshipRef(RelationshipType::MODIFIES, TfixedStrProcF, TfixedStrVarY); // Modifies("f", "y")
    RelationshipRef usesRef_stmtSyn_fixedStr = RelationshipRef(RelationshipType::USES, TifSyn, TfixedStrVarZ); // Uses(ifs, "z")
    RelationshipRef modifiesRef_fixedInt_fixedStr = RelationshipRef(RelationshipType::MODIFIES, TfixedIntRead, TfixedStrVarZ); // Modifies("5", "z")
    RelationshipRef modifiesRef_procSyn_wc = RelationshipRef(RelationshipType::MODIFIES, TprocSyn1, Twc); // Modifies(p1, _)
    RelationshipRef usesRef_fixedStr_wc = RelationshipRef(RelationshipType::USES, TfixedStrProcF, Twc); // Uses("f", _)
    RelationshipRef modifiesRef_stmtSyn_wc = RelationshipRef(RelationshipType::MODIFIES, TstmtSyn, Twc); // Modifies(s, _)
    RelationshipRef usesRef_fixedInt_wc = RelationshipRef(RelationshipType::USES, TfixedIntIf, Twc); // Uses("6", _)

    //get Results
    Result result1 = st->handleSuchThat(modifiesRef_procSyn_varSyn);// Modifies(p1, v) 2
    Result result2 = st->handleSuchThat(usesRef_fixedStr_varSyn);// Uses("f", v) 1
    Result result3 = st->handleSuchThat(modifiesRef_stmtSyn_varSyn);// Modifies(r, v) 2
    Result result4 = st->handleSuchThat(usesRef_fixedInt_varSyn);// Uses("2", v) 1
    Result result5 = st->handleSuchThat(usesRef_procSyn_fixedStr);// Uses(p1, "x") 1
    Result result6 = st->handleSuchThat(modifiesRef_fixedStr_fixedStr);// Modifies("f", "y") True
    Result result7 = st->handleSuchThat(usesRef_stmtSyn_fixedStr);// Uses(ifs, "z") 1
    Result result8 = st->handleSuchThat(modifiesRef_fixedInt_fixedStr);// Modifies("5", "z") True
    Result result9 = st->handleSuchThat(modifiesRef_procSyn_wc);// Modifies(p1, _) 1
    Result result10 = st->handleSuchThat(usesRef_fixedStr_wc);// Uses("f", _) True
    Result result11 = st->handleSuchThat(modifiesRef_stmtSyn_wc);// Modifies(s, _) 1
    Result result12 = st->handleSuchThat(usesRef_fixedInt_wc);// Uses("6", _) True


    SECTION("Table with 1 element"){
        //Create Table
        Table t;
        //Table with 1 element
        Entity result2Head = result2.getOneSynEntity();
        std::vector<ProgramElement> result2Body(result2.getOneSynSet().begin(),result2.getOneSynSet().end());
        t.inputElement(result2Head);
        t.inputProgramElements(result2Body);
        //test
        REQUIRE(t.getHeader()[0] == result2Head);
        REQUIRE(t.getBody()[0] == result2Body);
        REQUIRE(t.getIndexElement(result2Body, result2Body[0]) == 0);
        //clear
        t.makeTableEmpty();
        REQUIRE((t.getHeader()).size() == 0);
        REQUIRE(t.getBody().size() == 0);
    }

    SECTION("Table with 2 element"){
        
    }


}