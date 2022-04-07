#include "PkbGetterStubForTestSuchThatHandler.h"

/*
PkbGetterStubForTestSuchThatHandler::PkbGetterStubForTestSuchThatHandler(DB *db) : PkbGetter(db) {

}

std::set<std::pair<ProgramElement, ProgramElement>>
PkbGetterStubForTestSuchThatHandler::getRelationshipPairs(const PkbRelationshipType &r, const ElementType &leftTypeToGet,
                                                          const ElementType &rightTypeToGet) {
    std::set<std::pair<ProgramElement, ProgramElement>> result;
    if (r == PkbRelationshipType::MODIFIES && leftTypeToGet == ElementType::PROCEDURE && rightTypeToGet == ElementType::VARIABLE) {
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("x")));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("y")));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createVariable("z")));
        return result;
    }
    if (r == PkbRelationshipType::MODIFIES && leftTypeToGet == ElementType::READ && rightTypeToGet == ElementType::VARIABLE) {
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::READ, 5), ProgramElement::createVariable("z")));
        return result;
    }
    if (r == PkbRelationshipType::MODIFIES && leftTypeToGet ==ElementType::PROCEDURE && rightTypeToGet == ElementType::STATEMENT) {
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 1)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 2)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 3)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 4)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 5)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 6)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 7)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 8)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 9)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 10)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 11)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createStatement(ElementType::STATEMENT, 12)));
        return result;
    }
    if (r == PkbRelationshipType::MODIFIES && leftTypeToGet == ElementType::STATEMENT && rightTypeToGet == ElementType::VARIABLE) {
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createVariable("x")));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 2), ProgramElement::createVariable("y")));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 3), ProgramElement::createVariable("x")));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 5), ProgramElement::createVariable("z")));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 7), ProgramElement::createVariable("x")));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 8), ProgramElement::createVariable("z")));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 9), ProgramElement::createVariable("y")));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 10), ProgramElement::createVariable("z")));
        return result;
    }
    if (r == PkbRelationshipType::CALLS && leftTypeToGet == ElementType::PROCEDURE && rightTypeToGet == ElementType::PROCEDURE) {
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createProcedure("f"), ProgramElement::createProcedure("g")));
        return result;
    }
    if (r == PkbRelationshipType::FOLLOWS && leftTypeToGet == ElementType::PRINT && rightTypeToGet == ElementType::CALL) {
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::PRINT, 11), ProgramElement::createStatement(ElementType::CALL, 12)));
        return result;
    }
    if (r == PkbRelationshipType::PARENT && leftTypeToGet == ElementType::WHILE && rightTypeToGet == ElementType::STATEMENT) {
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::WHILE, 4), ProgramElement::createStatement(ElementType::STATEMENT, 5)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::WHILE, 4), ProgramElement::createStatement(ElementType::STATEMENT, 6)));
        return result;
    }
    if (r == PkbRelationshipType::NEXT && leftTypeToGet == ElementType::STATEMENT && rightTypeToGet == ElementType::CALL) {
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 11), ProgramElement::createStatement(ElementType::CALL, 12)));
        return result;
    }
    if (r == PkbRelationshipType::FOLLOWS && leftTypeToGet == ElementType::STATEMENT && rightTypeToGet == ElementType::STATEMENT) {
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 1), ProgramElement::createStatement(ElementType::STATEMENT, 2)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 2), ProgramElement::createStatement(ElementType::STATEMENT, 3)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 3), ProgramElement::createStatement(ElementType::STATEMENT, 4)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 4), ProgramElement::createStatement(ElementType::STATEMENT, 11)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 11), ProgramElement::createStatement(ElementType::STATEMENT, 12)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 5), ProgramElement::createStatement(ElementType::STATEMENT, 6)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 7), ProgramElement::createStatement(ElementType::STATEMENT, 8)));
        result.insert(std::pair<ProgramElement, ProgramElement>(ProgramElement::createStatement(ElementType::STATEMENT, 9), ProgramElement::createStatement(ElementType::STATEMENT, 10)));
        return result;
    }
    return result;
}

bool PkbGetterStubForTestSuchThatHandler::isRelationship(const PkbRelationshipType &r, const ProgramElement &leftSide,
                                                         const ProgramElement &rightSide) {
    if (r == PkbRelationshipType::MODIFIES && leftSide == ProgramElement::createProcedure("f") && rightSide == ProgramElement::createVariable("y")) {
        return true;
    }
    if (r == PkbRelationshipType::MODIFIES && leftSide == ProgramElement::createStatement(ElementType::STATEMENT, 5) && rightSide == ProgramElement::createVariable("z")) {
        return true;
    }
    if (r == PkbRelationshipType::CALLS && leftSide == ProgramElement::createProcedure("f") && rightSide == ProgramElement::createProcedure("g")) {
        return true;
    }
    if (r == PkbRelationshipType::NEXT && leftSide == ProgramElement::createStatement(ElementType::STATEMENT, 11) && rightSide == ProgramElement::createStatement(ElementType::STATEMENT, 12)) {
        return true;
    }
    return false;
}

std::set<ProgramElement> PkbGetterStubForTestSuchThatHandler::getRightSide(const PkbRelationshipType &r, const ProgramElement &leftSide,
                                                                           const ElementType &typeToGet) {
    std::set<ProgramElement> results;
    if (r == PkbRelationshipType::USES && leftSide == ProgramElement::createProcedure("f") && typeToGet==ElementType::VARIABLE) {
        results.insert(ProgramElement::createVariable("x"));
        results.insert(ProgramElement::createVariable("z"));
        return results;
    }
    if (r == PkbRelationshipType::USES && leftSide == ProgramElement::createStatement(ElementType::STATEMENT, 2) && typeToGet ==ElementType::VARIABLE) {
        results.insert(ProgramElement::createVariable("x"));
        return results;
    }
    if (r == PkbRelationshipType::USES && leftSide == ProgramElement::createStatement(ElementType::STATEMENT, 6) && typeToGet == ElementType::VARIABLE) {
        results.insert(ProgramElement::createVariable("z"));
        return results;
    }
    if (r == PkbRelationshipType::CALLS && leftSide == ProgramElement::createProcedure("f") && typeToGet == ElementType::PROCEDURE) {
        results.insert(ProgramElement::createProcedure("g"));
        return results;
    }
    if (r == PkbRelationshipType::PARENT_T && leftSide == ProgramElement::createStatement(ElementType::STATEMENT, 4) && typeToGet == ElementType::ASSIGNMENT) {
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 7));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 8));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 9));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 10));
        return results;
    }
    if (r == PkbRelationshipType::NEXT_T && leftSide == ProgramElement::createStatement(ElementType::STATEMENT, 1) && typeToGet == ElementType::STATEMENT) {
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 2));
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 3));
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 4));
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 5));
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 6));
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 7));
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 8));
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 9));
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 10));
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 11));
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 12));
        return results;
    }
    return results;
}

std::set<ProgramElement> PkbGetterStubForTestSuchThatHandler::getLeftSide(const PkbRelationshipType &r, const ProgramElement &rightSide,
                                                                          const ElementType &typeToGet) {
    std::set<ProgramElement> results;
    if (r == PkbRelationshipType::USES && rightSide == ProgramElement::createVariable("x") && typeToGet == ElementType::PROCEDURE) {
        results.insert(ProgramElement::createProcedure("f"));
        return results;
    }
    if (r == PkbRelationshipType::USES && rightSide == ProgramElement::createVariable("z") && typeToGet == ElementType::IF) {
        results.insert(ProgramElement::createStatement(ElementType::IF, 6));
        return results;
    }
    if (r == PkbRelationshipType::CALLS && rightSide == ProgramElement::createProcedure("g") && typeToGet == ElementType::PROCEDURE) {
        results.insert(ProgramElement::createProcedure("f"));
        return results;
    }
    if (r == PkbRelationshipType::FOLLOWS_T && rightSide == ProgramElement::createStatement(ElementType::STATEMENT, 12) && typeToGet == ElementType::ASSIGNMENT) {
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 1));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 2));
        results.insert(ProgramElement::createStatement(ElementType::ASSIGNMENT, 3));
        return results;
    }
    if (r == PkbRelationshipType::NEXT && rightSide == ProgramElement::createStatement(ElementType::STATEMENT, 2) && typeToGet == ElementType::STATEMENT) {
        results.insert(ProgramElement::createStatement(ElementType::STATEMENT, 1));
        return results;
    }
    return results;
}

 */
