#include "ModifiesGetter.h"

ModifiesGetter::ModifiesGetter(DB* db) : db(db) {}

bool ModifiesGetter::isRelationship(const ProgramElement& leftSide, const ProgramElement& rightSide) {
    bool result = false;
    if (rightSide.elementType != ElementType::kVariable)
        throw std::invalid_argument("Wrong right element type for isModifies");
    if (!(isStatementType(leftSide.elementType) || leftSide.elementType == ElementType::kProcedure))
        throw std::invalid_argument("Wrong left element type for isModifies");
    if (isStatementType(leftSide.elementType)) {
        auto modifiedVars = db->modifiesSTable.find(leftSide.stmtNo);
        if (modifiedVars == db->modifiesSTable.end()) return false;
        result = modifiedVars->second.find(rightSide.varName) != modifiedVars->second.end();
    }
    if (leftSide.elementType == ElementType::kProcedure) {
        auto modifiedVars = db->modifiesPTable.find(leftSide.procName);
        if (modifiedVars == db->modifiesPTable.end()) return false;
        result = modifiedVars->second.find(rightSide.varName) != modifiedVars->second.end();
    }
    return result;

}

std::set<ProgramElement> ModifiesGetter::getLeftSide(const ProgramElement& rightSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if (!(isStatementType(typeToGet) || typeToGet == ElementType::kProcedure))
        throw std::invalid_argument("Wrong typeToGet for getLeftSide for Modifies");
    if (rightSide.elementType != ElementType::kVariable)
        throw std::invalid_argument("Wrong rightSide type for getLeftSide for Modifies");

    if (isStatementType(typeToGet)) {
        auto modifiesStmtNos = db->modifiesSTableR.find(rightSide.varName);
        if (modifiesStmtNos == db->modifiesSTableR.end()) return {};
        for (const auto& stmtNo : modifiesStmtNos->second)
            insertStmtElement(result, db->elementStmtTable.at(stmtNo), typeToGet);
    }

    if (typeToGet == ElementType::kProcedure) {
        auto modifiesProcs = db->modifiesPTableR.find(rightSide.varName);
        if (modifiesProcs == db->modifiesPTableR.end()) return {};
        for (const auto& proc : modifiesProcs->second)
            result.insert(ProgramElement::createProcedure(proc));
    }
    return result;
}

std::set<ProgramElement> ModifiesGetter::getRightSide(const ProgramElement& leftSide, const ElementType& typeToGet) {
    std::set<ProgramElement> result;
    if (!(isStatementType(leftSide.elementType) || leftSide.elementType == ElementType::kProcedure))
        throw std::invalid_argument("Wrong leftSide element type for getRightSide for Modifies");
    if (typeToGet != ElementType::kVariable)
        throw std::invalid_argument("Wrong typeToGet for getRightSide for Modifies");

    if (isStatementType(leftSide.elementType)) {
        auto modifiedVars = db->modifiesSTable.find(leftSide.stmtNo);
        if (modifiedVars == db->modifiesSTable.end()) return {};
        for (const auto& var : modifiedVars->second)
            result.insert(ProgramElement::createVariable(var));
    }
    if (leftSide.elementType == ElementType::kProcedure) {
        auto modifiedVars = db->modifiesPTable.find(leftSide.procName);
        if (modifiedVars == db->modifiesPTable.end()) return {};
        for (const auto& var : modifiedVars->second)
            result.insert(ProgramElement::createVariable(var));
    }
    return result;
}