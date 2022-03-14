#include "UsesGetter.h"

UsesGetter::UsesGetter(DB* db) : db(db) {}

std::set<int> UsesGetter::getUsesStmtNosGivenConstant(const std::string& c) const {
    if (db->constantToStmtTable.count(c) == 0)
        return {};
    return db->constantToStmtTable.at(c);
}

bool UsesGetter::isUses(const ProgramElement& leftSide, const ProgramElement& rightSide) const {
    bool result = false;
    if (rightSide.elementType != ElementType::kVariable)
        throw std::invalid_argument("Wrong right element type for isUses");
    if (!(isStatementType(leftSide.elementType) || leftSide.elementType == ElementType::kProcedure))
        throw std::invalid_argument("Wrong left element type for isUses");
    if (isStatementType(leftSide.elementType)) {
        auto usedVars = db->usesSTable.find(leftSide.stmtNo);
        if (usedVars == db->usesSTable.end()) return false;
        result = usedVars->second.find(rightSide.varName) != usedVars->second.end();
    }
    if (leftSide.elementType == ElementType::kProcedure) {
        auto usedVars = db->usesPTable.find(leftSide.procName);
        if (usedVars == db->usesPTable.end()) return false;
        result = usedVars->second.find(rightSide.varName) != usedVars->second.end();
    }
    return result;

}

std::set<ProgramElement> UsesGetter::getLeftUses(const ProgramElement& rightSide,
                                                 const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if (!(isStatementType(typeToGet) || typeToGet == ElementType::kProcedure))
        throw std::invalid_argument("Wrong typeToGet for getLeftSide for Uses");
    if (rightSide.elementType != ElementType::kVariable && rightSide.elementType != ElementType::kConstant) // temporary
        throw std::invalid_argument("Wrong rightSide type for getLeftSide for Uses");

    // TODO: remove once expression is supported
    if (rightSide.elementType == ElementType::kConstant) {
        for (const int& stmtNo : getUsesStmtNosGivenConstant(rightSide.value))
            TemplateGetter::insertStmtElement(result, db->elementStmtTable.at(stmtNo), typeToGet);
        return result;
    }

    if (isStatementType(typeToGet)) {
        auto usesStmtNos = db->usesSTableR.find(rightSide.varName);
        if (usesStmtNos == db->usesSTableR.end()) return {};
        for (const auto& stmtNo : usesStmtNos->second)
            TemplateGetter::insertStmtElement(result, db->elementStmtTable.at(stmtNo), typeToGet);
    }
    if (typeToGet == ElementType::kProcedure) {
        auto usesProcs = db->usesPTableR.find(rightSide.varName);
        if (usesProcs == db->usesPTableR.end()) return {};
        for (const auto& proc : usesProcs->second)
            result.insert(ProgramElement::createProcedure(proc));
    }
    return result;
}

std::set<ProgramElement> UsesGetter::getRightUses(const ProgramElement& leftSide,
                                                  const ElementType& typeToGet) const {
    std::set<ProgramElement> result;
    if (!(isStatementType(leftSide.elementType) || leftSide.elementType == ElementType::kProcedure))
        throw std::invalid_argument("Wrong leftSide element type for getRightSide for Uses");
    if (typeToGet != ElementType::kVariable)
        throw std::invalid_argument("Wrong typeToGet for getRightSide for Uses");

    if (isStatementType(leftSide.elementType)) {
        auto usedVars = db->usesSTable.find(leftSide.stmtNo);
        if (usedVars == db->usesSTable.end()) return {};
        for (const auto& var : usedVars->second)
            result.insert(ProgramElement::createVariable(var));
    }
    if (leftSide.elementType == ElementType::kProcedure) {
        auto usedVars = db->usesPTable.find(leftSide.procName);
        if (usedVars == db->usesPTable.end()) return {};
        for (const auto& var : usedVars->second)
            result.insert(ProgramElement::createVariable(var));
    }
    return result;
}