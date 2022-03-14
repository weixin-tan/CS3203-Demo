#include "ParentTGetter.h"

ParentTGetter::ParentTGetter(DB* db) : db(db) {}

bool ParentTGetter::isParentT(const ProgramElement& leftSide, const ProgramElement& rightSide) const {
    if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isParentT");
    auto childrenT = db->parentTTable.find(leftSide.stmtNo);
    return (childrenT != db->parentTTable.end() && childrenT->second.find(rightSide.stmtNo) != childrenT->second.end());
}

std::set<ProgramElement> ParentTGetter::getLeftParentT(const ProgramElement& rightSide,
                                                     const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on ParentT");
    auto parentT = db->parentTTableR.find(rightSide.stmtNo);
    if (parentT == db->parentTTableR.end()) return {};
    for (const int& parentTStmtNo : parentT->second)
        TemplateGetter::insertStmtElement(result, db->elementStmtTable.at(parentTStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement> ParentTGetter::getRightParentT(const ProgramElement& leftSide,
                                                      const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getRightSide on ParentT");
    auto childrenT = db->parentTTable.find(leftSide.stmtNo);
    if (childrenT == db->parentTTable.end()) return {};
    for (const int& childTStmtNo : childrenT->second)
        TemplateGetter::insertStmtElement(result, db->elementStmtTable.at(childTStmtNo), typeToGet);
    return result;
}
