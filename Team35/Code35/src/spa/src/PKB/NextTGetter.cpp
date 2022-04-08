#include "NextTGetter.h"

NextTGetter::NextTGetter(DB* db) : db(db), de(db) {}

bool NextTGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if (!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isNextT");
    de.extractNextT(leftSide.stmtNo);
    auto nextT = db->nextTTable.find(leftSide.stmtNo);
    return (nextT != db->nextTTable.end() && nextT->second.find(rightSide.stmtNo) != nextT->second.end());
}

std::set<ProgramElement*> NextTGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on NEXT_T");
    de.extractNextTR(rightSide.stmtNo);
    auto previousT = db->nextTTableR.at(rightSide.stmtNo);
    for (const int& previousTStmtNo : previousT)
        RelationshipGetter::insertStmtElement(result, &db->elementStmtTable.at(previousTStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement*> NextTGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if(!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on NEXT_T");
    de.extractNextT(leftSide.stmtNo);
    auto nextT = db->nextTTable.at(leftSide.stmtNo);
    for (const int& nextTStmtNo : nextT)
        RelationshipGetter::insertStmtElement(result, &db->elementStmtTable.at(nextTStmtNo), typeToGet);
    return result;
}
