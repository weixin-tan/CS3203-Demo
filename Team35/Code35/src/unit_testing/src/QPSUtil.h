#ifndef SPA_QPSUTIL_H
#define SPA_QPSUTIL_H

#include <set>
#include "ProgramElement.h"
#include "Entity.h"

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

// Source for pattern partial and full match testing
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

struct QPS_SOURCE_INFO {
    ProgramElement procF = ProgramElement::createProcedure("f");
    ProgramElement procG = ProgramElement::createProcedure("g");

    ProgramElement varX = ProgramElement::createVariable("x");
    ProgramElement varY = ProgramElement::createVariable("y");
    ProgramElement varZ = ProgramElement::createVariable("z");

    ProgramElement const0 = ProgramElement::createConstant("0");
    ProgramElement const4 = ProgramElement::createConstant("4");
    ProgramElement const5 = ProgramElement::createConstant("5");
    ProgramElement const10 = ProgramElement::createConstant("10");
    ProgramElement const100 = ProgramElement::createConstant("100");

    ProgramElement stmt1 = ProgramElement::createStatement(ElementType::STATEMENT, 1);
    ProgramElement stmt2 = ProgramElement::createStatement(ElementType::STATEMENT, 2);
    ProgramElement stmt3 = ProgramElement::createStatement(ElementType::STATEMENT, 3);
    ProgramElement stmt4 = ProgramElement::createStatement(ElementType::STATEMENT, 4);
    ProgramElement stmt5 = ProgramElement::createStatement(ElementType::STATEMENT, 5);
    ProgramElement stmt6 = ProgramElement::createStatement(ElementType::STATEMENT, 6);
    ProgramElement stmt7 = ProgramElement::createStatement(ElementType::STATEMENT, 7);
    ProgramElement stmt8 = ProgramElement::createStatement(ElementType::STATEMENT, 8);
    ProgramElement stmt9 = ProgramElement::createStatement(ElementType::STATEMENT, 9);
    ProgramElement stmt10 = ProgramElement::createStatement(ElementType::STATEMENT, 10);
    ProgramElement stmt11 = ProgramElement::createStatement(ElementType::STATEMENT, 11);
    ProgramElement stmt12 = ProgramElement::createStatement(ElementType::STATEMENT, 12);

    ProgramElement assign1 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 1);
    ProgramElement assign2 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 2);
    ProgramElement assign3 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 3);
    ProgramElement assign7 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 7);
    ProgramElement assign8 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 8);
    ProgramElement assign9 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 9);
    ProgramElement assign10 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 10);

    ProgramElement print11 = ProgramElement::createStatement(ElementType::PRINT, 11, "x");

    ProgramElement while4 = ProgramElement::createStatement(ElementType::WHILE, 4);

    ProgramElement read5 = ProgramElement::createStatement(ElementType::READ, 5, "x");

    ProgramElement call12 = ProgramElement::createStatement(ElementType::CALL, 12, "g");

    ProgramElement if6 = ProgramElement::createStatement(ElementType::IF, 6);

    std::set<ProgramElement*> procedures = {&procF, &procG};
    std::set<ProgramElement*> variables = {&varX, &varY, &varZ};
    std::set<ProgramElement*> constants = {&const0, &const4, &const5, &const10, &const100};
    std::set<ProgramElement*> stmts = {&stmt1, &stmt2, &stmt3, &stmt4, &stmt5, &stmt6, &stmt7, &stmt8, &stmt9,
                                          &stmt10, &stmt11, &stmt12};
    std::set<ProgramElement*> assigns = {&assign1, &assign2, &assign3, &assign7, &assign8, &assign9, &assign10};
    std::set<ProgramElement*> prints = {&print11};
    std::set<ProgramElement*> whiles = {&while4};
    std::set<ProgramElement*> reads = {&read5};
    std::set<ProgramElement*> calls = {&call12};
    std::set<ProgramElement*> ifs = {&if6};

    std::set<std::pair<ProgramElement*, ProgramElement*>> ifVar = {{&if6, &varZ}};
    std::set<std::pair<ProgramElement*, ProgramElement*>> whileVar = {{&while4, &varX}};
};

struct PATTERN_SOURCE_INFO {
    ProgramElement varA = ProgramElement::createVariable("a");
    ProgramElement varB = ProgramElement::createVariable("b");
    ProgramElement varC = ProgramElement::createVariable("c");
    ProgramElement varX = ProgramElement::createVariable("x");
    ProgramElement varY = ProgramElement::createVariable("y");
    ProgramElement varZ = ProgramElement::createVariable("z");

    ProgramElement stmt1 = ProgramElement::createStatement(ElementType::STATEMENT, 1);
    ProgramElement stmt2 = ProgramElement::createStatement(ElementType::STATEMENT, 2);
    ProgramElement stmt3 = ProgramElement::createStatement(ElementType::STATEMENT, 3);
    ProgramElement stmt4 = ProgramElement::createStatement(ElementType::STATEMENT, 4);
    ProgramElement stmt5 = ProgramElement::createStatement(ElementType::STATEMENT, 5);
    ProgramElement stmt6 = ProgramElement::createStatement(ElementType::STATEMENT, 6);
    ProgramElement stmt7 = ProgramElement::createStatement(ElementType::STATEMENT, 7);
    ProgramElement stmt8 = ProgramElement::createStatement(ElementType::STATEMENT, 8);

