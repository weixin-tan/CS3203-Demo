#include <set>

#include "PkbGetter.h"
#include "ProgramElement.h"
#include "PkbRelationshipType.h"
#include "TemplateGetter.h"
#include "DB.h"

// TODO: rename

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
            return TemplateGetter::isStatementTypeToGet(elementToCheck.elementType, existingType);
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

PkbGetter::PkbGetter(DB* db) :
    db(db),
    modifiesGetter(db),
    usesGetter(db),
    followsGetter(db),
    followsTGetter(db),
    parentGetter(db),
    parentTGetter(db),
    callsGetter(db),
    callsTGetter(db),
    nextGetter(db)
    {}

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
                TemplateGetter::insertStmtElement(result, elementStmt, typeToGet);
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

bool PkbGetter::isRelationship(const PkbRelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide) const {
    if (!isExists(leftSide) || !isExists(rightSide)) return false;

    bool result = false;

    switch (r) {
        case PkbRelationshipType::kModifies : {
            result = modifiesGetter.isModifies(leftSide, rightSide);
            break;
        }
        case PkbRelationshipType::kUses: {
            result = usesGetter.isUses(leftSide, rightSide);
            break;
        }
        case PkbRelationshipType::kParent: {
            result = parentGetter.isParent(leftSide, rightSide);
            break;
        }
        case PkbRelationshipType::kParentT: {
            result = parentTGetter.isParentT(leftSide, rightSide);
            break;
        }
        case PkbRelationshipType::kFollows: {
            result = followsGetter.isFollows(leftSide, rightSide);
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            result = followsTGetter.isFollowsT(leftSide, rightSide);
            break;
        }
        case PkbRelationshipType::kCalls: {
            result = callsGetter.isCalls(leftSide, rightSide);
            break;
        }
        case PkbRelationshipType::kCallsT: {
            result = callsTGetter.isCallsT(leftSide, rightSide);
            break;
        }
        case PkbRelationshipType::kNext : {
            result = nextGetter.isNext(leftSide, rightSide);
            break;
        }
        default:
            throw std::invalid_argument("Unknown relationshipType for isRelationship");
    }

    return result;
}

std::set<ProgramElement> PkbGetter::getLeftSide(const PkbRelationshipType& r, const ProgramElement& rightSide,
                                                const ElementType& typeToGet) const {
    if (!isExists(rightSide)) return {};

    std::set<ProgramElement> result;

    switch (r) {
        case PkbRelationshipType::kModifies: {
            result = modifiesGetter.getLeftModifies(rightSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kUses: {
            result = usesGetter.getLeftUses(rightSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kFollows: {
            result = followsGetter.getLeftFollows(rightSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            result = followsTGetter.getLeftFollowsT(rightSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kParent: {
            result = parentGetter.getLeftParent(rightSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kParentT: {
            result = parentTGetter.getLeftParentT(rightSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kCalls: {
            result = callsGetter.getLeftCalls(rightSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kCallsT: {
            result = callsTGetter.getLeftCallsT(rightSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kNext: {
            result = nextGetter.getLeftNext(rightSide, typeToGet);
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
            result = modifiesGetter.getRightModifies(leftSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kUses: {
            result = usesGetter.getRightUses(leftSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kFollows: {
            result = followsGetter.getRightFollows(leftSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kFollowsT: {
            result = followsTGetter.getRightFollowsT(leftSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kParent: {
            result = parentGetter.getRightParent(leftSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kParentT: {
            result = parentTGetter.getRightParentT(leftSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kCalls: {
            result = callsGetter.getRightCalls(leftSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kCallsT: {
            result = callsTGetter.getRightCallsT(leftSide, typeToGet);
            break;
        }
        case PkbRelationshipType::kNext: {
            result = nextGetter.getRightNext(leftSide, typeToGet);
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
