//
// Created by Juliet Teoh on 18/3/22.
//

#include "PKBGetterStubForWithHandler.h"

PKBGetterStubForWithHandler::PKBGetterStubForWithHandler(DB* db) : PkbGetter(db) {
}

std::set<ProgramElement> PKBGetterStubForWithHandler::getEntity(const ElementType& typeToGet) const {
    std::set<ProgramElement> toReturn;
    if (typeToGet == ElementType::PROCEDURE) {
        toReturn.insert(ProgramElement::createProcedure("f"));
        toReturn.insert(ProgramElement::createProcedure("g"));
    }

    if (typeToGet == ElementType::CALL) {
        toReturn.insert(ProgramElement::createStatement(ElementType::CALL, 12, "g"));
    }

    if (typeToGet == ElementType::VARIABLE) {
        toReturn.insert(ProgramElement::createVariable("x"));
        toReturn.insert(ProgramElement::createVariable("y"));
        toReturn.insert(ProgramElement::createVariable("z"));
    }

    if (typeToGet == ElementType::WHILE) {
        toReturn.insert(ProgramElement::createStatement(ElementType::WHILE, 4));
    }

    if (typeToGet == ElementType::READ) {
        toReturn.insert(ProgramElement::createStatement(ElementType::READ, 5, "x"));
    }

    if (typeToGet == ElementType::PRINT) {
        toReturn.insert(ProgramElement::createStatement(ElementType::PRINT, 11, "x"));
    }

    if (typeToGet == ElementType::CONSTANT) {
        toReturn.insert(ProgramElement::createConstant("5"));
        toReturn.insert(ProgramElement::createConstant("4"));
        toReturn.insert(ProgramElement::createConstant("10"));
        toReturn.insert(ProgramElement::createConstant("100"));
        toReturn.insert(ProgramElement::createConstant("0"));
    }

    if (typeToGet == ElementType::IF) {
        toReturn.insert(ProgramElement::createStatement(ElementType::IF, 6));
    }

    if (typeToGet == ElementType::STATEMENT) {
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 1));
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 2));
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 3));
        toReturn.insert(ProgramElement::createStatement(ElementType::WHILE, 4));
        toReturn.insert(ProgramElement::createStatement(ElementType::READ, 5, "x"));
        toReturn.insert(ProgramElement::createStatement(ElementType::IF, 6));
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 7));
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 8));
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 9));
        toReturn.insert(ProgramElement::createStatement(ElementType::PRINT, 11, "x"));
        toReturn.insert(ProgramElement::createStatement(ElementType::CALL, 12, "g"));
    }

    if (typeToGet == ElementType::ASSIGNMENT) {
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 1));
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 2));
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 3));
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 7));
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 8));
        toReturn.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 9));
    }

    return toReturn;
}

//procedure f {
//  x = 5;  // 1
//  y = x;  // 2
//  x = 5;  // 3
//  while (x > 4) {  // 4
//    read x;  // 5
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