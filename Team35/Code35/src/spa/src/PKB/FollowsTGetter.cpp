#include "FollowsTGetter.h"

FollowsTGetter::FollowsTGetter(DB* db) : db(db) {}

bool FollowsTGetter::isFollowsT(const ProgramElement& leftSide, const ProgramElement& rightSide) const {
    if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isFollowsT");
    auto followsT = db->followsTTable.find(leftSide.stmtNo);
    return (followsT != db->followsTTable.end() && followsT->second.find(rightSide.stmtNo) != followsT->second.end());
}

std::set<ProgramElement> FollowsTGetter::getLeftFollowsT(const ProgramElement& rightSide,
                                                       const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on FollowsT");
    auto precedingT = db->followsTTableR.find(rightSide.stmtNo);
    if (precedingT == db->followsTTableR.end()) return {};
    for (const int& precedingTStmtNo : precedingT->second)
        insertStmtElement(result, db->elementStmtTable.at(precedingTStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement> FollowsTGetter::getRightFollowsT(const ProgramElement& leftSide,
                                                        const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if(!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getRightSide on FollowsT");
    auto followsT = db->followsTTable.find(leftSide.stmtNo);
    if (followsT == db->followsTTable.end()) return {};
    for (const int& followsTStmtNo : followsT->second)
        TemplateGetter::insertStmtElement(result, db->elementStmtTable.at(followsTStmtNo), typeToGet);
    return result;
}
