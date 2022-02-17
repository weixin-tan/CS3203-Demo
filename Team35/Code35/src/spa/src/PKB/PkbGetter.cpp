#include <set>
#include <cassert>

#include "PkbGetter.h"
#include "ProgramElement.h"
#include "DB.h"

// TODO: rename
bool inline isStatementTypeToGet(const ElementType& typeToGet, const ElementType& targetType) {
    return (typeToGet == ElementType::kStatement) || (typeToGet == targetType);
}

bool PkbGetter::isExists(const ProgramElement& elementToCheck) const {
    if (isStatementType(elementToCheck.element_type)) {
        int stmtNo = elementToCheck.integer_value;
        if (db->stmtTable.count(stmtNo) == 0) return false;
        return isStatementTypeToGet(elementToCheck.element_type, db->stmtTypeTable.at(stmtNo));
    }
    if (ElementType::kProcedure == elementToCheck.element_type)
        return db->procedures.count(elementToCheck.string_value);
    if (ElementType::kVariable == elementToCheck.element_type)
        return db->variables.count(elementToCheck.string_value);
    if (ElementType::kConstant == elementToCheck.element_type)
        return db->constants.count(elementToCheck.string_value);
    assert(false);
}

PkbGetter::PkbGetter(DB* db) : db(db) {}

bool PkbGetter::isRelationship(const RelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide) const {
    if (!isExists(leftSide) || !isExists(rightSide)) return false;

    bool result = false;

    switch (r) {
        case RelationshipType::Modifies : {
            assert(rightSide.element_type == ElementType::kVariable);
            if (isStatementType(leftSide.element_type)) {
                std::set<ProgramElement> modifyStatements = getLeftSide(r, rightSide, leftSide.element_type);
                result = modifyStatements.find(leftSide) != modifyStatements.end();
            } else if (leftSide.element_type == ElementType::kProcedure) {
                std::set<ProgramElement> modifyProcedure = getLeftSide(r, rightSide, leftSide.element_type);
                result = modifyProcedure.find(leftSide) != modifyProcedure.end();
            }
            break;
        }
        case RelationshipType::Uses: {
            assert(rightSide.element_type == ElementType::kVariable);
            if (isStatementType(leftSide.element_type)) {
                std::set<ProgramElement> usesStatements = getLeftSide(r, rightSide, leftSide.element_type);
                result = usesStatements.find(leftSide) != usesStatements.end();
            } else if (leftSide.element_type == ElementType::kProcedure) {
                std::set<ProgramElement> usesProcedure = getLeftSide(r, rightSide, leftSide.element_type);
                result = usesProcedure.find(leftSide) != usesProcedure.end();
            }
            break;
        }
        case RelationshipType::Parent: {
            assert(isStatementType(leftSide.element_type) && isStatementType(rightSide.element_type));
            result = db->childToParentTable.at(rightSide.integer_value) == leftSide.integer_value;
            break;
        }
        case RelationshipType::Follows: {
            assert(isStatementType(leftSide.element_type) && isStatementType(rightSide.element_type));
            result = db->stmtFollowing.at(rightSide.integer_value) == leftSide.integer_value;
            break;
        }
        case RelationshipType::ParentT: {
            int targetStmtNo = leftSide.integer_value;
            int currentStmtNo = rightSide.integer_value;
            while (currentStmtNo != ParsedStatement::default_null_stmt_no) {
                currentStmtNo = db->childToParentTable.at(currentStmtNo);
                if (currentStmtNo == targetStmtNo) {
                    result = true;
                    break;
                }
            }
            break;
        }
        case RelationshipType::FollowsT: {
            int targetStmtNo = leftSide.integer_value;
            int currentStmtNo = rightSide.integer_value;
            while (currentStmtNo != ParsedStatement::default_null_stmt_no) {
                currentStmtNo = db->stmtFollowing.at(currentStmtNo);
                if (currentStmtNo == targetStmtNo) {
                    result = true;
                    break;
                }
            }
            break;
        }
        default: {
            assert(false);
        }
    }

    return result;
}

