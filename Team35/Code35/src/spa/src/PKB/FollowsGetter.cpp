#include "FollowsGetter.h"

FollowsGetter::FollowsGetter(DB* db) : db(db) {}

bool FollowsGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isFollows");
    auto follows = db->followsTable.find(leftSide.stmtNo);
    return (follows != db->followsTable.end() && follows->second.find(rightSide.stmtNo) != follows->second.end());
}

std::set<ProgramElement> FollowsGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on Follows");
    auto preceding = db->followsTableR.find(rightSide.stmtNo);
    if (preceding == db->followsTableR.end()) return {};
    for (const int& precedingStmtNo : preceding->second)
        insertStmtElement(result, db->elementStmtTable.at(precedingStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement> FollowsGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getRightSide on Follows");
    auto follows = db->followsTable.find(leftSide.stmtNo);
    if (follows == db->followsTable.end()) return {};
    for (const int& followsStmtNo : follows->second)
        insertStmtElement(result, db->elementStmtTable.at(followsStmtNo), typeToGet);
    return result;
}
