#include <set>

#include "PkbGetter.h"
#include "ProgramElement.h"
#include "PkbRelationshipType.h"
#include "RelationshipGetter.h"
#include "DB.h"

bool PkbGetter::isExists(const ProgramElement& elementToCheck) const {
    switch (elementToCheck.elementType) {
        case ElementType::STATEMENT:
        case ElementType::READ:
        case ElementType::PRINT:
        case ElementType::CALL:
        case ElementType::WHILE:
        case ElementType::IF:
        case ElementType::ASSIGNMENT: {
            int stmtNo = elementToCheck.stmtNo;
            auto stmt = db->elementStmtTable.find(stmtNo);
            if (stmt == db->elementStmtTable.end()) return false;
            ElementType existingType = stmt->second.elementType;
            return RelationshipGetter::isStatementTypeToGet(elementToCheck.elementType, existingType);
        }
        case ElementType::PROCEDURE:return db->procedures.count(elementToCheck.procName);
        case ElementType::VARIABLE:return db->variables.count(elementToCheck.varName);
        case ElementType::CONSTANT:return db->constants.count(elementToCheck.value);
        default:throw std::logic_error("Unknown element type to check, or didn't return");
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
        nextGetter(db),
        nextTGetter(db),
        affectsGetter(db),
        affectsTGetter(db),
        relationshipGetterMap({
                                      {PkbRelationshipType::MODIFIES, &modifiesGetter},
                                      {PkbRelationshipType::USES, &usesGetter},
                                      {PkbRelationshipType::FOLLOWS, &followsGetter},
                                      {PkbRelationshipType::FOLLOWS_T, &followsTGetter},
                                      {PkbRelationshipType::PARENT, &parentGetter},
                                      {PkbRelationshipType::PARENT_T, &parentTGetter},
                                      {PkbRelationshipType::CALLS, &callsGetter},
                                      {PkbRelationshipType::CALLS_T, &callsTGetter},
                                      {PkbRelationshipType::NEXT, &nextGetter},
                                      {PkbRelationshipType::NEXT_T, &nextTGetter},
                                      {PkbRelationshipType::AFFECTS, &affectsGetter},
                                      {PkbRelationshipType::AFFECTS_T, &affectsTGetter}
                              }) {
}

std::set<ProgramElement*> PkbGetter::getEntity(const ElementType& typeToGet) const {
    std::set<ProgramElement*> result;

    switch (typeToGet) {
        case ElementType::STATEMENT:
        case ElementType::READ:
        case ElementType::PRINT:
        case ElementType::CALL:
        case ElementType::WHILE:
        case ElementType::IF:
        case ElementType::ASSIGNMENT: {
            for (auto& p : db->elementStmtTable)
                RelationshipGetter::insertStmtElement(result, &p.second, typeToGet);
            break;
        }
        case ElementType::VARIABLE: {
            for (auto& p : db->elementVarTable)
                result.insert(&p.second);
            break;
        }
        case ElementType::PROCEDURE: {
            for (auto& p : db->elementProcTable)
                result.insert(&p.second);
            break;
        }
        case ElementType::CONSTANT: {
            for (auto& p : db->elementConstTable)
                result.insert(&p.second);
            break;
        }
        default:throw std::invalid_argument("Unknown typeToGet for getEntity");
    }

    return result;
}

bool PkbGetter::isRelationship(const PkbRelationshipType& r,
                               const ProgramElement& leftSide,
                               const ProgramElement& rightSide) {
    if (!isExists(leftSide) || !isExists(rightSide)) return false;

    return relationshipGetterMap.at(r)->isRelationship(leftSide, rightSide);
}

std::set<ProgramElement*> PkbGetter::getLeftSide(const PkbRelationshipType& r, const ProgramElement& rightSide,
                                                 const ElementType& typeToGet) {
    if (!isExists(rightSide)) return {};

    return relationshipGetterMap.at(r)->getLeftSide(rightSide, typeToGet);
}

std::set<ProgramElement*> PkbGetter::getRightSide(const PkbRelationshipType& r, const ProgramElement& leftSide,
                                                  const ElementType& typeToGet) {
    if (!isExists(leftSide)) return {};

    return relationshipGetterMap.at(r)->getRightSide(leftSide, typeToGet);
}

std::set<std::pair<ProgramElement*, ProgramElement*>> PkbGetter::getRelationshipPairs(const PkbRelationshipType& r,
                                                                                      const ElementType& leftTypeToGet,
                                                                                      const ElementType& rightTypeToGet) {
    std::set<std::pair<ProgramElement*, ProgramElement*>> result;
    std::set<ProgramElement*> leftSides = getEntity(leftTypeToGet);
    for (const auto& leftSide : leftSides)
        for (const auto& rightSide : getRightSide(r, *leftSide, rightTypeToGet))
            result.insert(std::make_pair(leftSide, rightSide));
    return result;
}

// This would give the assignments that fulfills the following expression. 
std::set<ProgramElement*> PkbGetter::getAssignmentGivenExpression(const Expr& expr,
                                                                  const ExpressionIndicator& indicator) const {
    std::set<ProgramElement*> result;
    // Converting it to an expression first
    const Expr& expr1 = expr;

    std::map<int, Expr>::iterator it;
    for (it = db->exprTable.begin(); it != db->exprTable.end(); it++) {
        // if expression does not exist. 
        if (it->second.isNullExpr()) { continue; }
        if (db->stmtTable[it->first].statementType != StatementType::ASSIGNMENT_STMT) { continue; }
        if (expressionProcessor.fullfillsMatching(expr1, it->second, indicator)) {
            // get the assignment
            result.insert(&db->elementStmtTable.at(it->first));
        }
    }
    return result;

}

std::set<ProgramElement*> PkbGetter::getAssignmentGivenVariableAndExpression(const ProgramElement& variable,
                                                                             const Expr& expr,
                                                                             const ExpressionIndicator& indicator) {

    std::set<ProgramElement*> result;
    // Converting it to an expression first
    const Expr& expr1 = expr;

    // Get the assignments that match the variable. 
    std::set<ProgramElement*>
            assignments = getLeftSide(PkbRelationshipType::MODIFIES, variable, ElementType::ASSIGNMENT);
    // Get the line numbers for the assignments only. 
    std::set<int> assignmentNo;
    for (const auto& itset : assignments) {
        assignmentNo.insert(itset->stmtNo);
    }

    // iterate through the DB stmt table to find the assignments only that match variable. 
    for (const auto it : assignmentNo) {
        // get the expression
        Expr expr2 = db->exprTable[it];
        // if it exists, we will then check if it is the correct expression 
        if (expressionProcessor.fullfillsMatching(expr1, expr2, indicator)) {

            //if it is the correct expression, we get the program element and
            //put into the results
            result.insert(&db->elementStmtTable.at(it));
        }

    }
    return result;
}

std::set<std::pair<ProgramElement*,
                   ProgramElement*>> PkbGetter::getAssignmentWithVariableGivenExpression(const Expr& expr,
                                                                                         const ExpressionIndicator& indicator) const {

    std::set<std::pair<ProgramElement*, ProgramElement*>> result;
    // Converting it to an expression first
    const Expr& expr1 = expr;

    std::map<int, Expr>::iterator it;
    for (it = db->exprTable.begin(); it != db->exprTable.end(); it++) {
        // if expression does not exist. 
        if (it->second.isNullExpr()) { continue; }
        if (expressionProcessor.fullfillsMatching(expr1, it->second, indicator)) {
            // get the assignment
            ProgramElement* second = &db->elementVarTable.at(db->stmtTable.at(it->first).varModified.at(0));
            ProgramElement* first = &db->elementStmtTable.at(it->first);
            result.insert(std::make_pair(first, second));
        }
    }
    return result;
}

std::set<ProgramElement*> PkbGetter::getIfGivenVariable(const ProgramElement& variable) const {
    std::set<ProgramElement*> result;
    std::map<int, ParsedStatement>::iterator it;
    for (it = db->stmtTable.begin(); it != db->stmtTable.end(); it++) {
        // if the statement is a if statement
        // and the variable USED in the if statement (statically)
        // contains the variable
        std::vector<std::string> varUsed = it->second.varUsed;
        bool isVarPresent = std::find(varUsed.begin(), varUsed.end(), variable.varName) != varUsed.end();
        if (it->second.statementType == StatementType::IF_STMT && isVarPresent)
            result.insert(&db->elementStmtTable.at(it->first));
    }
    return result;
}

std::set<ProgramElement*> PkbGetter::getWhileGivenVariable(const ProgramElement& variable) const {
    std::set<ProgramElement*> result;
    std::map<int, ParsedStatement>::iterator it;
    for (it = db->stmtTable.begin(); it != db->stmtTable.end(); it++) {
        // if the statement is a while statement
        // and the variable USED in the while statement (statically)
        // contains the variable
        std::vector<std::string> varUsed = it->second.varUsed;
        bool isVarPresent = std::find(varUsed.begin(), varUsed.end(), variable.varName) != varUsed.end();
        if (it->second.statementType == StatementType::WHILE_STMT && isVarPresent)
            result.insert(&db->elementStmtTable.at(it->first));
    }
    return result;
}

std::set<std::pair<ProgramElement*, ProgramElement*>> PkbGetter::getIfWithVariable() const {
    std::set<std::pair<ProgramElement*, ProgramElement*>> result;
    std::set<ProgramElement*> ifStatements = getEntity(ElementType::IF);

    for (const auto& itset : ifStatements) {
        if (db->stmtTable[itset->stmtNo].varUsed.empty()) continue;
        for (const auto& i : db->stmtTable[itset->stmtNo].varUsed) {
            // can be changed to get directly from element stmt table.
            ProgramElement* var = &db->elementVarTable.at(i);
            ProgramElement* ifStmt = itset;
            result.insert(std::make_pair(ifStmt, var));
        }
    }
    return result;
};

std::set<std::pair<ProgramElement*, ProgramElement*>> PkbGetter::getWhileWithVariable() const {
    std::set<std::pair<ProgramElement*, ProgramElement*>> result;
    std::set<ProgramElement*> whileStatements = getEntity(ElementType::WHILE);

    for (const auto& itset : whileStatements) {
        if (db->stmtTable[itset->stmtNo].varUsed.empty()) continue;
        for (const auto& i : db->stmtTable[itset->stmtNo].varUsed) {
            ProgramElement* var = &db->elementVarTable.at(i);
            ProgramElement* whileStmt = itset;
            result.insert(std::make_pair(whileStmt, var));
        }
    }
    return result;
}; 
