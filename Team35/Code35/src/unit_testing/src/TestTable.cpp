//
// Created by Prerthan Munireternam on 27/03/2022.
//
#include <vector>
#include <set>
#include "catch.hpp"
#include "Table.h"
#include "TableRow.h"

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



TEST_CASE("Test case 1"){

    //create results
    Result result1;
    result1.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result1.setValid(true);
    result1.setTwoSynEntities(std::pair<Entity, Entity>(procSyn1, varSyn));
    std::set<std::pair<ProgramElement, ProgramElement>> result1Elements;
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("x")));
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("y")));
    result1Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("z")));
    result1.setTwoSynSet(result1Elements);

    Result result2;
    result2.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result2.setValid(true);
    result2.setOneSynEntity(varSyn);
    std::set<ProgramElement> result2Elements;
    result2Elements.insert(ProgramElement::createVariable("x"));
    result2Elements.insert(ProgramElement::createVariable("z"));
    result2.setOneSynSet(result2Elements);

    Result result3;
    result3.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result3.setValid(true);
    result3.setTwoS2ynEntities(std::pair<Entity, Entity>(readSyn, varSyn));
    std::set<std::pair<ProgramElement, ProgramElement>> result3Elements;
    result3Elements.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::READ, 5), ProgramElement::createVariable("z")));
    result3.setTwoSynSet(result3Elements);

    Result result4;
    result4.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result4.setValid(true);
    result4.setOneSynEntity(varSyn);
    std::set<ProgramElement> result4Elements;
    result4Elements.insert(ProgramElement::createVariable("x"));
    result4.setOneSynSet(result4Elements);

    Result result5;
    result5.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result5.setValid(true);
    result5.setOneSynEntity(procSyn1);
    std::set<ProgramElement> result5Elements;
    result5Elements.insert(ProgramElement::createProcedure("f"));
    result5.setOneSynSet(result5Elements);

    Result result6;
    result6.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result6.setValid(true);
    result6.setOneSynEntity(ifSyn);
    std::set<ProgramElement> result6Elements;
    result6Elements.insert(ProgramElement::createStatement(ElementType::IF, 6));
    result6.setOneSynSet(result6Elements);

    Result result7;
    result7.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result7.setValid(true);
    result7.setOneSynEntity(procSyn1);
    std::set<ProgramElement> result7Elements;
    result7Elements.insert(ProgramElement::createProcedure("f"));
    result7.setOneSynSet(result7Elements);

    Result result8;
    result8.setResultType(ResultType::SUCH_THAT_CLAUSE);
    result8.setValid(true);
    result8.setOneSynEntity(stmtSyn);
    std::set<ProgramElement> result8Elements;
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 1));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 2));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 3));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 5));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 7));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 8));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 9));
    result8Elements.insert(ProgramElement::createStatement(ElementType::STATEMENT, 10));
    result8.setOneSynSet(result8Elements);

    /*
     * r1 - 2
     * r2 - 1
     * r3 - 2
     * r4 - 1
     * r5 - 1
     * r6 - 
     */


    SECTION("Merger 2 1 syn"){
        Table t1(result2);
        Table t2(result2);
        Table tResult(t1, t2);

    }

}