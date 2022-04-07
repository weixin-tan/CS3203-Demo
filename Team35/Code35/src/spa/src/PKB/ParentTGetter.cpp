#include "ParentTGetter.h"

ParentTGetter::ParentTGetter(DB* db) : db(db) {}

bool ParentTGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isParentT");
    auto childrenT = db->parentTTable.find(leftSide.stmtNo);
    return (childrenT != db->parentTTable.end() && childrenT->second.find(rightSide.stmtNo) != childrenT->second.end());
}

std::set<ProgramElement*> ParentTGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on PARENT_T");
    auto parentT = db->parentTTableR.find(rightSide.stmtNo);
    if (parentT == db->parentTTableR.end()) return {};
    for (const int& parentTStmtNo : parentT->second)
        RelationshipGetter::insertStmtElement(result, &db->elementStmtTable.at(parentTStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement*> ParentTGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getRightSide on PARENT_T");
    auto childrenT = db->parentTTable.find(leftSide.stmtNo);
    if (childrenT == db->parentTTable.end()) return {};
    for (const int& childTStmtNo : childrenT->second)
        RelationshipGetter::insertStmtElement(result, &db->elementStmtTable.at(childTStmtNo), typeToGet);
    return result;
}
