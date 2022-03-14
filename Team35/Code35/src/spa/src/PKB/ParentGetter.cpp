#include "ParentGetter.h"

ParentGetter::ParentGetter(DB* db) : db(db) {}

bool ParentGetter::isParent(const ProgramElement& leftSide, const ProgramElement& rightSide) const {
    if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isParent");
    auto children = db->parentTable.find(leftSide.stmtNo);
    return (children != db->parentTable.end() && children->second.find(rightSide.stmtNo) != children->second.end());
}

std::set<ProgramElement> ParentGetter::getLeftParent(const ProgramElement& rightSide,
                                                       const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on Parent");
    auto parent = db->parentTableR.find(rightSide.stmtNo);
    if (parent == db->parentTableR.end()) return {};
    for (const int& parentStmtNo : parent->second)
        TemplateGetter::insertStmtElement(result, db->elementStmtTable.at(parentStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement> ParentGetter::getRightParent(const ProgramElement& leftSide,
                                                        const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getRightSide on Parent");
    auto children = db->parentTable.find(leftSide.stmtNo);
    if (children == db->parentTable.end()) return {};
    for (const int& childStmtNo : children->second)
        TemplateGetter::insertStmtElement(result, db->elementStmtTable.at(childStmtNo), typeToGet);
    return result;
}