    ProgramElement assign1 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 1);
    ProgramElement assign2 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 2);
    ProgramElement assign4 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 4);
    ProgramElement assign5 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 5);
    ProgramElement assign7 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 7);
    ProgramElement assign8 = ProgramElement::createStatement(ElementType::ASSIGNMENT, 8);

    ProgramElement if3 = ProgramElement::createStatement(ElementType::IF, 3);
    ProgramElement while6 = ProgramElement::createStatement(ElementType::WHILE, 6);

    std::set<ProgramElement*> vars = {&varA, &varB, &varC, &varX, &varY, &varZ};
    std::set<ProgramElement*> stmts = {&stmt1, &stmt2, &stmt3, &stmt4, &stmt5, &stmt6, &stmt7, &stmt8};
    std::set<ProgramElement*> assigns = {&assign1, &assign2, &assign4, &assign5, &assign7, &assign8};
    std::set<ProgramElement*> ifs = {&if3};
    std::set<ProgramElement*> whiles = {&while6};

    std::set<std::pair<ProgramElement*, ProgramElement*>> assignVarPartial = {{&assign1, &varX}, {&assign8, &varA}};
    std::set<std::pair<ProgramElement*, ProgramElement*>> assignVarFull = {{&assign8, &varA}};
    std::set<ProgramElement*> assignGivenExprPartial = {&assign2, &assign4, &assign5, &assign7};
    std::set<ProgramElement*> assignGivenExprFull = {&assign1};
    std::set<ProgramElement*> assignGivenExprVarPartial = {&assign1};
    std::set<ProgramElement*> assignGivenExprVarFull = {&assign4, &assign5};
};

struct QPS_ENTITY_INFO {
    Entity procSyn1 = Entity(EntityType::PROCEDURE, "p1");
    Entity procSyn2 = Entity(EntityType::PROCEDURE, "p2");

    Entity varSyn = Entity(EntityType::VARIABLE, "v");
    Entity varSynVarAttr = Entity(EntityType::VARIABLE, "v", EntityAttributeType::VARNAME);
    Entity constSyn = Entity(EntityType::CONSTANT, "c");

    Entity stmtSyn = Entity(EntityType::STATEMENT, "s");
    Entity stmtSynStmtAttr = Entity(EntityType::STATEMENT, "s", EntityAttributeType::STMT);
    Entity printSyn = Entity(EntityType::PRINT, "p");
    Entity printSynStmtAttr = Entity(EntityType::PRINT, "p", EntityAttributeType::STMT);
    Entity printSynVarAttr = Entity(EntityType::PRINT, "p", EntityAttributeType::VARNAME);
    Entity readSyn = Entity(EntityType::READ, "r");
    Entity readSynStmtAttr = Entity(EntityType::READ, "r", EntityAttributeType::STMT);
    Entity readSynVarAttr = Entity(EntityType::READ, "r", EntityAttributeType::VARNAME);
    Entity assignSyn = Entity(EntityType::ASSIGNMENT, "a");
    Entity callSyn = Entity(EntityType::CALL, "call");
    Entity whileSyn = Entity(EntityType::WHILE, "w");
    Entity ifSyn = Entity(EntityType::IF, "ifs");


    Entity fixedInt1 = Entity(EntityType::FIXED_INTEGER, "1");
    Entity fixedInt2 = Entity(EntityType::FIXED_INTEGER, "2");
    Entity fixedInt4 = Entity(EntityType::FIXED_INTEGER, "4");
    Entity fixedInt5 = Entity(EntityType::FIXED_INTEGER, "5");
    Entity fixedInt6 = Entity(EntityType::FIXED_INTEGER, "6");
    Entity fixedInt11 = Entity(EntityType::FIXED_INTEGER, "11");
    Entity fixedInt12 = Entity(EntityType::FIXED_INTEGER, "12");

    Entity fixedStringF = Entity(EntityType::FIXED_STRING, "f");
    Entity fixedStringG = Entity(EntityType::FIXED_STRING, "g");
    Entity fixedStringW = Entity(EntityType::FIXED_STRING, "w");
    Entity fixedStringX = Entity(EntityType::FIXED_STRING, "x");
    Entity fixedStringY = Entity(EntityType::FIXED_STRING, "y");
    Entity fixedStringZ = Entity(EntityType::FIXED_STRING, "z");

    Entity fixedStringABC = Entity(EntityType::FIXED_STRING, "a + b + c");
    Entity fixedStringBC = Entity(EntityType::FIXED_STRING, "b + c");
    Entity fixedStringWWX = Entity(EntityType::FIXED_STRING_WITHIN_WILDCARD, "x");
    Entity fixedStringWWBC = Entity(EntityType::FIXED_STRING_WITHIN_WILDCARD, "b + c");


    Entity wildcard = Entity(EntityType::WILDCARD, "_");
};

#endif //SPA_QPSUTIL_H
