//
// Created by viv on 4/2/2022.
//

#include "SuchThatHandler.h"

SuchThatHandler::SuchThatHandler(PkbGetter* pkb) {
  SuchThatHandler::pkb = pkb;
}

std::vector<Result> SuchThatHandler::processClause(Clause c) {
    return nullptr;
}

bool SuchThatHandler::isRelationship(RelationshipType r, Entity e1, Entity e2) {
    return false;
}

Result SuchThatHandler::getEntity(Type typeToGet) {
    return nullptr;
}

Result SuchThatHandler::getRelationshipStatements(RelationshipType r) {
    return nullptr;
}

Result SuchThatHandler::getLeftSide(RelationshipType r, Entity rightSide, Type typeToGet) {
    return nullptr;
}

Result SuchThatHandler::getRightSide(RelationshipType r, Entity rightSide, Type typeToGet) {
    return nullptr;
}

