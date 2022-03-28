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
#include "Result.h"

//create entities

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

//create ProgramElements
ProgramElement s1 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 1);
ProgramElement s2 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 2);
ProgramElement s3 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 3);
ProgramElement s4 = ProgramElement::createStatement(ElementType::WHILE, 4);
ProgramElement s5 = ProgramElement::createStatement(ElementType::READ, 5, "z");
ProgramElement s6 = ProgramElement::createStatement(ElementType::IF, 6);
ProgramElement s7 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 7);
ProgramElement s8 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 8);
ProgramElement s9 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 9);
ProgramElement s10 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 10);
ProgramElement s11 = ProgramElement::createStatement(ElementType::PRINT, 11, "x");

//create Results


TEST_CASE("Test case 1"){

/*
    SECTION("Merger 2 1 syn"){
        Table t1(result2);
        Table t2(result2);
        Table tResult(t1, t2);

    }
    */

}