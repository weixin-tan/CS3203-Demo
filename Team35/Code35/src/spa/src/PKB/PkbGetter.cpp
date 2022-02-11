#include <set>
#include <cassert>

#include "PkbGetter.h"
#include "ProgramElement.h"
#include "DB.h"

// TODO: refactor to reference convertor constant value
#define NULL_STMT_NO -1

// TODO: rename
bool inline isStatementTypeToGet(const ElementType& typeToGet, const ElementType& targetType) {
    return (typeToGet == ElementType::Statement) || (typeToGet == targetType);
}

PkbGetter::PkbGetter(DB* db) : db(db) {}

bool PkbGetter::isRelationship(const RelationshipType& r, const ProgramElement &leftSide, const ProgramElement &rightSide) const {
    bool result = false;

    switch (r) {
        case RelationshipType::Modifies : {
            assert(rightSide.element_type == ElementType::Variable);
            if (isStatementType(leftSide.element_type)) {
                std::set<ProgramElement> modifyStatements = getLeftSide(r, rightSide, leftSide.element_type);
                result = modifyStatements.find(leftSide) != modifyStatements.end();
            } else if (leftSide.element_type == ElementType::Procedure) {
                assert(false);
            }
            break;
        }
        case RelationshipType::Uses: {
            assert(rightSide.element_type == ElementType::Variable);
            if (isStatementType(leftSide.element_type)) {
                std::set<ProgramElement> usesStatements = getLeftSide(r, rightSide, leftSide.element_type);
                result = usesStatements.find(leftSide) != usesStatements.end();
            } else if (leftSide.element_type == ElementType::Procedure) {
                assert(false);
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
            while (currentStmtNo != NULL_STMT_NO) {
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
            while (currentStmtNo != NULL_STMT_NO) {
                currentStmtNo = db->stmtFollowing[currentStmtNo];
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
                result.merge(getLeftSide(r, ProgramElement::createVariable(var), ElementType::Statement));
            break;
        }
        case RelationshipType::Uses: {
            for (const auto&[var, _] : db->varToUsesStmtTable)
                result.merge(getLeftSide(r, ProgramElement::createVariable(var), ElementType::Statement));
            break;
        }
        default: {
            assert(false);
        }
    }

    return result;
}

std::set<ProgramElement> PkbGetter::getEntity(const ElementType &typeToGet) const {
    std::set<ProgramElement> result;

    if (isStatementType(typeToGet)) {
        for (const auto&[stmtNo, eType]: db->stmtTypeTable)
            if (eType == typeToGet)
                result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
    }
    switch (typeToGet) {
        case ElementType::Variable: {
            for (const std::string& var: db->variables)
                result.insert(ProgramElement::createVariable(var));
            break;
        }
        case ElementType::Procedure: {
            assert(false);
        }
        default: {
            assert(false);
        }
    }

    return result;
}

std::set<ProgramElement> PkbGetter::getLeftSide(const RelationshipType& r, const ProgramElement &rightSide,
                                                const ElementType &typeToGet) const {
    std::set<ProgramElement> result;

    switch (r) {
        case RelationshipType::Modifies: {
            assert(isStatementType(typeToGet) || typeToGet == ElementType::Procedure);
            assert(rightSide.element_type == ElementType::Variable);

            if (isStatementType(typeToGet)) {
                std::set<int> stmtNos;
                for (const int &stmtNo : db->varToModifyStmtTable.at(rightSide.string_value)) {
                    int curStmtNo = stmtNo;
                    // do not revisit statements visited
                    while (curStmtNo != NULL_STMT_NO && stmtNos.find(curStmtNo) == stmtNos.end()) {
                        if (db->stmtTypeTable.at(curStmtNo) == typeToGet)
                            stmtNos.insert(curStmtNo);
                        curStmtNo = db->childToParentTable.at(curStmtNo);
                    }
                }
                for (const int& stmtNo : stmtNos)
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
            }
            break;
        }
        case RelationshipType::Uses: {
            assert(isStatementType(typeToGet) || typeToGet == ElementType::Procedure);
            assert(rightSide.element_type == ElementType::Variable);

            if (isStatementType(typeToGet)) {
                std::set<int> stmtNos;
                for (const int &stmtNo : db->varToUsesStmtTable.at(rightSide.string_value)) {
                    int curStmtNo = stmtNo;
                    // do not revisit statements visited
                    while (curStmtNo != NULL_STMT_NO && stmtNos.find(curStmtNo) == stmtNos.end()) {
                        if (db->stmtTypeTable.at(curStmtNo) == typeToGet)
                            stmtNos.insert(curStmtNo);
                        curStmtNo = db->childToParentTable.at(curStmtNo);
                    }
                }
                for (const int& stmtNo : stmtNos)
                    result.insert(ProgramElement::createStatement(typeToGet, stmtNo));
            }
            break;
        }
        case RelationshipType::Follows: {
            assert(isStatementType(rightSide.element_type) && isStatementType(typeToGet));

            int targetStmtNo = db->stmtPreceding.at(rightSide.integer_value);
            if (targetStmtNo != NULL_STMT_NO && db->stmtTypeTable.at(targetStmtNo) == typeToGet)
                result.insert(ProgramElement::createStatement(typeToGet, targetStmtNo));
            break;
        }
        case RelationshipType::Parent: {
            assert(isStatementType(rightSide.element_type) && isStatementType(typeToGet));

            int targetStmtNo = db->childToParentTable.at(rightSide.integer_value);
            if (targetStmtNo != NULL_STMT_NO && db->stmtTypeTable.at(targetStmtNo) == typeToGet)
                result.insert(ProgramElement::createStatement(typeToGet, targetStmtNo));
            break;
        }
        case RelationshipType::FollowsT: {
            assert(isStatementType(rightSide.element_type) && isStatementType(typeToGet));

            int currentStmtNo = db->stmtPreceding.at(rightSide.integer_value);
            while (currentStmtNo != NULL_STMT_NO) {
                if (db->stmtTypeTable.at(currentStmtNo) == typeToGet)
                    result.insert(ProgramElement::createStatement(typeToGet, currentStmtNo));
                currentStmtNo = db->stmtPreceding.at(currentStmtNo);
            }

            break;
        }
        case RelationshipType::ParentT: {
            assert(isStatementType(rightSide.element_type) && isStatementType(typeToGet));

            int currentStmtNo = db->childToParentTable.at(rightSide.integer_value);
            while (currentStmtNo != NULL_STMT_NO) {
                if (db->stmtTypeTable.at(currentStmtNo) == typeToGet)
                    result.insert(ProgramElement::createStatement(typeToGet, currentStmtNo));
                currentStmtNo = db->childToParentTable[currentStmtNo];
            }

            break;
        }
        default: {
            assert(false);
        }
    }

    return result;
}

std::set<ProgramElement> PkbGetter::getRightSide(const RelationshipType& r, const ProgramElement &leftSide,
                                                 const ElementType &typeToGet) const {
    std::set<ProgramElement> result;
    switch (r) {
        case RelationshipType::Modifies: {
            assert(isStatementType(leftSide.element_type) || leftSide.element_type == ElementType::Procedure);
            assert(typeToGet == ElementType::Variable);

            if (isStatementType(leftSide.element_type)) {
                for (const std::string& var : db->modifyStmtToVarTable.at(leftSide.integer_value))
                    result.insert(ProgramElement::createVariable(var));

                for (const int& childStmtNo: db->parentToChildTable.at(leftSide.integer_value))
                    result.merge(getRightSide(r, ProgramElement::createStatement(ElementType::Statement, childStmtNo), typeToGet));
            } else {
                assert(false);
            }
            break;
        }
        case RelationshipType::Uses: {
            assert(isStatementType(leftSide.element_type) || leftSide.element_type == ElementType::Procedure);
            assert(typeToGet == ElementType::Variable);

            if (isStatementType(leftSide.element_type)) {
                for (const std::string& var : db->usesStmtToVarTable.at(leftSide.integer_value))
                    result.insert(ProgramElement::createVariable(var));

                for (const int& childStmtNo: db->parentToChildTable.at(leftSide.integer_value))
                    result.merge(getRightSide(r, ProgramElement::createStatement(ElementType::Statement, childStmtNo), typeToGet));
            } else {
                assert(false);
            }
            break;
        }
        case RelationshipType::Follows: {
            assert(isStatementType(leftSide.element_type) && isStatementType(typeToGet));

            int targetStmtNo = db->stmtFollowing.at(leftSide.integer_value);
            if (targetStmtNo != NULL_STMT_NO && isStatementTypeToGet(typeToGet, db->stmtTypeTable.at(targetStmtNo)))
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
            while (currentStmtNo != NULL_STMT_NO) {
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
                if (childStmtType == typeToGet)
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