std::set<ProgramElement> PkbGetter::getRelationshipStatements(const RelationshipType& r) const {
    std::set<ProgramElement> result;

    switch (r) {
        case RelationshipType::Modifies: {
            for (const auto&[var, _] : db->varToModifyStmtTable)
                result.merge(getLeftSide(r, ProgramElement::createVariable(var), ElementType::kStatement));
            break;
        }
        case RelationshipType::Uses: {
            for (const auto&[var, _] : db->varToUsesStmtTable)
                result.merge(getLeftSide(r, ProgramElement::createVariable(var), ElementType::kStatement));
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

std::set<ProgramElement> PkbGetter::getLeftSide(const RelationshipType& r, const ProgramElement& rightSide,
                                                const ElementType& typeToGet) const {
    if (!isExists(rightSide)) return {};

    std::set<ProgramElement> result;

    switch (r) {
        case RelationshipType::Modifies: {
            assert(isStatementType(typeToGet) || typeToGet == ElementType::kProcedure);
            // TODO: temporary allow constant, for pattern
            assert(rightSide.element_type == ElementType::kVariable || rightSide.element_type == ElementType::kConstant);

            if (rightSide.element_type == ElementType::kConstant) {
                for (const int& stmtNo : db->constantToStmtTable.at(rightSide.string_value))
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
                break;
            }

            if (isStatementType(typeToGet)) {
                std::set<int> stmtNos;
                for (const int& stmtNo : db->varToModifyStmtTable.at(rightSide.string_value)) {
                    int curStmtNo = stmtNo;
                    // do not revisit statements visited
                    while (curStmtNo != ParsedStatement::default_null_stmt_no && stmtNos.find(curStmtNo) == stmtNos.end()) {
                        if (isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(curStmtNo)))
                            stmtNos.insert(curStmtNo);
                        curStmtNo = db->childToParentTable.at(curStmtNo);
                    }
                }
                for (const int& stmtNo : stmtNos)
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
                break;
            } else if (typeToGet == ElementType::kProcedure) {
                for (const std::string& proc : db->varToModifyProcTable.at(rightSide.string_value))
                    result.insert(ProgramElement::createProcedure(proc));
            } else assert(false);
            break;
        }
        case RelationshipType::Uses: {
            assert(isStatementType(typeToGet) || typeToGet == ElementType::kProcedure);
            assert(rightSide.element_type == ElementType::kVariable);

            if (isStatementType(typeToGet)) {
                std::set<int> stmtNos;
                for (const int& stmtNo : db->varToUsesStmtTable.at(rightSide.string_value)) {
                    int curStmtNo = stmtNo;
                    // do not revisit statements visited
                    while (curStmtNo != ParsedStatement::default_null_stmt_no && stmtNos.find(curStmtNo) == stmtNos.end()) {
                        if (isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(curStmtNo)))
                            stmtNos.insert(curStmtNo);
                        curStmtNo = db->childToParentTable.at(curStmtNo);
                    }
                }
                for (const int& stmtNo : stmtNos)
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
            } else if (typeToGet == ElementType::kProcedure) {
                for (const std::string& proc : db->varToUsesProcTable.at(rightSide.string_value))
                    result.insert(ProgramElement::createProcedure(proc));
            } else assert(false);
            break;
        }
        case RelationshipType::Follows: {
            assert(isStatementType(rightSide.element_type) && isStatementType(typeToGet));

            int targetStmtNo = db->stmtPreceding.at(rightSide.integer_value);
            if (targetStmtNo != ParsedStatement::default_null_stmt_no && isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(targetStmtNo)))
                result.insert(ProgramElement::createStatement(typeToGet, targetStmtNo));
            break;
        }
        case RelationshipType::Parent: {
            assert(isStatementType(rightSide.element_type) && isStatementType(typeToGet));

            int targetStmtNo = db->childToParentTable.at(rightSide.integer_value);
            if (targetStmtNo != ParsedStatement::default_null_stmt_no && isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(targetStmtNo)))
                result.insert(ProgramElement::createStatement(typeToGet, targetStmtNo));
            break;
        }
        case RelationshipType::FollowsT: {
            assert(isStatementType(rightSide.element_type) && isStatementType(typeToGet));

            int currentStmtNo = db->stmtPreceding.at(rightSide.integer_value);
            while (currentStmtNo != ParsedStatement::default_null_stmt_no) {
                if (isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(currentStmtNo)))
                    result.insert(ProgramElement::createStatement(typeToGet, currentStmtNo));
                currentStmtNo = db->stmtPreceding.at(currentStmtNo);
            }

            break;
        }
        case RelationshipType::ParentT: {
            assert(isStatementType(rightSide.element_type) && isStatementType(typeToGet));

            int currentStmtNo = db->childToParentTable.at(rightSide.integer_value);
            while (currentStmtNo != ParsedStatement::default_null_stmt_no) {
                if (isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(currentStmtNo)))
                    result.insert(ProgramElement::createStatement(typeToGet, currentStmtNo));
                currentStmtNo = db->childToParentTable.at(currentStmtNo);
            }

            break;
        }
        default: {
            assert(false);
        }
    }

    return result;
}

