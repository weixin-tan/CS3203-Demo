#include <set>
#include <cassert>

#include "PkbGetter.h"
#include "ProgramElement.h"
#include "PkbRelationshipType.h"
#include "DB.h"

// TODO: rename
bool inline isStatementTypeToGet(const ElementType& typeToGet, const ElementType& targetType) {
    return (typeToGet == ElementType::kStatement) || (typeToGet == targetType);
}

bool PkbGetter::isExists(const ProgramElement& elementToCheck) const {
    switch (elementToCheck.elementType) {
        case ElementType::kStatement:
        case ElementType::kRead:
        case ElementType::kPrint:
        case ElementType::kCall:
        case ElementType::kWhile:
        case ElementType::kIf:
        case ElementType::kAssignment: {
            int stmtNo = elementToCheck.stmtNo;
            if (db->stmtTable.count(stmtNo) == 0) return false;
            return isStatementTypeToGet(elementToCheck.elementType, getStmtType(stmtNo));
        }
        case ElementType::kProcedure:
            return db->procedures.count(elementToCheck.procName);
        case ElementType::kVariable:
            return db->variables.count(elementToCheck.varName);
        case ElementType::kConstant:
            return db->constants.count(elementToCheck.value);
        default:
            throw std::logic_error("Unknown element type to check, or didn't return");
    }
}

// TODO: move this to db
// TODO: maybe use custom map
std::set<int> PkbGetter::getChildStmtNos(int parentStmtNo) const {
    if (db->parentToChildTable.count(parentStmtNo) == 0)
        return {};
    return db->parentToChildTable.at(parentStmtNo);
}

std::set<std::string> PkbGetter::getModifiedVars(int parentStmtNo) const {
    if (db->modifyStmtToVarTable.count(parentStmtNo) == 0)
        return {};
    return db->modifyStmtToVarTable.at(parentStmtNo);
}

std::set<int> PkbGetter::getModifiesStmtNosGivenVariable(const std::string& var) const {
    if (db->varToModifyStmtTable.count(var) == 0)
        return {};
    return db->varToModifyStmtTable.at(var);
}

std::set<std::string> PkbGetter::getModifiesProcs(const std::string& var) const {
    if (db->varToModifyProcTable.count(var) == 0)
        return {};
    return db->varToModifyProcTable.at(var);
}

std::set<std::string> PkbGetter::getVariableGivenModifyProc(const std::string& proc) const {
    if (db->modifyProcToVarTable.count(proc) == 0)
        return {};
    return db->modifyProcToVarTable.at(proc);
}

std::set<std::string> PkbGetter::getUsedVars(int parentStmtNo) const {
    if (db->usesStmtToVarTable.count(parentStmtNo) == 0)
        return {};
    return db->usesStmtToVarTable.at(parentStmtNo);
}

std::set<int> PkbGetter::getUsesStmtNosGivenVariable(const std::string& var) const {
    if (db->varToUsesStmtTable.count(var) == 0)
        return {};
    return db->varToUsesStmtTable.at(var);
}

std::set<std::string> PkbGetter::getUsesProcs(const std::string& var) const {
    if (db->varToUsesProcTable.count(var) == 0)
        return {};
    return db->varToUsesProcTable.at(var);
}

std::set<std::string> PkbGetter::getVariableGivenUsesProc(const std::string& proc) const {
    if (db->usesProcToVarTable.count(proc) == 0)
        return {};
    return db->usesProcToVarTable.at(proc);
}

int PkbGetter::getParentStmtNo(int childStmtNo) const {
    if (db->childToParentTable.count(childStmtNo) == 0)
        return ParsedStatement::default_null_stmt_no;
    return db->childToParentTable.at(childStmtNo);
}

int PkbGetter::getFollowingStmtNo(int stmtNo) const {
    if (db->stmtFollowing.count(stmtNo) == 0)
        return ParsedStatement::default_null_stmt_no;
    return db->stmtFollowing.at(stmtNo);
}

int PkbGetter::getPrecedingStmtNo(int stmtNo) const {
    if (db->stmtPreceding.count(stmtNo) == 0)
        return ParsedStatement::default_null_stmt_no;
    return db->stmtPreceding.at(stmtNo);
}

