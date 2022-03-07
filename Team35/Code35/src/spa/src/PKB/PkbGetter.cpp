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
            if (db->stmtTable.find(stmtNo) == db->stmtTable.end()) return false;
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

PkbGetter::PkbGetter(DB* db) : db(db) {}

bool PkbGetter::isRelationship(const PkbRelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide) const {
    if (!isExists(leftSide) || !isExists(rightSide)) return false;

    bool result = false;

    switch (r) {
        case PkbRelationshipType::kModifies : {
            if (rightSide.elementType != ElementType::kVariable)
                throw std::invalid_argument("Wrong right element type for isModifies");
            if (!(isStatementType(leftSide.elementType) || leftSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong left element type for isModifies");
            if (isStatementType(leftSide.elementType)) {
                auto modifiedVars = db->modifiesSTable.find(leftSide.stmtNo);
                if (modifiedVars == db->modifiesSTable.end()) break;
                result = modifiedVars->second.find(rightSide.varName) != modifiedVars->second.end();
            }
            if (leftSide.elementType == ElementType::kProcedure) {
                auto modifiedVars = db->modifiesPTable.find(leftSide.procName);
                if (modifiedVars == db->modifiesPTable.end()) break;
                result = modifiedVars->second.find(rightSide.varName) != modifiedVars->second.end();
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
            if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
                throw std::invalid_argument("Wrong element type for isParent");
            auto children = db->parentTable.find(leftSide.stmtNo);
            result = (children != db->parentTable.end() && children->second.find(rightSide.stmtNo) != children->second.end());
            break;
        }
        case PkbRelationshipType::kParentT: {
            if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
                throw std::invalid_argument("Wrong element type for isParentT");
            auto childrenT = db->parentTTable.find(leftSide.stmtNo);
            result = (childrenT != db->parentTTable.end() && childrenT->second.find(rightSide.stmtNo) != childrenT->second.end());
            break;
        }
        case PkbRelationshipType::kFollows: {
            if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
                throw std::invalid_argument("Wrong element type for isFollows");
            auto follows = db->followsTable.find(leftSide.stmtNo);
            result = (follows != db->followsTable.end() && follows->second.find(rightSide.stmtNo) != follows->second.end());
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
                throw std::invalid_argument("Wrong element type for isFollowsT");
            auto followsT = db->followsTTable.find(leftSide.stmtNo);
            result = (followsT != db->followsTTable.end() && followsT->second.find(rightSide.stmtNo) != followsT->second.end());
            break;
        }
        case PkbRelationshipType::kCalls: {
            if (!(leftSide.elementType == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for isCalls");
            auto calls = db->callsTable.find(leftSide.procName);
            result = (calls != db->callsTable.end() && calls->second.find(rightSide.procName) != calls->second.end());
            break;
        }
        case PkbRelationshipType::kCallsT: {
            if (!(leftSide.elementType == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for isCallsT");
            auto callsT = db->callsTTable.find(leftSide.procName);
            result = (callsT != db->callsTTable.end() && callsT->second.find(rightSide.procName) != callsT->second.end());
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

// TODO: optimise for reverse relationship
std::set<ProgramElement> PkbGetter::getLeftSide(const PkbRelationshipType& r, const ProgramElement& rightSide,
                                                const ElementType& typeToGet) const {
    if (!isExists(rightSide)) return {};

    std::set<ProgramElement> result;

    switch (r) {
        case PkbRelationshipType::kModifies: {
            if (!(isStatementType(typeToGet) || typeToGet == ElementType::kProcedure))
                throw std::invalid_argument("Wrong typeToGet for getLeftSide for Modifies");
            if (rightSide.elementType != ElementType::kVariable)
                throw std::invalid_argument("Wrong rightSide type for getLeftSide for Modifies");

            if (isStatementType(typeToGet)) {
                for (const auto&[stmtNo, modifiedVars] : db->modifiesSTable)
                    if (modifiedVars.find(rightSide.varName) != modifiedVars.end() &&
                            isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(stmtNo)))
                        result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
            }
            if (typeToGet == ElementType::kProcedure) {
                for (const auto&[proc, modifiedVars] : db->modifiesPTable)
                    if (modifiedVars.find(rightSide.varName) != modifiedVars.end())
                        result.insert(ProgramElement::createProcedure(proc));
            }
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
            if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getLeftSide on Follows");
            for (const auto&[stmtNo, followsStmtNos] : db->followsTable) {
                if (followsStmtNos.find(rightSide.stmtNo) == followsStmtNos.end()) continue;
                ElementType stmtType = db->stmtTypeTable.at(stmtNo);
                if (isStatementTypeToGet(typeToGet, stmtType))
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
            }
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getLeftSide on FollowsT");
            for (const auto&[stmtNo, followsTStmtNos] : db->followsTTable) {
                if (followsTStmtNos.find(rightSide.stmtNo) == followsTStmtNos.end()) continue;
                ElementType stmtType = db->stmtTypeTable.at(stmtNo);
                if (isStatementTypeToGet(typeToGet, stmtType))
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
            }
            break;
        }
        case PkbRelationshipType::kParent: {
            if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getLeftSide on Parent");
            for (const auto&[stmtNo, childrenStmtNos] : db->parentTable) {
                if (childrenStmtNos.find(rightSide.stmtNo) == childrenStmtNos.end()) continue;
                ElementType stmtType = db->stmtTypeTable.at(stmtNo);
                if (isStatementTypeToGet(typeToGet, stmtType))
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
            }
            break;
        }
        case PkbRelationshipType::kParentT: {
            if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getLeftSide on ParentT");
            for (const auto&[stmtNo, childrenTStmtNos] : db->parentTTable) {
                if (childrenTStmtNos.find(rightSide.stmtNo) == childrenTStmtNos.end()) continue;
                ElementType stmtType = db->stmtTypeTable.at(stmtNo);
                if (isStatementTypeToGet(typeToGet, stmtType))
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
            }
            break;
        }
        case PkbRelationshipType::kCalls: {
            if (!(typeToGet == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for getLeftSide on Calls");
            for (const auto&[callsProc, calledProcs] : db->callsTable)
                if (calledProcs.find(rightSide.procName) != calledProcs.end())
                    result.insert(ProgramElement::createProcedure(callsProc));
            break;
        }
        case PkbRelationshipType::kCallsT: {
            if (!(typeToGet == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for getLeftSide on CallsT");
            for (const auto&[callsTProc, calledTProcs] : db->callsTTable)
                if (calledTProcs.find(rightSide.procName) != calledTProcs.end())
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
            if (!(isStatementType(leftSide.elementType) || leftSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong leftSide element type for getRightSide for Modifies");
            if (typeToGet != ElementType::kVariable)
                throw std::invalid_argument("Wrong typeToGet for getRightSide for Modifies");

            if (isStatementType(leftSide.elementType)) {
                auto modifiedVars = db->modifiesSTable.find(leftSide.stmtNo);
                if (modifiedVars == db->modifiesSTable.end()) break;
                for (const auto& var : modifiedVars->second)
                    result.insert(ProgramElement::createVariable(var));
            }
            if (leftSide.elementType == ElementType::kProcedure) {
                auto modifiedVars = db->modifiesPTable.find(leftSide.procName);
                if (modifiedVars == db->modifiesPTable.end()) break;
                for (const auto& var : modifiedVars->second)
                    result.insert(ProgramElement::createVariable(var));
            }
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
            if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getRightSide on Follows");
            auto follows = db->followsTable.find(leftSide.stmtNo);
            if (follows == db->followsTable.end()) break;
            for (const int& followsStmtNo : follows->second) {
                ElementType followsStmtType = db->stmtTypeTable.at(followsStmtNo);
                if (isStatementTypeToGet(typeToGet, followsStmtType))
                    result.insert(ProgramElement::createStatement(typeToGet, followsStmtNo));
            }
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            if(!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getRightSide on FollowsT");
            auto followsT = db->followsTTable.find(leftSide.stmtNo);
            if (followsT == db->followsTTable.end()) break;
            for (const int& followsTStmtNo : followsT->second) {
                ElementType followsTStmtType = db->stmtTypeTable.at(followsTStmtNo);
                if (isStatementTypeToGet(typeToGet, followsTStmtType))
                    result.insert(ProgramElement::createStatement(typeToGet, followsTStmtNo));
            }
            break;
        }
        case PkbRelationshipType::kParent: {
            if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getRightSide on Parent");
            auto children = db->parentTable.find(leftSide.stmtNo);
            if (children == db->parentTable.end()) break;
            for (const int& childStmtNo : children->second) {
                ElementType childStmtType = db->stmtTypeTable.at(childStmtNo);
                if (isStatementTypeToGet(typeToGet, childStmtType))
                    result.insert(ProgramElement::createStatement(typeToGet, childStmtNo));
            }
            break;
        }
        case PkbRelationshipType::kParentT: {
            if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getRightSide on ParentT");
            auto childrenT = db->parentTTable.find(leftSide.stmtNo);
            if (childrenT == db->parentTTable.end()) break;
            for (const int& childTStmtNo : childrenT->second) {
                ElementType childTStmtType = db->stmtTypeTable.at(childTStmtNo);
                if (isStatementTypeToGet(typeToGet, childTStmtType))
                    result.insert(ProgramElement::createStatement(typeToGet, childTStmtNo));
            }
            break;
        }
        case PkbRelationshipType::kCalls: {
            if (!(leftSide.elementType == ElementType::kProcedure && typeToGet == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for getRightSide on Calls");
            auto calls = db->callsTable.find(leftSide.procName);
            if (calls == db->callsTable.end()) break;
            for (const std::string& calledProc : calls->second)
                result.insert(ProgramElement::createProcedure(calledProc));
            break;
        }
        case PkbRelationshipType::kCallsT: {
            if (!(leftSide.elementType == ElementType::kProcedure && typeToGet == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for getRightSide on CallsT");
            auto callsT = db->callsTTable.find(leftSide.procName);
            if (callsT == db->callsTTable.end()) break;
            for (const std::string& calledProc : callsT->second)
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