std::set<ProgramElement> PkbGetter::getRightSide(const RelationshipType& r, const ProgramElement& leftSide,
                                                 const ElementType& typeToGet) const {
    if (!isExists(leftSide)) return {};
    std::set<ProgramElement> result;
    switch (r) {
        case RelationshipType::Modifies: {
            assert(isStatementType(leftSide.element_type) || leftSide.element_type == ElementType::kProcedure);
            assert(typeToGet == ElementType::kVariable);

            if (isStatementType(leftSide.element_type)) {
                for (const std::string& var : db->modifyStmtToVarTable.at(leftSide.integer_value))
                    result.insert(ProgramElement::createVariable(var));

                for (const int& childStmtNo: db->parentToChildTable.at(leftSide.integer_value))
                    result.merge(getRightSide(r, ProgramElement::createStatement(ElementType::kStatement, childStmtNo), typeToGet));
            } else if (leftSide.element_type == ElementType::kProcedure) {
                for (const std::string& var : db->modifyProcToVarTable.at(leftSide.string_value))
                    result.insert(ProgramElement::createVariable(var));
            } else assert(false);
            break;
        }
        case RelationshipType::Uses: {
            assert(isStatementType(leftSide.element_type) || leftSide.element_type == ElementType::kProcedure);
            assert(typeToGet == ElementType::kVariable);

            if (isStatementType(leftSide.element_type)) {
                for (const std::string& var : db->usesStmtToVarTable.at(leftSide.integer_value))
                    result.insert(ProgramElement::createVariable(var));

                for (const int& childStmtNo: db->parentToChildTable.at(leftSide.integer_value))
                    result.merge(getRightSide(r, ProgramElement::createStatement(ElementType::kStatement, childStmtNo), typeToGet));
            } else if (leftSide.element_type == ElementType::kProcedure) {
                for (const std::string& var : db->usesProcToVarTable.at(leftSide.string_value))
                    result.insert(ProgramElement::createVariable(var));
            } else assert(false);
            break;
        }
        case RelationshipType::Follows: {
            assert(isStatementType(leftSide.element_type) && isStatementType(typeToGet));

            int targetStmtNo = db->stmtFollowing.at(leftSide.integer_value);
            if (targetStmtNo != ParsedStatement::default_null_stmt_no && isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(targetStmtNo)))
                result.insert(ProgramElement::createStatement(typeToGet, targetStmtNo));
            break;
        }
        case RelationshipType::Parent: {
            assert(isStatementType(leftSide.element_type) && isStatementType(typeToGet));

            for (const int& targetStmtNo : db->parentToChildTable.at(leftSide.integer_value))
                if (isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(targetStmtNo)))
                    result.insert(ProgramElement::createStatement(typeToGet, targetStmtNo));
            break;
        }
        case RelationshipType::FollowsT: {
            assert(isStatementType(leftSide.element_type) && isStatementType(typeToGet));

            int currentStmtNo = db->stmtFollowing.at(leftSide.integer_value);
            while (currentStmtNo != ParsedStatement::default_null_stmt_no) {
                if (isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(currentStmtNo)))
                    result.insert(ProgramElement::createStatement(typeToGet, currentStmtNo));
                currentStmtNo = db->stmtPreceding.at(currentStmtNo);
            }

            break;
        }
        case RelationshipType::ParentT: {
            assert(isStatementType(leftSide.element_type) && isStatementType(typeToGet));

            for (const int& childStmtNo : db->parentToChildTable.at(leftSide.integer_value)) {
                ElementType childStmtType = db->stmtTypeTable.at(childStmtNo);
                if (isStatementTypeToGet(typeToGet, childStmtType))
                    result.insert(ProgramElement::createStatement(typeToGet, childStmtNo));
                result.merge(getRightSide(r, ProgramElement::createStatement(childStmtType, childStmtNo), typeToGet));
            }
            break;
        }
        default: {
            assert(false);
        }
    }

    return result;
}

// TODO: temporary, expression currently is just a constant or variable used
std::set<ProgramElement> PkbGetter::getAssignmentGivenExpression(const ProgramElement& expression) const {
    return getLeftSide(RelationshipType::Modifies, expression, ElementType::kAssignment);
}

std::set<ProgramElement> PkbGetter::getAssignmentGivenVariableAndExpression(const ProgramElement& variable, const ProgramElement& expression) const {
    std::set<ProgramElement> result;
    std::set<ProgramElement> assignments = getAssignmentGivenExpression(expression);
    for (const auto& assignment : assignments)
        if (getRightSide(RelationshipType::Modifies, assignment, ElementType::kVariable).count(variable) != 0)
            result.insert(assignment);
    return result;
}
