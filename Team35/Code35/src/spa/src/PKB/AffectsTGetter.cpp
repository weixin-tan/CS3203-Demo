#include "AffectsTGetter.h"

AffectsTGetter::AffectsTGetter(DB* db) : db(db), de(db) {}

bool AffectsTGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if (!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isAffectsT");
    if (db->elementStmtTable.at(leftSide.stmtNo).elementType != ElementType::ASSIGNMENT
            || db->elementStmtTable.at(rightSide.stmtNo).elementType != ElementType::ASSIGNMENT) return false;
    de.extractAffectsT(leftSide.stmtNo);
    auto affects = db->affectsTTable.at(leftSide.stmtNo);
    return affects.count(rightSide.stmtNo) != 0;
}

std::set<ProgramElement*> AffectsTGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on AffectsT");
    if (db->elementStmtTable.at(rightSide.stmtNo).elementType != ElementType::ASSIGNMENT
            || ((typeToGet != ElementType::ASSIGNMENT) && (typeToGet != ElementType::STATEMENT)))
        return {};
    de.extractAffectsTR(rightSide.stmtNo);
    for (const int& affectsStmtNo : db->affectsTTableR.at(rightSide.stmtNo))
        RelationshipGetter::insertStmtElement(result, &db->elementStmtTable.at(affectsStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement*> AffectsTGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for isAffectsT");
    if (db->elementStmtTable.at(leftSide.stmtNo).elementType != ElementType::ASSIGNMENT
            || ((typeToGet != ElementType::ASSIGNMENT) && (typeToGet != ElementType::STATEMENT)))
        return {};
    const std::string var = *db->modifiesSTable.at(leftSide.stmtNo).begin();
    de.extractAffectsT(leftSide.stmtNo);
    for (const int& affectsStmtNo : db->affectsTTable.at(leftSide.stmtNo))
        RelationshipGetter::insertStmtElement(result, &db->elementStmtTable.at(affectsStmtNo), typeToGet);
    return result;
}
