#include "NextGetter.h"

NextGetter::NextGetter(DB* db) : db(db) {}

bool NextGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isNext");
    auto next = db->nextTable.find(leftSide.stmtNo);
    return (next != db->nextTable.end() && next->second.find(rightSide.stmtNo) != next->second.end());
}

std::set<ProgramElement*> NextGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on Next");
    auto previous = db->nextTableR.find(rightSide.stmtNo);
    if (previous == db->nextTableR.end()) return {};
    for (const int& previousStmtNo : previous->second)
        RelationshipGetter::insertStmtElement(result, &db->elementStmtTable.at(previousStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement*> NextGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getRightSide on Next");
    auto next = db->nextTable.find(leftSide.stmtNo);
    if (next == db->nextTable.end()) return {};
    for (const int& nextStmtNo : next->second)
        RelationshipGetter::insertStmtElement(result, &db->elementStmtTable.at(nextStmtNo), typeToGet);
    return result;
}
