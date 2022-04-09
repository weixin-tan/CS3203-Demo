//
// Created by Prerthan Munireternam on 27/03/2022.
//
#include <vector>
#include <set>
#include "catch.hpp"
#include "Table.h"
#include "TableRow.h"
#include "Result.h"


//create entities

Entity procSyn1Table = Entity(EntityType::PROCEDURE, "p1");
Entity procSyn2Table = Entity(EntityType::PROCEDURE, "p2");

Entity varSynTable = Entity(EntityType::VARIABLE, "v");
Entity constSynTable = Entity(EntityType::CONSTANT, "c");

Entity stmtSynTable = Entity(EntityType::STATEMENT, "s");
Entity printSynTable = Entity(EntityType::PRINT, "p");
Entity readSynTable = Entity(EntityType::READ, "r");
Entity assignSynTable = Entity(EntityType::ASSIGNMENT, "a");
Entity callSynTable = Entity(EntityType::CALL, "call");
Entity whileSynTable = Entity(EntityType::WHILE, "w");
Entity ifSynTable = Entity(EntityType::IF, "ifs");

Entity fixedStrVarXTable = Entity(EntityType::FIXED_STRING, "x");
Entity fixedStrVarYTable = Entity(EntityType::FIXED_STRING, "y");
Entity fixedStrVarZTable = Entity(EntityType::FIXED_STRING, "z");
Entity fixedStrProcFTable = Entity(EntityType::FIXED_STRING, "f");
Entity fixedStrProcGTable = Entity(EntityType::FIXED_STRING, "g");

Entity fixedIntStmtTable = Entity(EntityType::FIXED_INTEGER, "1");
Entity fixedIntPrintTable = Entity(EntityType::FIXED_INTEGER, "11");
Entity fixedIntReadTable = Entity(EntityType::FIXED_INTEGER, "5");
Entity fixedIntAssignTable = Entity(EntityType::FIXED_INTEGER, "2");
Entity fixedIntWhileTable = Entity(EntityType::FIXED_INTEGER, "4");
Entity fixedIntCallTable = Entity(EntityType::FIXED_INTEGER, "12");
Entity fixedIntIfTable = Entity(EntityType::FIXED_INTEGER, "6");

Entity wcTable = Entity(EntityType::WILDCARD, "_");

//create ProgramElements
ProgramElement s1Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 1);
ProgramElement s2Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 2);
ProgramElement s3Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 3);
ProgramElement s4Table = ProgramElement::createStatement(ElementType::WHILE, 4);
ProgramElement s5Table = ProgramElement::createStatement(ElementType::READ, 5, "z");
ProgramElement s6Table = ProgramElement::createStatement(ElementType::IF, 6);
ProgramElement s7Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 7);
ProgramElement s8Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 8);
ProgramElement s9Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 9);
ProgramElement s10Table = ProgramElement::createStatement(ElementType::ASSIGNMENT, 10);
ProgramElement s11Table = ProgramElement::createStatement(ElementType::PRINT, 11, "x");


TEST_CASE("creating results"){

}