#include "PkbSetter.h"
#include "../SP/ParsedStatement.h"
#include "../SP/StatementType.h"
#include "ElementType.h"

const std::map<StatementType, ElementType> PkbSetter::spTypeToElementTypeTable = {
        {StatementType::ASSIGNMENT_STMT, ElementType::ASSIGNMENT},
        {StatementType::PRINT_STMT,  ElementType::PRINT},
        {StatementType::CALL_STMT,   ElementType::CALL},
        {StatementType::IF_STMT,     ElementType::IF},
        {StatementType::WHILE_STMT,  ElementType::WHILE},
        {StatementType::READ_STMT,   ElementType::READ}
};

PkbSetter::PkbSetter(DB* db) : db(db), designExtractor(db), pkbValidator(db) {}

void PkbSetter::handleStatement(const ParsedStatement& parsedStatement) {
    db->stmtTable[parsedStatement.stmtNo] = parsedStatement;
    db->elementStmtTable.insert({parsedStatement.stmtNo, convertParsedStatement(parsedStatement)});
}

void PkbSetter::handleVariables(const ParsedStatement& parsedStatement) {
    for (const auto& var: parsedStatement.varModified) {
        db->variables.insert(var);
        db->elementVarTable.insert({var, ProgramElement::createVariable(var)});
    }
    for (const auto& var: parsedStatement.varUsed) {
        db->variables.insert(var);
        db->elementVarTable.insert({var, ProgramElement::createVariable(var)});
    }
}

void PkbSetter::handleProcedure(const ParsedStatement& parsedStatement) {
    db->procedures.insert(parsedStatement.procedureName);
    db->elementProcTable.insert({parsedStatement.procedureName, ProgramElement::createProcedure(parsedStatement.procedureName)});
}

void PkbSetter::handleConstants(const ParsedStatement& statement) {
    for (const std::string& c : statement.constant) {
        db->constants.insert(c);
        db->elementConstTable.insert({c, ProgramElement::createConstant(c)});
    }
}

void PkbSetter::handleExpression(const ParsedStatement& statement) {
    db->exprTable[statement.stmtNo] = statement.pattern;
}

ProgramElement PkbSetter::convertParsedStatement(const ParsedStatement& statement) {
    ElementType elementType = spTypeToElementTypeTable.at(statement.statementType);
    std::string procOrVarName = ProgramElement::nullStringValue;
    if (elementType == ElementType::READ)
        procOrVarName = *statement.varModified.begin();
    if (elementType == ElementType::PRINT)
        procOrVarName = *statement.varUsed.begin();
    if (elementType == ElementType::CALL)
        procOrVarName = statement.procedureCalled;
    return ProgramElement::createStatement(elementType, statement.stmtNo, procOrVarName);
}

void PkbSetter::insertStmt(const ParsedStatement& parsedStatement) {
    // handle entity
    handleStatement(parsedStatement);
    handleVariables(parsedStatement);
    handleConstants(parsedStatement);
    handleProcedure(parsedStatement);
    handleExpression(parsedStatement);
}

void PkbSetter::insertStmts(const std::vector<std::vector<ParsedStatement>>& procedures, bool testing) {
    // insert legacy stuff, might be deprecated
    for (const auto& procedure : procedures)
        for (const auto& parsedStatement : procedure)
            insertStmt(parsedStatement);

    designExtractor.precompute();
    pkbValidator.validate(procedures, testing);
}
