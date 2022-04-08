#include "AffectsGetter.h"

AffectsGetter::AffectsGetter(DB* db) : db(db), affectsExtractor(db) {}

bool AffectsGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    if (!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
        throw std::invalid_argument("Wrong element type for isAffects");
    if (db->elementStmtTable.at(leftSide.stmtNo).elementType != ElementType::ASSIGNMENT
            || db->elementStmtTable.at(rightSide.stmtNo).elementType != ElementType::ASSIGNMENT) return false;
    if (db->usesSTable.at(rightSide.stmtNo).count(*db->modifiesSTable.at(leftSide.stmtNo).begin()) == 0)
        return false;
    affectsExtractor.extractAffects(leftSide.stmtNo);
    auto affects = db->affectsTable.at(leftSide.stmtNo);
    return affects.count(rightSide.stmtNo) != 0;
}

std::set<ProgramElement*> AffectsGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for getLeftSide on Affects");
    if (db->elementStmtTable.at(rightSide.stmtNo).elementType != ElementType::ASSIGNMENT
            || ((typeToGet != ElementType::ASSIGNMENT) && (typeToGet != ElementType::STATEMENT)))
        return {};
    affectsExtractor.extractAffectsR(rightSide.stmtNo);
    for (const int& affectsStmtNo : db->affectsTableR.at(rightSide.stmtNo))
        RelationshipGetter::insertStmtElement(result, &db->elementStmtTable.at(affectsStmtNo), typeToGet);
    return result;
}

std::set<ProgramElement*> AffectsGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement*> result;
    if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
        throw std::invalid_argument("Wrong element type for isAffects");
    if (db->elementStmtTable.at(leftSide.stmtNo).elementType != ElementType::ASSIGNMENT
            || ((typeToGet != ElementType::ASSIGNMENT) && (typeToGet != ElementType::STATEMENT)))
        return {};
    const std::string var = *db->modifiesSTable.at(leftSide.stmtNo).begin();
    affectsExtractor.extractAffects(leftSide.stmtNo);
    for (const int& affectsStmtNo : db->affectsTable.at(leftSide.stmtNo))
        RelationshipGetter::insertStmtElement(result, &db->elementStmtTable.at(affectsStmtNo), typeToGet);
    return result;
}
