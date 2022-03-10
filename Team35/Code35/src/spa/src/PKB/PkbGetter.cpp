#include <set>

#include "PkbGetter.h"
#include "ProgramElement.h"
#include "PkbRelationshipType.h"
#include "DB.h"

// TODO: rename
bool inline isStatementTypeToGet(const ElementType& typeToGet, const ElementType& targetType) {
    return (typeToGet == ElementType::kStatement) || (typeToGet == targetType);
}

void PkbGetter::insertStmtElement(std::set<ProgramElement>& result, const ProgramElement& statement, const ElementType& typeToGet) {
    if (isStatementTypeToGet(typeToGet, statement.elementType))
        result.insert(statement);
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
            auto stmt = db->elementStmtTable.find(stmtNo);
            if (stmt == db->elementStmtTable.end()) return false;
            ElementType existingType = stmt->second.elementType;
            return isStatementTypeToGet(elementToCheck.elementType, existingType);
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

std::set<int> PkbGetter::getUsesStmtNosGivenConstant(const std::string& c) const {
    if (db->constantToStmtTable.count(c) == 0)
        return {};
    return db->constantToStmtTable.at(c);
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
            if (rightSide.elementType != ElementType::kVariable)
                throw std::invalid_argument("Wrong right element type for isUses");
            if (!(isStatementType(leftSide.elementType) || leftSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong left element type for isUses");
            if (isStatementType(leftSide.elementType)) {
                auto usedVars = db->usesSTable.find(leftSide.stmtNo);
                if (usedVars == db->usesSTable.end()) break;
                result = usedVars->second.find(rightSide.varName) != usedVars->second.end();
            }
            if (leftSide.elementType == ElementType::kProcedure) {
                auto usedVars = db->usesPTable.find(leftSide.procName);
                if (usedVars == db->usesPTable.end()) break;
                result = usedVars->second.find(rightSide.varName) != usedVars->second.end();
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
        case PkbRelationshipType::kNext : {
            if(!(isStatementType(leftSide.elementType) && isStatementType(rightSide.elementType)))
                throw std::invalid_argument("Wrong element type for isNext");
            auto next = db->nextTable.find(leftSide.stmtNo);
            result = (next != db->nextTable.end() && next->second.find(rightSide.stmtNo) != next->second.end());
            break;
        }
        default:
            throw std::invalid_argument("Unknown relationshipType for isRelationship");
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
            for (const auto&[stmtNo, elementStmt] : db->elementStmtTable)
                insertStmtElement(result, elementStmt, typeToGet);
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
        default:
            throw std::invalid_argument("Unknown typeToGet for getEntity");
    }

    return result;
}

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
                auto modifiesStmtNos = db->modifiesSTableR.find(rightSide.varName);
                if (modifiesStmtNos == db->modifiesSTableR.end()) break;
                for (const auto& stmtNo : modifiesStmtNos->second)
                    insertStmtElement(result, db->elementStmtTable.at(stmtNo), typeToGet);
            }
            if (typeToGet == ElementType::kProcedure) {
                auto modifiesProcs = db->modifiesPTableR.find(rightSide.varName);
                if (modifiesProcs == db->modifiesPTableR.end()) break;
                for (const auto& proc : modifiesProcs->second)
                    result.insert(ProgramElement::createProcedure(proc));
            }
            break;
        }
        case PkbRelationshipType::kUses: {
            if (!(isStatementType(typeToGet) || typeToGet == ElementType::kProcedure))
                throw std::invalid_argument("Wrong typeToGet for getLeftSide for Uses");
            if (rightSide.elementType != ElementType::kVariable && rightSide.elementType != ElementType::kConstant) // temporary
                throw std::invalid_argument("Wrong rightSide type for getLeftSide for Uses");

            // TODO: remove once expression is supported
            if (rightSide.elementType == ElementType::kConstant) {
                for (const int& stmtNo : getUsesStmtNosGivenConstant(rightSide.value))
                    insertStmtElement(result, db->elementStmtTable.at(stmtNo), typeToGet);
                break;
            }

            if (isStatementType(typeToGet)) {
                auto usesStmtNos = db->usesSTableR.find(rightSide.varName);
                if (usesStmtNos == db->usesSTableR.end()) break;
                for (const auto& stmtNo : usesStmtNos->second)
                    insertStmtElement(result, db->elementStmtTable.at(stmtNo), typeToGet);
            }
            if (typeToGet == ElementType::kProcedure) {
                auto usesProcs = db->usesPTableR.find(rightSide.varName);
                if (usesProcs == db->usesPTableR.end()) break;
                for (const auto& proc : usesProcs->second)
                    result.insert(ProgramElement::createProcedure(proc));
            }
            break;
        }
        case PkbRelationshipType::kFollows: {
            if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getLeftSide on Follows");
            auto preceding = db->followsTableR.find(rightSide.stmtNo);
            if (preceding == db->followsTableR.end()) break;
            for (const int& precedingStmtNo : preceding->second)
                insertStmtElement(result, db->elementStmtTable.at(precedingStmtNo), typeToGet);
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getLeftSide on FollowsT");
            auto precedingT = db->followsTTableR.find(rightSide.stmtNo);
            if (precedingT == db->followsTTableR.end()) break;
            for (const int& precedingTStmtNo : precedingT->second)
                insertStmtElement(result, db->elementStmtTable.at(precedingTStmtNo), typeToGet);
            break;
        }
        case PkbRelationshipType::kParent: {
            if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getLeftSide on Parent");
            auto parent = db->parentTableR.find(rightSide.stmtNo);
            if (parent == db->parentTableR.end()) break;
            for (const int& parentStmtNo : parent->second)
                insertStmtElement(result, db->elementStmtTable.at(parentStmtNo), typeToGet);
            break;
        }
        case PkbRelationshipType::kParentT: {
            if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getLeftSide on ParentT");
            auto parentT = db->parentTTableR.find(rightSide.stmtNo);
            if (parentT == db->parentTTableR.end()) break;
            for (const int& parentTStmtNo : parentT->second)
                insertStmtElement(result, db->elementStmtTable.at(parentTStmtNo), typeToGet);
            break;
        }
        case PkbRelationshipType::kCalls: {
            if (!(typeToGet == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for getLeftSide on Calls");
            auto caller = db->callsTableR.find(rightSide.procName);
            if (caller == db->callsTableR.end()) break;
            for (const std::string& callerProc : caller->second)
                result.insert(ProgramElement::createProcedure(callerProc));
            break;
        }
        case PkbRelationshipType::kCallsT: {
            if (!(typeToGet == ElementType::kProcedure && rightSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong element type for getLeftSide on CallsT");
            auto callerT = db->callsTTableR.find(rightSide.procName);
            if (callerT == db->callsTTableR.end()) break;
            for (const std::string& callerTProc : callerT->second)
                result.insert(ProgramElement::createProcedure(callerTProc));
            break;
        }
        case PkbRelationshipType::kNext: {
            if(!(isStatementType(rightSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getLeftSide on Next");
            auto previous = db->nextTableR.find(rightSide.stmtNo);
            if (previous == db->nextTableR.end()) break;
            for (const int& previousStmtNo : previous->second)
                insertStmtElement(result, db->elementStmtTable.at(previousStmtNo), typeToGet);
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
            if (!(isStatementType(leftSide.elementType) || leftSide.elementType == ElementType::kProcedure))
                throw std::invalid_argument("Wrong leftSide element type for getRightSide for Uses");
            if (typeToGet != ElementType::kVariable)
                throw std::invalid_argument("Wrong typeToGet for getRightSide for Uses");

            if (isStatementType(leftSide.elementType)) {
                auto usedVars = db->usesSTable.find(leftSide.stmtNo);
                if (usedVars == db->usesSTable.end()) break;
                for (const auto& var : usedVars->second)
                    result.insert(ProgramElement::createVariable(var));
            }
            if (leftSide.elementType == ElementType::kProcedure) {
                auto usedVars = db->usesPTable.find(leftSide.procName);
                if (usedVars == db->usesPTable.end()) break;
                for (const auto& var : usedVars->second)
                    result.insert(ProgramElement::createVariable(var));
            }
            break;
        }
        case PkbRelationshipType::kFollows: {
            if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getRightSide on Follows");
            auto follows = db->followsTable.find(leftSide.stmtNo);
            if (follows == db->followsTable.end()) break;
            for (const int& followsStmtNo : follows->second)
                insertStmtElement(result, db->elementStmtTable.at(followsStmtNo), typeToGet);
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            if(!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getRightSide on FollowsT");
            auto followsT = db->followsTTable.find(leftSide.stmtNo);
            if (followsT == db->followsTTable.end()) break;
            for (const int& followsTStmtNo : followsT->second)
                insertStmtElement(result, db->elementStmtTable.at(followsTStmtNo), typeToGet);
            break;
        }
        case PkbRelationshipType::kParent: {
            if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getRightSide on Parent");
            auto children = db->parentTable.find(leftSide.stmtNo);
            if (children == db->parentTable.end()) break;
            for (const int& childStmtNo : children->second)
                insertStmtElement(result, db->elementStmtTable.at(childStmtNo), typeToGet);
            break;
        }
        case PkbRelationshipType::kParentT: {
            if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getRightSide on ParentT");
            auto childrenT = db->parentTTable.find(leftSide.stmtNo);
            if (childrenT == db->parentTTable.end()) break;
            for (const int& childTStmtNo : childrenT->second)
                insertStmtElement(result, db->elementStmtTable.at(childTStmtNo), typeToGet);
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
        case PkbRelationshipType::kNext: {
            if (!(isStatementType(leftSide.elementType) && isStatementType(typeToGet)))
                throw std::invalid_argument("Wrong element type for getRightSide on Next");
            auto next = db->nextTable.find(leftSide.stmtNo);
            if (next == db->nextTable.end()) break;
            for (const int& nextStmtNo : next->second)
                insertStmtElement(result, db->elementStmtTable.at(nextStmtNo), typeToGet);
            break;
        }
        default:
            throw std::invalid_argument("Unknown relationship type for getRightSide or didn't break");
    }

    return result;
}

std::set<std::pair<ProgramElement, ProgramElement>> PkbGetter::getRelationshipPairs(const PkbRelationshipType& r, const ElementType& leftTypeToGet, const ElementType& rightTypeToGet) const {
    std::set<std::pair<ProgramElement, ProgramElement>> result;
    std::set<ProgramElement> leftSides = getEntity(leftTypeToGet);
    for (const auto& leftSide : leftSides)
        for (const auto& rightSide : getRightSide(r, leftSide, rightTypeToGet))
            result.insert(std::make_pair(leftSide, rightSide));
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