std::set<int> PkbGetter::getUsesStmtNosGivenConstant(const std::string& c) const {
    if (db->constantToStmtTable.count(c) == 0)
        return {};
    return db->constantToStmtTable.at(c);
}

ElementType PkbGetter::getStmtType(int stmtNo) const {
    if (db->stmtTypeTable.count(stmtNo) == 0)
        assert(false);  // should've been caught
    return db->stmtTypeTable.at(stmtNo);
}

std::set<std::string> PkbGetter::getUsedConstants(int stmtNo) const {
    if (db->usesStmtToConstantTable.count(stmtNo) == 0)
        return {};
    return db->usesStmtToConstantTable.at(stmtNo);
}

std::set<std::string> PkbGetter::getCalls(const std::string& proc) const {
    if (db->callsTable.count(proc) == 0)
        return {};
    return db->callsTable.at(proc);
}

std::set<std::string> PkbGetter::getCallsT(const std::string& proc) const {
    if (db->callsTTable.count(proc) == 0)
        return {};
    return db->callsTTable.at(proc);
}

PkbGetter::PkbGetter(DB* db) : db(db) {}

bool PkbGetter::isRelationship(const PkbRelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide) const {
    if (!isExists(leftSide) || !isExists(rightSide)) return false;

    bool result = false;

    switch (r) {
        case PkbRelationshipType::kModifies : {
            assert(rightSide.elementType == ElementType::kVariable);
            if (isStatementType(leftSide.elementType)) {
                std::set<ProgramElement> modifyStatements = getLeftSide(r, rightSide, leftSide.elementType);
                result = modifyStatements.find(leftSide) != modifyStatements.end();
            } else if (leftSide.elementType == ElementType::kProcedure) {
                std::set<ProgramElement> modifyProcedure = getLeftSide(r, rightSide, leftSide.elementType);
                result = modifyProcedure.find(leftSide) != modifyProcedure.end();
            }
            break;
        }
        case PkbRelationshipType::kUses: {
            assert(rightSide.elementType == ElementType::kVariable);
            if (isStatementType(leftSide.elementType)) {
                std::set<ProgramElement> usesStatements = getLeftSide(r, rightSide, leftSide.elementType);
                result = usesStatements.find(leftSide) != usesStatements.end();
            } else if (leftSide.elementType == ElementType::kProcedure) {
                std::set<ProgramElement> usesProcedure = getLeftSide(r, rightSide, leftSide.elementType);
                result = usesProcedure.find(leftSide) != usesProcedure.end();
            }
            break;
        }
        case PkbRelationshipType::kParent: {
            assert(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType));
            result = getParentStmtNo(rightSide.stmtNo) == leftSide.stmtNo;
            break;
        }
        case PkbRelationshipType::kFollows: {
            assert(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType));
            result = getFollowingStmtNo(leftSide.stmtNo) == rightSide.stmtNo;
            break;
        }
        case PkbRelationshipType::kParentT: {
            int targetStmtNo = leftSide.stmtNo;
            int currentStmtNo = rightSide.stmtNo;
            while (currentStmtNo != ParsedStatement::default_null_stmt_no) {
                currentStmtNo = getParentStmtNo(currentStmtNo);
                if (currentStmtNo == targetStmtNo) {
                    result = true;
                    break;
                }
            }
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            int targetStmtNo = leftSide.stmtNo;
            int currentStmtNo = rightSide.stmtNo;
            while (currentStmtNo != ParsedStatement::default_null_stmt_no) {
                currentStmtNo = getPrecedingStmtNo(currentStmtNo);
                if (currentStmtNo == targetStmtNo) {
                    result = true;
                    break;
                }
            }
            break;
        }
        case PkbRelationshipType::kCalls: {
            if (!(leftSide.elementType == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for isCalls");
            result = (db->callsTable.count(leftSide.procName) != 0)
                    && (db->callsTable.at(leftSide.procName).count(rightSide.procName) != 0);
            break;
        }
        case PkbRelationshipType::kCallsT: {
            if (!(leftSide.elementType == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for isCallsT");
            result = (db->callsTTable.count(leftSide.procName) != 0)
                    && (db->callsTTable.at(leftSide.procName).count(rightSide.procName) != 0);
            break;
        }
        default: {
            assert(false);
        }
    }

    return result;
}

std::set<ProgramElement> PkbGetter::getEntity(const ElementType& typeToGet) const {
    std::set<ProgramElement> result;

    switch (typeToGet) {
        case ElementType::kStatement:
        case ElementType::kRead:
        case ElementType::kPrint:
        case ElementType::kCall:
        case ElementType::kWhile:
        case ElementType::kIf:
        case ElementType::kAssignment: {
            for (const auto&[stmtNo, eType] : db->stmtTypeTable)
                if (isStatementTypeToGet(typeToGet, eType))
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
            break;
        }
        case ElementType::kVariable: {
            for (const std::string& var: db->variables)
                result.insert(ProgramElement::createVariable(var));
            break;
        }
        case ElementType::kProcedure: {
            for (const std::string& proc: db->procedures)
                result.insert(ProgramElement::createProcedure(proc));
            break;
        }
        case ElementType::kConstant: {
            for (const std::string& c : db->constants)
                result.insert(ProgramElement::createConstant(c));
            break;
        }
        default: {
            assert(false);
        }
    }

    return result;
}

// TODO: should we still store reverse relationships?
std::set<ProgramElement> PkbGetter::getLeftSide(const PkbRelationshipType& r, const ProgramElement& rightSide,
                                                const ElementType& typeToGet) const {
    if (!isExists(rightSide)) return {};

    std::set<ProgramElement> result;

    switch (r) {
        case PkbRelationshipType::kModifies: {
            assert(isStatementType(typeToGet) || typeToGet == ElementType::kProcedure);
            assert(rightSide.elementType == ElementType::kVariable);

            if (isStatementType(typeToGet)) {
                std::set<int> stmtNos;
                for (const int& stmtNo : getModifiesStmtNosGivenVariable(rightSide.varName)) {
                    int curStmtNo = stmtNo;
                    // do not revisit statements visited
                    while (curStmtNo != ParsedStatement::default_null_stmt_no && stmtNos.find(curStmtNo) == stmtNos.end()) {
                        if (isStatementTypeToGet(typeToGet, getStmtType(curStmtNo)))
                            stmtNos.insert(curStmtNo);
                        curStmtNo = getParentStmtNo(curStmtNo);
                    }
                }
                for (const int& stmtNo : stmtNos)
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
                break;
            } else if (typeToGet == ElementType::kProcedure) {
                for (const std::string& proc : getModifiesProcs(rightSide.varName))
                    result.insert(ProgramElement::createProcedure(proc));
            } else assert(false);
            break;
        }
        case PkbRelationshipType::kUses: {
            assert(isStatementType(typeToGet) || typeToGet == ElementType::kProcedure);
            assert(rightSide.elementType == ElementType::kVariable || rightSide.elementType == ElementType::kConstant);

            // TODO: remove once expression is supported
            if (rightSide.elementType == ElementType::kConstant) {
                for (const int& stmtNo : getUsesStmtNosGivenConstant(rightSide.value))
                    if (isStatementTypeToGet(typeToGet, getStmtType(stmtNo)))
                        result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
                break;
            }

            if (isStatementType(typeToGet)) {
                std::set<int> stmtNos;
                for (const int& stmtNo : getUsesStmtNosGivenVariable(rightSide.varName)) {
                    int curStmtNo = stmtNo;
                    // do not revisit statements visited
                    while (curStmtNo != ParsedStatement::default_null_stmt_no && stmtNos.find(curStmtNo) == stmtNos.end()) {
                        if (isStatementTypeToGet(typeToGet, getStmtType(curStmtNo)))
                            stmtNos.insert(curStmtNo);
                        curStmtNo = getParentStmtNo(curStmtNo);
                    }
                }
                for (const int& stmtNo : stmtNos)
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
            } else if (typeToGet == ElementType::kProcedure) {
                for (const std::string& proc : getUsesProcs(rightSide.varName))
                    result.insert(ProgramElement::createProcedure(proc));
            } else assert(false);
            break;
        }
        case PkbRelationshipType::kFollows: {
            assert(isStatementType(rightSide.elementType) && isStatementType(typeToGet));

            int targetStmtNo = getPrecedingStmtNo(rightSide.stmtNo);
            if (targetStmtNo != ParsedStatement::default_null_stmt_no && isStatementTypeToGet(typeToGet, getStmtType(targetStmtNo)))
                result.insert(ProgramElement::createStatement(typeToGet, targetStmtNo));
            break;
        }
        case PkbRelationshipType::kParent: {
            assert(isStatementType(rightSide.elementType) && isStatementType(typeToGet));

            int targetStmtNo = getParentStmtNo(rightSide.stmtNo);
            if (targetStmtNo != ParsedStatement::default_null_stmt_no && isStatementTypeToGet(typeToGet, getStmtType(targetStmtNo)))
                result.insert(ProgramElement::createStatement(typeToGet, targetStmtNo));
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            assert(isStatementType(rightSide.elementType) && isStatementType(typeToGet));

            int currentStmtNo = getPrecedingStmtNo(rightSide.stmtNo);
            while (currentStmtNo != ParsedStatement::default_null_stmt_no) {
                if (isStatementTypeToGet(typeToGet, getStmtType(currentStmtNo)))
                    result.insert(ProgramElement::createStatement(typeToGet, currentStmtNo));
                currentStmtNo = getPrecedingStmtNo(currentStmtNo);
            }

            break;
        }
        case PkbRelationshipType::kParentT: {
            assert(isStatementType(rightSide.elementType) && isStatementType(typeToGet));

            int currentStmtNo = getParentStmtNo(rightSide.stmtNo);
            while (currentStmtNo != ParsedStatement::default_null_stmt_no) {
                if (isStatementTypeToGet(typeToGet, getStmtType(currentStmtNo)))
                    result.insert(ProgramElement::createStatement(typeToGet, currentStmtNo));
                currentStmtNo = getParentStmtNo(currentStmtNo);
            }

            break;
        }
        case PkbRelationshipType::kCalls: {
            if (!(typeToGet == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for getLeftSide on Calls");
            for (const auto&[callsProc, calledProcs] : db->callsTable)
                if (calledProcs.count(rightSide.procName) != 0)
                    result.insert(ProgramElement::createProcedure(callsProc));
            break;
        }
        case PkbRelationshipType::kCallsT: {
            if (!(typeToGet == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for getLeftSide on CallsT");
            for (const auto&[callsTProc, calledTProcs] : db->callsTTable)
                if (calledTProcs.count(rightSide.procName) != 0)
                    result.insert(ProgramElement::createProcedure(callsTProc));
            break;
        }
        default:
            throw std::invalid_argument("Unknown relationship type for getLeftSide or didn't break");
    }

    return result;
}

std::set<ProgramElement> PkbGetter::getRightSide(const PkbRelationshipType& r, const ProgramElement& leftSide,
                                                 const ElementType& typeToGet) const {
    if (!isExists(leftSide)) return {};
    std::set<ProgramElement> result;
    switch (r) {
        case PkbRelationshipType::kModifies: {
            assert(isStatementType(leftSide.elementType) || leftSide.elementType == ElementType::kProcedure);
            assert(typeToGet == ElementType::kVariable);

            if (isStatementType(leftSide.elementType)) {
                for (const std::string& var : getModifiedVars(leftSide.stmtNo))
                    result.insert(ProgramElement::createVariable(var));

                for (const int& childStmtNo: getChildStmtNos(leftSide.stmtNo))
                    result.merge(getRightSide(r, ProgramElement::createStatement(ElementType::kStatement, childStmtNo), typeToGet));
            } else if (leftSide.elementType == ElementType::kProcedure) {
                for (const std::string& var : getVariableGivenModifyProc(leftSide.procName))
                    result.insert(ProgramElement::createVariable(var));
            } else assert(false);
            break;
        }
        case PkbRelationshipType::kUses: {
            assert(isStatementType(leftSide.elementType) || leftSide.elementType == ElementType::kProcedure);
            assert(typeToGet == ElementType::kVariable);

            if (isStatementType(leftSide.elementType)) {
                for (const std::string& var : getUsedVars(leftSide.stmtNo))
                    result.insert(ProgramElement::createVariable(var));

                for (const int& childStmtNo: getChildStmtNos(leftSide.stmtNo))
                    result.merge(getRightSide(r, ProgramElement::createStatement(ElementType::kStatement, childStmtNo), typeToGet));
            } else if (leftSide.elementType == ElementType::kProcedure) {
                for (const std::string& var : getVariableGivenUsesProc(leftSide.procName))
                    result.insert(ProgramElement::createVariable(var));
            } else assert(false);
            break;
        }
        case PkbRelationshipType::kFollows: {
            assert(isStatementType(leftSide.elementType) && isStatementType(typeToGet));

            int targetStmtNo = getFollowingStmtNo(leftSide.stmtNo);
            if (targetStmtNo != ParsedStatement::default_null_stmt_no && isStatementTypeToGet(typeToGet, getStmtType(targetStmtNo)))
                result.insert(ProgramElement::createStatement(typeToGet, targetStmtNo));
            break;
        }
        case PkbRelationshipType::kParent: {
            assert(isStatementType(leftSide.elementType) && isStatementType(typeToGet));

            for (const int& childStmtNo: getChildStmtNos(leftSide.stmtNo))
                if (isStatementTypeToGet(typeToGet, getStmtType(childStmtNo)))
                    result.insert(ProgramElement::createStatement(typeToGet, childStmtNo));
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            assert(isStatementType(leftSide.elementType) && isStatementType(typeToGet));

            int currentStmtNo = getFollowingStmtNo(leftSide.stmtNo);
            while (currentStmtNo != ParsedStatement::default_null_stmt_no) {
                if (isStatementTypeToGet(typeToGet, getStmtType(currentStmtNo)))
                    result.insert(ProgramElement::createStatement(typeToGet, currentStmtNo));
                currentStmtNo = getFollowingStmtNo(currentStmtNo);
            }

            break;
        }
        case PkbRelationshipType::kParentT: {
            assert(isStatementType(leftSide.elementType) && isStatementType(typeToGet));

            for (const int& childStmtNo: getChildStmtNos(leftSide.stmtNo)) {
                ElementType childStmtType = getStmtType(childStmtNo);
                if (isStatementTypeToGet(typeToGet, childStmtType))
                    result.insert(ProgramElement::createStatement(typeToGet, childStmtNo));
                result.merge(getRightSide(r, ProgramElement::createStatement(childStmtType, childStmtNo), typeToGet));
            }
            break;
        }
        case PkbRelationshipType::kCalls: {
            if (!(leftSide.elementType == ElementType::kProcedure && typeToGet == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for getRightSide on Calls");
            for (const std::string& calledProc : getCalls(leftSide.procName))
                result.insert(ProgramElement::createProcedure(calledProc));
            break;
        }
        case PkbRelationshipType::kCallsT: {
            if (!(leftSide.elementType == ElementType::kProcedure && typeToGet == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for getRightSide on CallsT");
            for (const std::string& calledProc : getCallsT(leftSide.procName))
                result.insert(ProgramElement::createProcedure(calledProc));
            break;
        }
        default:
            throw std::invalid_argument("Unknown relationship type for getRightSide or didn't break");
    }

    return result;
}

// TODO: temporary, expression currently is just a constant or variable used
std::set<ProgramElement> PkbGetter::getAssignmentGivenExpression(const ProgramElement& expression) const {
    return getLeftSide(PkbRelationshipType::kUses, expression, ElementType::kAssignment);
}

std::set<ProgramElement> PkbGetter::getAssignmentGivenVariableAndExpression(const ProgramElement& variable, const ProgramElement& expression) const {
    std::set<ProgramElement> result;
    std::set<ProgramElement> assignments = getAssignmentGivenExpression(expression);
    for (const auto& assignment : assignments)
        if (getRightSide(PkbRelationshipType::kModifies, assignment, ElementType::kVariable).count(variable) != 0)
            result.insert(assignment);
    return result;
}
